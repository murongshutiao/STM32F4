/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   使用串口1，重定向c库printf函数到usart端口，中断接收模式
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./uart/bsp_uart_modbus.h"

UART_HandleTypeDef MBMasterUartHandler;
UART_HandleTypeDef MBSlaveUartHandler;


 /**
  * @brief  MBMaster_USART GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */  
void MBMaster_Uart_Config(void)
{ 
  
  MBMasterUartHandler.Instance          = MBMaster_USART;
  
  MBMasterUartHandler.Init.BaudRate     = MBMaster_USART_BAUDRATE;
  MBMasterUartHandler.Init.WordLength   = UART_WORDLENGTH_8B;
  MBMasterUartHandler.Init.StopBits     = UART_STOPBITS_1;
  MBMasterUartHandler.Init.Parity       = UART_PARITY_NONE;
  MBMasterUartHandler.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  MBMasterUartHandler.Init.Mode         = UART_MODE_TX_RX;
  
  HAL_UART_Init(&MBMasterUartHandler);
    
 /*使能串口接收断 */
  __HAL_UART_ENABLE_IT(&MBMasterUartHandler,UART_IT_RXNE);  
}


 /**
  * @brief  MBSlave_USART GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */  
void MBSlave_Uart_Config(void)
{ 
  
  MBSlaveUartHandler.Instance          = MBSlave_USART;
  
  MBSlaveUartHandler.Init.BaudRate     = MBSlave_USART_BAUDRATE;
  MBSlaveUartHandler.Init.WordLength   = UART_WORDLENGTH_8B;
  MBSlaveUartHandler.Init.StopBits     = UART_STOPBITS_1;
  MBSlaveUartHandler.Init.Parity       = UART_PARITY_NONE;
  MBSlaveUartHandler.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  MBSlaveUartHandler.Init.Mode         = UART_MODE_TX_RX;
  
  HAL_UART_Init(&MBSlaveUartHandler);
    
 /*使能串口接收断 */
  __HAL_UART_ENABLE_IT(&MBSlaveUartHandler,UART_IT_RXNE);  
}


/**
  * @brief UART MSP 初始化 
  * @param huart: UART handle
  * @retval 无
  */
void MBMaster_Uart_Init(void)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  MBMaster_USART_CLK_ENABLE();
	
	MBMaster_USART_RX_GPIO_CLK_ENABLE();
  MBMaster_USART_TX_GPIO_CLK_ENABLE();
  
  /* 配置Tx引脚为复用功能  */
  GPIO_InitStruct.Pin = MBMaster_USART_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = MBMaster_USART_TX_AF;
  HAL_GPIO_Init(MBMaster_USART_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* 配置Rx引脚为复用功能 */
  GPIO_InitStruct.Pin = MBMaster_USART_RX_PIN;
  GPIO_InitStruct.Alternate = MBMaster_USART_RX_AF;
  HAL_GPIO_Init(MBMaster_USART_RX_GPIO_PORT, &GPIO_InitStruct); 
 
  HAL_NVIC_SetPriority(MBMaster_USART_IRQ ,0,1);	//抢占优先级0，子优先级1
  HAL_NVIC_EnableIRQ(MBMaster_USART_IRQ );		    //使能USART1中断通道  

  MBMaster_Uart_Config();
}

void MBSlave_Uart_Init(void)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  MBSlave_USART_CLK_ENABLE();
	
	MBSlave_USART_RX_GPIO_CLK_ENABLE();
  MBSlave_USART_TX_GPIO_CLK_ENABLE();
  
  /* 配置Tx引脚为复用功能  */
  GPIO_InitStruct.Pin = MBSlave_USART_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = MBSlave_USART_TX_AF;
  HAL_GPIO_Init(MBSlave_USART_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* 配置Rx引脚为复用功能 */
  GPIO_InitStruct.Pin = MBSlave_USART_RX_PIN;
  GPIO_InitStruct.Alternate = MBSlave_USART_RX_AF;
  HAL_GPIO_Init(MBSlave_USART_RX_GPIO_PORT, &GPIO_InitStruct); 
 
  HAL_NVIC_SetPriority(MBSlave_USART_IRQ ,0,1);	//抢占优先级0，子优先级1
  HAL_NVIC_EnableIRQ(MBSlave_USART_IRQ );		    //使能USART1中断通道  

  MBSlave_Uart_Config();
}

/*****************  发送字符串 **********************/
void MBMaster_Uart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&MBMasterUartHandler,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
  
}

/*****************  发送字符串 **********************/
void MBSlave_Uart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&MBSlaveUartHandler,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
  
}

/*********************************************END OF FILE**********************/
