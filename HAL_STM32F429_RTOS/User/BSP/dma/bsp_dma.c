#include "./bsp_dma.h"


/* 相关宏定义，使用存储器到存储器传输必须使用DMA2 */

/* 定义aSRC_Const_Buffer数组作为DMA传输数据源
  const关键字将aSRC_Const_Buffer数组变量定义为常量类型 */
const uint32_t aSRC_Const_Buffer[]= {
                                    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
                                    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
                                    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
                                    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
                                    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
                                    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
                                    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
                                    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};
                    
#define BUFFER_SIZE     (sizeof(aSRC_Const_Buffer) / sizeof(aSRC_Const_Buffer[0]))


uint32_t aDST_Buffer[BUFFER_SIZE];  /* 定义DMA传输目标存储器 */


DMA_HandleTypeDef DMA_M2M_Handler;

DMA_HandleTypeDef  DMA_Handle;      //DMA句柄

/**
  * 判断指定长度的两个数据源是否完全相等，
  * 如果完全相等返回1，只要其中一对数据不相等返回0
  */
static uint8_t Buffercmp(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength)
{
	/* 数据长度递减 */
	while(BufferLength--)
	{
		/* 判断两个数据源是否对应相等 */
		if(*pBuffer != *pBuffer1)
		{
		  /* 对应数据源不相等马上退出函数，并返回0 */
		  return 0;
		}
		/* 递增两个数据源的地址指针 */
		pBuffer++;
		pBuffer1++;
	}
	/* 完成判断并且对应数据相对 */
	return 1;  
}



/**
  * DMA传输配置
  */
void DMA_MemToMem_Config(void)
{
	DMA_STREAM_CLOCK();     /* 使能DMA时钟 */

	DMA_M2M_Handler.Instance = DMA_STREAM;                       /* 流 */
	DMA_M2M_Handler.Init .Channel = DMA_CHANNEL;                 /* DMA数据流通道选择 */
	DMA_M2M_Handler.Init.Direction = DMA_MEMORY_TO_MEMORY;       /* 存储器到存储器模式 */
	DMA_M2M_Handler.Init.PeriphInc = DMA_PINC_ENABLE;            /* 使能自动递增功能 */
	DMA_M2M_Handler.Init.MemInc = DMA_MINC_ENABLE;               /* 使能自动递增功能 */
	DMA_M2M_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;      /* 源数据是字大小(32位) */
	DMA_M2M_Handler.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;         /* 目标数据也是字大小(32位) */
	DMA_M2M_Handler.Init.Mode = DMA_NORMAL;                      /* 一次传输模式，存储器到存储器模式不能使用循环传输 */
	DMA_M2M_Handler.Init.Priority = DMA_PRIORITY_HIGH;           /* DMA数据流优先级为高 */
	DMA_M2M_Handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;        /* 禁用FIFO模式 */ 
	DMA_M2M_Handler.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	DMA_M2M_Handler.Init.MemBurst = DMA_MBURST_SINGLE;           /* 单次模式 */
	DMA_M2M_Handler.Init.PeriphBurst = DMA_PBURST_SINGLE;        /* 单次模式 */
	HAL_DMA_Init(&DMA_M2M_Handler);                              /* 初始化DMA数据流参数配置 */

}




void DMA_MemToMem_Test(void)
{
    HAL_StatusTypeDef DMA_status = HAL_ERROR;
	uint8_t TransferStatus;

    /* 启动DMA数据流传输, */
	DMA_status = HAL_DMA_Start(&DMA_M2M_Handler,(uint32_t)aSRC_Const_Buffer,(uint32_t)aDST_Buffer,BUFFER_SIZE);

	if (DMA_status != HAL_OK)
	{
		/* DMA出错就让程序运行下面循环：RGB彩色灯闪烁 */   
			LED_RED;

	} 

	/* 等待DMA传输完成 */
	while(__HAL_DMA_GET_FLAG(&DMA_M2M_Handler,DMA_FLAG_TCIF0_4)==DISABLE)
	{

	}

	/* 比较源数据与传输后数据 */
	TransferStatus=Buffercmp(aSRC_Const_Buffer, aDST_Buffer, BUFFER_SIZE);

	/* 判断源数据与传输后数据比较结果*/
	if(TransferStatus==0)  
	{
		/* 源数据与传输后数据不相等时RGB彩色灯显示红色 */
		LED_RED;
	}
	else
	{ 
		/* 源数据与传输后数据相等时RGB彩色灯显示蓝色 */
		LED_BLUE;
	}	
}


/**
  * @brief  USART1 TX DMA 配置，内存到外设(USART1->DR)
  * @param  无
  * @retval 无
  */
void Debug_Uart_Dma_Config(void)
{
	Debug_Uart_DMA_CLK_ENABLE();  
	
	//Tx DMA配置
	DMA_Handle.Instance = Debug_Uart_DMA_STREAM;             	//数据流选择
	DMA_Handle.Init.Channel = Debug_Uart_DMA_CHANNEL;              //通道选择
	DMA_Handle.Init.Direction = DMA_MEMORY_TO_PERIPH;             //存储器到外设
	DMA_Handle.Init.PeriphInc = DMA_PINC_DISABLE;                 //外设非增量模式
	DMA_Handle.Init.MemInc = DMA_MINC_ENABLE;                     //存储器增量模式
	DMA_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
	DMA_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
	DMA_Handle.Init.Mode = DMA_NORMAL;                            //外设普通模式
	DMA_Handle.Init.Priority = DMA_PRIORITY_MEDIUM;               //中等优先级
	DMA_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;              //禁用FIFO
	DMA_Handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;      
	DMA_Handle.Init.MemBurst = DMA_MBURST_SINGLE;                 //存储器突发单次传输
	DMA_Handle.Init.PeriphBurst = DMA_PBURST_SINGLE;              //外设突发单次传输
	
	HAL_DMA_Init(&DMA_Handle);
	/* Associate the DMA handle */
	__HAL_LINKDMA(&DebugUartHandler, hdmatx, DMA_Handle); 
  
}




void Debug_Uart_Dma_Test(void)
{
	uint8_t SendBuff[SENDBUFF_SIZE];

	/*填充将要发送的数据*/
	for(uint8_t i=0;i<SENDBUFF_SIZE;i++)
	{
		SendBuff[i]	 = 'A';
		
	}

	HAL_UART_Transmit_DMA(&DebugUartHandler, (uint8_t *)SendBuff ,SENDBUFF_SIZE);

	for(uint32_t i=0;i<0xfffff;i++);
}











