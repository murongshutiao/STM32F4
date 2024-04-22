#include "./debug/bsp_uart_debug.h"

UART_HandleTypeDef DebugUartHandler;


 /**
  * @brief  DEBUG_USART GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
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
    
 /*使能串口接收断 */
  __HAL_UART_ENABLE_IT(&DebugUartHandler,UART_IT_RXNE);  
}


/**
  * @brief UART MSP 初始化 
  * @param huart: UART handle
  * @retval 无
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
    
    /* 配置Rx引脚为复用功能 */
    GPIO_InitStruct.Pin = DEBUG_USART_RX_PIN;
    GPIO_InitStruct.Alternate = DEBUG_USART_RX_AF;
    HAL_GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStruct); 
    
    HAL_NVIC_SetPriority(DEBUG_USART_IRQ ,0,1);	//抢占优先级0，子优先级1
    HAL_NVIC_EnableIRQ(DEBUG_USART_IRQ );		    //使能USART1中断通道  

    Debug_Uart_Config();
}


/*****************  发送字符串 **********************/
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

        /* 启动定时器 */
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

///重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口DEBUG_USART */
	HAL_UART_Transmit(&DebugUartHandler, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}

///重定向c库函数scanf到串口DEBUG_USART，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		
	int ch;
	HAL_UART_Receive(&DebugUartHandler, (uint8_t *)&ch, 1, 1000);	
	return (ch);
}



/*********************************************END OF FILE**********************/
