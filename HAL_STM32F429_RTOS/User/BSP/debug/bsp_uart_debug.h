#ifndef __BSP_UART_DEBUG_H
#define	__BSP_UART_DEBUG_H
#include "board.h"

#define IAP_BUFFERSIZE  (128)// * 1024) 

//串口波特
#define DEBUG_USART_BAUDRATE                    115200


//引脚定义
/*******************************************************/
#define DEBUG_USART                             USART1
#define DEBUG_USART_CLK_ENABLE()                __USART1_CLK_ENABLE();

// #define RCC_PERIPHCLK_UARTx                     RCC_PERIPHCLK_USART1
// #define RCC_UARTxCLKSOURCE_SYSCLK               RCC_USART1CLKSOURCE_SYSCLK

#define DEBUG_USART_RX_GPIO_PORT                GPIOA
#define DEBUG_USART_RX_PIN                      GPIO_PIN_10
#define DEBUG_USART_RX_GPIO_CLK_ENABLE()        __GPIOA_CLK_ENABLE()
#define DEBUG_USART_RX_AF                       GPIO_AF7_USART1

#define DEBUG_USART_TX_GPIO_PORT                GPIOA
#define DEBUG_USART_TX_PIN                      GPIO_PIN_9
#define DEBUG_USART_TX_GPIO_CLK_ENABLE()        __GPIOA_CLK_ENABLE()
#define DEBUG_USART_TX_AF                       GPIO_AF7_USART1

#define DEBUG_USART_IRQHandler                  USART1_IRQHandler
#define DEBUG_USART_IRQ                 		    USART1_IRQn
/************************************************************/


/**
 *  存储器到存储器对流和通道没有要求
 * 存储器到存储器只能使用单次传输模式，不能使用循环传输模式
 */
#define DMA_STREAM               DMA2_Stream1
#define DMA_CHANNEL              DMA_CHANNEL_2
#define DMA_STREAM_CLOCK()       __DMA2_CLK_ENABLE()

#define DMA_ADC_STREAM               DMA2_Stream0
#define DMA_ADC_CHANNEL              DMA_CHANNEL_0
#define DMA_ADC_STREAM_CLOCK()       __DMA2_CLK_ENABLE()


/* DMA - USART1_TX*/
#define Debug_Uart_DMA_STREAM            		DMA2_Stream7
#define Debug_Uart_DMA_CHANNEL           		DMA_CHANNEL_4
#define Debug_Uart_DMA_CLK_ENABLE()      		__DMA2_CLK_ENABLE()	

typedef enum
{
    E_UartFsm_NoInit = 0,
    E_UartFsm_Receiving = 1,
    E_UartFsm_Finish = 2,
    E_UartFsm_Error = 3,
}E_UartFsmStatus_t;


typedef struct
{
    uint32_t semaphore:1;   /* */
    uint32_t status:2;      /* 当前串口状态 */
    uint32_t len:29;        /* 当前数据长度 */
    uint32_t *pDataBuffer;
}UartFsm_Handler_t;


extern uint8_t DebugUartReceiveBuffer[IAP_BUFFERSIZE];
extern UartFsm_Handler_t DebugUartReveiveData;


void Debug_Uart_SendString(uint8_t *str);
void Debug_Uart_Init(void);
//int fputc(int ch, FILE *f);
extern UART_HandleTypeDef DebugUartHandler;


#define Debug_Uart_printf(Data, ... )   UARTx_PRINTF(&DebugUartHandler,Data,##__VA_ARGS__)
#endif /* __USART1_H */
