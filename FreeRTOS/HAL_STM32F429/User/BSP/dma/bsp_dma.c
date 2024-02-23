#include "./bsp_dma.h"


/* ��غ궨�壬ʹ�ô洢�����洢���������ʹ��DMA2 */

/* ����aSRC_Const_Buffer������ΪDMA��������Դ
  const�ؼ��ֽ�aSRC_Const_Buffer�����������Ϊ�������� */
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


uint32_t aDST_Buffer[BUFFER_SIZE];  /* ����DMA����Ŀ��洢�� */


DMA_HandleTypeDef DMA_M2M_Handler;

DMA_HandleTypeDef  DMA_Handle;      //DMA���

/**
  * �ж�ָ�����ȵ���������Դ�Ƿ���ȫ��ȣ�
  * �����ȫ��ȷ���1��ֻҪ����һ�����ݲ���ȷ���0
  */
static uint8_t Buffercmp(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength)
{
	/* ���ݳ��ȵݼ� */
	while(BufferLength--)
	{
		/* �ж���������Դ�Ƿ��Ӧ��� */
		if(*pBuffer != *pBuffer1)
		{
		  /* ��Ӧ����Դ����������˳�������������0 */
		  return 0;
		}
		/* ������������Դ�ĵ�ַָ�� */
		pBuffer++;
		pBuffer1++;
	}
	/* ����жϲ��Ҷ�Ӧ������� */
	return 1;  
}



/**
  * DMA��������
  */
void DMA_MemToMem_Config(void)
{
	DMA_STREAM_CLOCK();     /* ʹ��DMAʱ�� */

	DMA_M2M_Handler.Instance = DMA_STREAM;                       /* �� */
	DMA_M2M_Handler.Init .Channel = DMA_CHANNEL;                 /* DMA������ͨ��ѡ�� */
	DMA_M2M_Handler.Init.Direction = DMA_MEMORY_TO_MEMORY;       /* �洢�����洢��ģʽ */
	DMA_M2M_Handler.Init.PeriphInc = DMA_PINC_ENABLE;            /* ʹ���Զ��������� */
	DMA_M2M_Handler.Init.MemInc = DMA_MINC_ENABLE;               /* ʹ���Զ��������� */
	DMA_M2M_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;      /* Դ�������ִ�С(32λ) */
	DMA_M2M_Handler.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;         /* Ŀ������Ҳ���ִ�С(32λ) */
	DMA_M2M_Handler.Init.Mode = DMA_NORMAL;                      /* һ�δ���ģʽ���洢�����洢��ģʽ����ʹ��ѭ������ */
	DMA_M2M_Handler.Init.Priority = DMA_PRIORITY_HIGH;           /* DMA���������ȼ�Ϊ�� */
	DMA_M2M_Handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;        /* ����FIFOģʽ */ 
	DMA_M2M_Handler.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	DMA_M2M_Handler.Init.MemBurst = DMA_MBURST_SINGLE;           /* ����ģʽ */
	DMA_M2M_Handler.Init.PeriphBurst = DMA_PBURST_SINGLE;        /* ����ģʽ */
	HAL_DMA_Init(&DMA_M2M_Handler);                              /* ��ʼ��DMA�������������� */

}




void DMA_MemToMem_Test(void)
{
    HAL_StatusTypeDef DMA_status = HAL_ERROR;
	uint8_t TransferStatus;

    /* ����DMA����������, */
	DMA_status = HAL_DMA_Start(&DMA_M2M_Handler,(uint32_t)aSRC_Const_Buffer,(uint32_t)aDST_Buffer,BUFFER_SIZE);

	if (DMA_status != HAL_OK)
	{
		/* DMA������ó�����������ѭ����RGB��ɫ����˸ */   
			LED_RED;

	} 

	/* �ȴ�DMA������� */
	while(__HAL_DMA_GET_FLAG(&DMA_M2M_Handler,DMA_FLAG_TCIF0_4)==DISABLE)
	{

	}

	/* �Ƚ�Դ�����봫������� */
	TransferStatus=Buffercmp(aSRC_Const_Buffer, aDST_Buffer, BUFFER_SIZE);

	/* �ж�Դ�����봫������ݱȽϽ��*/
	if(TransferStatus==0)  
	{
		/* Դ�����봫������ݲ����ʱRGB��ɫ����ʾ��ɫ */
		LED_RED;
	}
	else
	{ 
		/* Դ�����봫����������ʱRGB��ɫ����ʾ��ɫ */
		LED_BLUE;
	}	
}


/**
  * @brief  USART1 TX DMA ���ã��ڴ浽����(USART1->DR)
  * @param  ��
  * @retval ��
  */
void Debug_Uart_Dma_Config(void)
{
	Debug_Uart_DMA_CLK_ENABLE();  
	
	//Tx DMA����
	DMA_Handle.Instance = Debug_Uart_DMA_STREAM;             	//������ѡ��
	DMA_Handle.Init.Channel = Debug_Uart_DMA_CHANNEL;              //ͨ��ѡ��
	DMA_Handle.Init.Direction = DMA_MEMORY_TO_PERIPH;             //�洢��������
	DMA_Handle.Init.PeriphInc = DMA_PINC_DISABLE;                 //���������ģʽ
	DMA_Handle.Init.MemInc = DMA_MINC_ENABLE;                     //�洢������ģʽ
	DMA_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
	DMA_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
	DMA_Handle.Init.Mode = DMA_NORMAL;                            //������ͨģʽ
	DMA_Handle.Init.Priority = DMA_PRIORITY_MEDIUM;               //�е����ȼ�
	DMA_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;              //����FIFO
	DMA_Handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;      
	DMA_Handle.Init.MemBurst = DMA_MBURST_SINGLE;                 //�洢��ͻ�����δ���
	DMA_Handle.Init.PeriphBurst = DMA_PBURST_SINGLE;              //����ͻ�����δ���
	
	HAL_DMA_Init(&DMA_Handle);
	/* Associate the DMA handle */
	__HAL_LINKDMA(&DebugUartHandler, hdmatx, DMA_Handle); 
  
}




void Debug_Uart_Dma_Test(void)
{
	uint8_t SendBuff[SENDBUFF_SIZE];

	/*��佫Ҫ���͵�����*/
	for(uint8_t i=0;i<SENDBUFF_SIZE;i++)
	{
		SendBuff[i]	 = 'A';
		
	}

	HAL_UART_Transmit_DMA(&DebugUartHandler, (uint8_t *)SendBuff ,SENDBUFF_SIZE);

	for(uint32_t i=0;i<0xfffff;i++);
}











