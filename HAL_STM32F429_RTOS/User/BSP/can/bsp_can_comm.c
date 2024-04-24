

#include "./can/bsp_can_comm.h"

CAN_HandleTypeDef Can_Handle;
CAN_TxHeaderTypeDef TxMessage;		
CAN_RxHeaderTypeDef RxMessage;		
uint8_t               TxData[8];    //发送缓冲区
uint8_t               RxData[8];    //接收缓冲区
uint32_t              TxMailbox;

extern __IO uint32_t flag ;		 //用于标志是否接收到数据，在中断函数中赋值
/*
 * 函数名：CAN_GPIO_Config
 * 描述  ：CAN的GPIO 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    CAN_TX_GPIO_CLK_ENABLE();
    CAN_RX_GPIO_CLK_ENABLE();	

    GPIO_InitStructure.Pin = CAN_TX_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Pull  = GPIO_PULLUP;
    GPIO_InitStructure.Alternate =  GPIO_AF9_CAN1;
    HAL_GPIO_Init(CAN_TX_GPIO_PORT, &GPIO_InitStructure);

    /* 配置CAN接收引脚 */
    GPIO_InitStructure.Pin = CAN_RX_PIN ;
    HAL_GPIO_Init(CAN_RX_GPIO_PORT, &GPIO_InitStructure);
}

/*
 * 函数名：CAN_NVIC_Config
 * 描述  ：CAN的NVIC 配置,第1优先级组，0，0优先级
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_NVIC_Config(void)
{
	/* 配置抢占优先级的分组 */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
	/*中断设置，抢占优先级0，子优先级为0*/
	HAL_NVIC_SetPriority(CAN_RX_IRQ, 0 ,0);
	HAL_NVIC_EnableIRQ(CAN_RX_IRQ);
}

/*
 * 函数名：CAN_Mode_Config
 * 描述  ：CAN的模式 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_Mode_Config(void)
{
    CAN_CLK_ENABLE();   /* 使能CAN时钟 */
    
    Can_Handle.Instance = CANx;
	Can_Handle.Init.TimeTriggeredMode = DISABLE;			   //MCR-TTCM  关闭时间触发通信模式使能，时间戳
	Can_Handle.Init.AutoBusOff = ENABLE;			   //MCR-ABOM  自动离线管理 
	Can_Handle.Init.AutoWakeUp = ENABLE;			   //MCR-AWUM  使用自动唤醒模式，CAN外设可进入低功耗模式
	Can_Handle.Init.AutoRetransmission = DISABLE;			   //MCR-NART  禁止报文自动重传	  DISABLE-自动重传
	Can_Handle.Init.ReceiveFifoLocked = DISABLE;			   //MCR-RFLM  接收FIFO 锁定模式  DISABLE-溢出时新报文会覆盖原有报文  
	Can_Handle.Init.TransmitFifoPriority = DISABLE;			   //MCR-TXFP  发送FIFO优先级 DISABLE-优先级取决于报文标示符 
	Can_Handle.Init.Mode = CAN_MODE_LOOPBACK;  //回环模式
	Can_Handle.Init.SyncJumpWidth = CAN_SJW_1TQ;		   //BTR-SJW 重新同步跳跃宽度 2个时间单元
	 
	/* ss=1 bs1=5 bs2=3 位时间宽度为(1+5+3) 波特率即为时钟周期tq*(1+3+6)  */
	Can_Handle.Init.TimeSeg1 = CAN_BS1_4TQ;		   //BTR-TS1 时间段1 占用了4个时间单元
	Can_Handle.Init.TimeSeg2 = CAN_BS2_2TQ;		   //BTR-TS1 时间段2 占用了2个时间单元	
	
	/* CAN Baudrate = 1 MBps (1MBps已为stm32的CAN最高速率) (CAN 时钟频率为 APB 1 = 42 MHz) */
	Can_Handle.Init.Prescaler = 6;		   ////BTR-BRP 波特率分频器  定义了时间单元的时间长度 42/(1+4+2)/6 = 1 Mbps
	HAL_CAN_Init(&Can_Handle);
}

/*
 * 函数名：CAN_Filter_Config
 * 描述  ：CAN的过滤器 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_Filter_Config(void)
{
    CAN_FilterTypeDef  CAN_FilterInitStructure;

    /*CAN筛选器初始化*/
    CAN_FilterInitStructure.FilterBank = 0;						//筛选器组0
    CAN_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDMASK;	    //工作在掩码模式
    CAN_FilterInitStructure.FilterScale = CAN_FILTERSCALE_32BIT;	//筛选器位宽为单个32位。
    /* 使能筛选器，按照标志的内容进行比对筛选，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。 */

    CAN_FilterInitStructure.FilterIdHigh =  ((((uint32_t)0x1314<<3) | CAN_ID_EXT | CAN_RTR_DATA) & 0xFFFF0000) >> 16;		//要筛选的ID高位 
    CAN_FilterInitStructure.FilterIdLow =   (( (uint32_t)0x1314<<3) | CAN_ID_EXT | CAN_RTR_DATA) & 0xFFFF; //要筛选的ID低位 
    CAN_FilterInitStructure.FilterMaskIdHigh = 0xFFFF;			//筛选器高16位每位必须匹配
    CAN_FilterInitStructure.FilterMaskIdLow = 0xFFFF;			//筛选器低16位每位必须匹配
    CAN_FilterInitStructure.FilterFIFOAssignment = CAN_FILTER_FIFO0 ;	//筛选器被关联到FIFO0
    CAN_FilterInitStructure.FilterActivation = ENABLE;			//使能筛选器
    HAL_CAN_ConfigFilter(&Can_Handle,&CAN_FilterInitStructure);
}





/**
  * @brief  初始化 Rx Message数据结构体
  * @param  RxMessage: 指向要初始化的数据结构体
  * @retval None
  */
void Init_RxMes(void)
{
  uint8_t ubCounter = 0;

  /*把接收结构体清零*/
  RxMessage.StdId = 0x00;
  RxMessage.ExtId = 0x00;
  RxMessage.IDE = CAN_ID_STD;
  RxMessage.DLC = 0;

  for (ubCounter = 0; ubCounter < 8; ubCounter++)
  {
    RxData[ubCounter] = 0x00;
  }
}

void CAN_Config(void)
{
  CAN_GPIO_Config();
  CAN_NVIC_Config();
  CAN_Mode_Config();
  CAN_Filter_Config();
  Init_RxMes();     /* 清空数据 */
  HAL_CAN_Start(&Can_Handle);
  __HAL_CAN_ENABLE_IT(&Can_Handle,CAN_IT_RX_FIFO0_MSG_PENDING);	
}


/*
 * 函数名：CAN_SetMsg
 * 描述  ：CAN通信报文内容设置,设置一个数据内容为0-7的数据包
 * 输入  ：发送报文结构体
 * 输出  : 无
 * 调用  ：外部调用
 */	 
void CAN_SetMsg(void)
{	  
    uint8_t ubCounter = 0;
    TxMessage.StdId=0x00;						 
    TxMessage.ExtId=0x1314;					 //使用的扩展ID
    TxMessage.IDE=CAN_ID_EXT;				  //扩展模式
    TxMessage.RTR=CAN_RTR_DATA;				 //发送的是数据
    TxMessage.DLC=8;							 //数据长度为8字节
    TxMessage.TransmitGlobalTime = DISABLE;
        
    /*设置要发送的数据0-7*/
    for (ubCounter = 0; ubCounter < 8; ubCounter++)
    {
        TxData[ubCounter] = ubCounter;
    }
    
    /* 发送CAN数据帧,CAN句柄，帧数据，发送的数据地址，发送的邮箱 */
    HAL_CAN_AddTxMessage(&Can_Handle, &TxMessage, TxData, &TxMailbox);
    while(HAL_CAN_GetTxMailboxesFreeLevel(&Can_Handle) != 3);   /* F4有3个发送邮箱，等空闲邮箱为3个即发送成功 */
    
    // if(HAL_CAN_GetRxFifoFillLevel(&Can_Handle, CAN_RX_FIFO0) != 1)
    // {
    //      /* Reception Missing */
    // //    Error_Handler();
    // }

    // /* 从接收邮箱获取数据 */
    // if(HAL_CAN_GetRxMessage(&Can_Handle, CAN_RX_FIFO0, &RxMessage, RxData) != HAL_OK)
    // {
    //      /* Reception Error */
    //     //Error_Handler();
    // }
    
    // if((RxMessage.ExtId == 0x1314) && (RxMessage.IDE == CAN_ID_EXT) && (RxMessage.DLC == 8))
    // {
    //      (void)0;
    //      flag = 1;
    // }
  

}

/**
  * @brief  CAN接收完成中断(非阻塞) 
  * @param  hcan: CAN句柄指针
  * @retval 无
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
	// /* 比较ID是否为0x1314 */ 
	// if((RxMessage.ExtId==0x1314) && (RxMessage.IDE==CAN_ID_EXT) && (RxMessage.DLC==8) )
	// {
	// 	flag = 1; //接收成功  
	// }
	// else
	// {
	// 	flag = 0; //接收失败
	// }
    /* 从接收邮箱获取数据 */
    if(HAL_CAN_GetRxMessage(&Can_Handle, CAN_RX_FIFO0, &RxMessage, RxData) != HAL_OK)
    {
         /* Reception Error */
        //Error_Handler();
    }
	//HAL_CAN_Receive_IT(&Can_Handle, CAN_IT_RX_FIFO0_MSG_PENDING); /* 清中断 */
    __HAL_CAN_ENABLE_IT(&Can_Handle, CAN_IT_RX_FIFO0_MSG_PENDING);
}




/**
  * @brief  CAN错误回调函数
  * @param  hcan: CAN句柄指针
  * @retval 无
  */
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	printf("\r\nCAN出错\r\n");
}


void CAN_RX_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&Can_Handle);
}

void CAN1_TX_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&Can_Handle);
}

/**************************END OF FILE************************************/
