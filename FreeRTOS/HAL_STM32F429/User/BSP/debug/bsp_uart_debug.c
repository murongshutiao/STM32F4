#include "./debug/bsp_uart_debug.h"

UART_HandleTypeDef DebugUartHandler;


 /**
  * @brief  DEBUG_USART GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */  
void Debug_Uart_Config(void)
{ 
  
  DebugUartHandler.Instance          = DEBUG_USART;
  DebugUartHandler.Init.BaudRate     = DEBUG_USART_BAUDRATE;
  DebugUartHandler.Init.WordLength   = UART_WORDLENGTH_8B;
  DebugUartHandler.Init.StopBits     = UART_STOPBITS_1;
  DebugUartHandler.Init.Parity       = UART_PARITY_NONE;
  DebugUartHandler.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  DebugUartHandler.Init.Mode         = UART_MODE_TX_RX;
  
  HAL_UART_Init(&DebugUartHandler);
    
 /*ʹ�ܴ��ڽ��ն� */
  __HAL_UART_ENABLE_IT(&DebugUartHandler,UART_IT_RXNE);  
}


/**
  * @brief UART MSP ��ʼ�� 
  * @param huart: UART handle
  * @retval ��
  */
void Debug_Uart_Init(void)
{  
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    DEBUG_USART_CLK_ENABLE();
        
    DEBUG_USART_RX_GPIO_CLK_ENABLE();
    DEBUG_USART_TX_GPIO_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = DEBUG_USART_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = DEBUG_USART_TX_AF;
    HAL_GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStruct);
    
    /* ����Rx����Ϊ���ù��� */
    GPIO_InitStruct.Pin = DEBUG_USART_RX_PIN;
    GPIO_InitStruct.Alternate = DEBUG_USART_RX_AF;
    HAL_GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStruct); 
    
    HAL_NVIC_SetPriority(DEBUG_USART_IRQ ,0,1);	//��ռ���ȼ�0�������ȼ�1
    HAL_NVIC_EnableIRQ(DEBUG_USART_IRQ );		    //ʹ��USART1�ж�ͨ��  

    Debug_Uart_Config();
}


/*****************  �����ַ��� **********************/
void Debug_Uart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&DebugUartHandler,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
  
}


UartFsm_Handler_t DebugUartReveiveData = {0,E_UartFsm_NoInit,0,};

uint8_t DebugUartReceiveBuffer[IAP_BUFFERSIZE];

#if 1
void  DEBUG_USART_IRQHandler(void)
{
  uint8_t ch=0; 
  
	if(__HAL_UART_GET_FLAG( &DebugUartHandler, UART_FLAG_RXNE ) != RESET)
	{		
        ch=( uint16_t)READ_REG(DebugUartHandler.Instance->DR);
        //WRITE_REG(DebugUartHandler.Instance->DR,ch); 

        /* ������ʱ�� */
        multi_timer_start(&Timer_DebugUart_Handler,2000,(multi_timer_callback_t)MultiTimer_DebugUart_CallBack,NULL);
        DebugUartReceiveBuffer[DebugUartReveiveData.len++] = ch;

//		HAL_UART_Transmit( &UartHandle,&ch ,1,1 );    
	}
//  HAL_UART_IRQHandler(&UartHandle);	
}
#else
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uint8_t recv_buf = 0;
    
    if(huart->Instance == USART1)
    {
        HAL_UART_Receive_IT(&DebugUartHandler,(uint8_t *)&recv_buf,1);
        shellHandler(&shell, recv_buf);
    }
}

#endif

///�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ�����DEBUG_USART */
	HAL_UART_Transmit(&DebugUartHandler, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}

///�ض���c�⺯��scanf������DEBUG_USART����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		
	int ch;
	HAL_UART_Receive(&DebugUartHandler, (uint8_t *)&ch, 1, 1000);	
	return (ch);
}



/*********************************************END OF FILE**********************/
