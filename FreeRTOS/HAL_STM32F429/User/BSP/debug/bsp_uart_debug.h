#ifndef __BSP_UART_DEBUG_H
#define	__BSP_UART_DEBUG_H
#include "board.h"

#define IAP_BUFFERSIZE  (128)// * 1024) 

//串口波特
#define DEBUG_USART_BAUDRATE                    115200

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
