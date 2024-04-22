

#include "./can/bsp_can_comm.h"

CAN_HandleTypeDef Can_Handle;
CAN_TxHeaderTypeDef TxMessage;		
CAN_RxHeaderTypeDef RxMessage;		
uint8_t               TxData[8];    //���ͻ�����
uint8_t               RxData[8];    //���ջ�����
uint32_t              TxMailbox;

extern __IO uint32_t flag ;		 //���ڱ�־�Ƿ���յ����ݣ����жϺ����и�ֵ
/*
 * ��������CAN_GPIO_Config
 * ����  ��CAN��GPIO ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
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

    /* ����CAN�������� */
    GPIO_InitStructure.Pin = CAN_RX_PIN ;
    HAL_GPIO_Init(CAN_RX_GPIO_PORT, &GPIO_InitStructure);
}

/*
 * ��������CAN_NVIC_Config
 * ����  ��CAN��NVIC ����,��1���ȼ��飬0��0���ȼ�
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_NVIC_Config(void)
{
	/* ������ռ���ȼ��ķ��� */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
	/*�ж����ã���ռ���ȼ�0�������ȼ�Ϊ0*/
	HAL_NVIC_SetPriority(CAN_RX_IRQ, 0 ,0);
	HAL_NVIC_EnableIRQ(CAN_RX_IRQ);
}

/*
 * ��������CAN_Mode_Config
 * ����  ��CAN��ģʽ ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_Mode_Config(void)
{
    CAN_CLK_ENABLE();   /* ʹ��CANʱ�� */
    
    Can_Handle.Instance = CANx;
	Can_Handle.Init.TimeTriggeredMode = DISABLE;			   //MCR-TTCM  �ر�ʱ�䴥��ͨ��ģʽʹ�ܣ�ʱ���
	Can_Handle.Init.AutoBusOff = ENABLE;			   //MCR-ABOM  �Զ����߹��� 
	Can_Handle.Init.AutoWakeUp = ENABLE;			   //MCR-AWUM  ʹ���Զ�����ģʽ��CAN����ɽ���͹���ģʽ
	Can_Handle.Init.AutoRetransmission = DISABLE;			   //MCR-NART  ��ֹ�����Զ��ش�	  DISABLE-�Զ��ش�
	Can_Handle.Init.ReceiveFifoLocked = DISABLE;			   //MCR-RFLM  ����FIFO ����ģʽ  DISABLE-���ʱ�±��ĻḲ��ԭ�б���  
	Can_Handle.Init.TransmitFifoPriority = DISABLE;			   //MCR-TXFP  ����FIFO���ȼ� DISABLE-���ȼ�ȡ���ڱ��ı�ʾ�� 
	Can_Handle.Init.Mode = CAN_MODE_LOOPBACK;  //�ػ�ģʽ
	Can_Handle.Init.SyncJumpWidth = CAN_SJW_1TQ;		   //BTR-SJW ����ͬ����Ծ��� 2��ʱ�䵥Ԫ
	 
	/* ss=1 bs1=5 bs2=3 λʱ����Ϊ(1+5+3) �����ʼ�Ϊʱ������tq*(1+3+6)  */
	Can_Handle.Init.TimeSeg1 = CAN_BS1_4TQ;		   //BTR-TS1 ʱ���1 ռ����4��ʱ�䵥Ԫ
	Can_Handle.Init.TimeSeg2 = CAN_BS2_2TQ;		   //BTR-TS1 ʱ���2 ռ����2��ʱ�䵥Ԫ	
	
	/* CAN Baudrate = 1 MBps (1MBps��Ϊstm32��CAN�������) (CAN ʱ��Ƶ��Ϊ APB 1 = 42 MHz) */
	Can_Handle.Init.Prescaler = 6;		   ////BTR-BRP �����ʷ�Ƶ��  ������ʱ�䵥Ԫ��ʱ�䳤�� 42/(1+4+2)/6 = 1 Mbps
	HAL_CAN_Init(&Can_Handle);
}

/*
 * ��������CAN_Filter_Config
 * ����  ��CAN�Ĺ����� ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_Filter_Config(void)
{
    CAN_FilterTypeDef  CAN_FilterInitStructure;

    /*CANɸѡ����ʼ��*/
    CAN_FilterInitStructure.FilterBank = 0;						//ɸѡ����0
    CAN_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDMASK;	    //����������ģʽ
    CAN_FilterInitStructure.FilterScale = CAN_FILTERSCALE_32BIT;	//ɸѡ��λ��Ϊ����32λ��
    /* ʹ��ɸѡ�������ձ�־�����ݽ��бȶ�ɸѡ����չID�������µľ����������ǵĻ��������FIFO0�� */

    CAN_FilterInitStructure.FilterIdHigh =  ((((uint32_t)0x1314<<3) | CAN_ID_EXT | CAN_RTR_DATA) & 0xFFFF0000) >> 16;		//Ҫɸѡ��ID��λ 
    CAN_FilterInitStructure.FilterIdLow =   (( (uint32_t)0x1314<<3) | CAN_ID_EXT | CAN_RTR_DATA) & 0xFFFF; //Ҫɸѡ��ID��λ 
    CAN_FilterInitStructure.FilterMaskIdHigh = 0xFFFF;			//ɸѡ����16λÿλ����ƥ��
    CAN_FilterInitStructure.FilterMaskIdLow = 0xFFFF;			//ɸѡ����16λÿλ����ƥ��
    CAN_FilterInitStructure.FilterFIFOAssignment = CAN_FILTER_FIFO0 ;	//ɸѡ����������FIFO0
    CAN_FilterInitStructure.FilterActivation = ENABLE;			//ʹ��ɸѡ��
    HAL_CAN_ConfigFilter(&Can_Handle,&CAN_FilterInitStructure);
}





/**
  * @brief  ��ʼ�� Rx Message���ݽṹ��
  * @param  RxMessage: ָ��Ҫ��ʼ�������ݽṹ��
  * @retval None
  */
void Init_RxMes(void)
{
  uint8_t ubCounter = 0;

  /*�ѽ��սṹ������*/
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
  Init_RxMes();     /* ������� */
  HAL_CAN_Start(&Can_Handle);
  __HAL_CAN_ENABLE_IT(&Can_Handle,CAN_IT_RX_FIFO0_MSG_PENDING);	
}


/*
 * ��������CAN_SetMsg
 * ����  ��CANͨ�ű�����������,����һ����������Ϊ0-7�����ݰ�
 * ����  �����ͱ��Ľṹ��
 * ���  : ��
 * ����  ���ⲿ����
 */	 
void CAN_SetMsg(void)
{	  
    uint8_t ubCounter = 0;
    TxMessage.StdId=0x00;						 
    TxMessage.ExtId=0x1314;					 //ʹ�õ���չID
    TxMessage.IDE=CAN_ID_EXT;				  //��չģʽ
    TxMessage.RTR=CAN_RTR_DATA;				 //���͵�������
    TxMessage.DLC=8;							 //���ݳ���Ϊ8�ֽ�
    TxMessage.TransmitGlobalTime = DISABLE;
        
    /*����Ҫ���͵�����0-7*/
    for (ubCounter = 0; ubCounter < 8; ubCounter++)
    {
        TxData[ubCounter] = ubCounter;
    }
    
    /* ����CAN����֡,CAN�����֡���ݣ����͵����ݵ�ַ�����͵����� */
    HAL_CAN_AddTxMessage(&Can_Handle, &TxMessage, TxData, &TxMailbox);
    while(HAL_CAN_GetTxMailboxesFreeLevel(&Can_Handle) != 3);   /* F4��3���������䣬�ȿ�������Ϊ3�������ͳɹ� */
    
    // if(HAL_CAN_GetRxFifoFillLevel(&Can_Handle, CAN_RX_FIFO0) != 1)
    // {
    //      /* Reception Missing */
    // //    Error_Handler();
    // }

    // /* �ӽ��������ȡ���� */
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
  * @brief  CAN��������ж�(������) 
  * @param  hcan: CAN���ָ��
  * @retval ��
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
	// /* �Ƚ�ID�Ƿ�Ϊ0x1314 */ 
	// if((RxMessage.ExtId==0x1314) && (RxMessage.IDE==CAN_ID_EXT) && (RxMessage.DLC==8) )
	// {
	// 	flag = 1; //���ճɹ�  
	// }
	// else
	// {
	// 	flag = 0; //����ʧ��
	// }
    /* �ӽ��������ȡ���� */
    if(HAL_CAN_GetRxMessage(&Can_Handle, CAN_RX_FIFO0, &RxMessage, RxData) != HAL_OK)
    {
         /* Reception Error */
        //Error_Handler();
    }
	//HAL_CAN_Receive_IT(&Can_Handle, CAN_IT_RX_FIFO0_MSG_PENDING); /* ���ж� */
    __HAL_CAN_ENABLE_IT(&Can_Handle, CAN_IT_RX_FIFO0_MSG_PENDING);
}




/**
  * @brief  CAN����ص�����
  * @param  hcan: CAN���ָ��
  * @retval ��
  */
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	printf("\r\nCAN����\r\n");
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
