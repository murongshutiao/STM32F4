#ifndef __BSP_UART_MODBUS_H
#define	__BSP_UART_MODBUS_H

#include "board.h"


//´®¿Ú²¨ÌØ
#define MBMaster_USART_BAUDRATE                    115200
#define MBSlave_USART_BAUDRATE                    115200





void MBMaster_Uart_Init(void);
void MBMaster_Uart_SendString(uint8_t *str);
void MBSlave_Uart_Init(void);
void MBSlave_Uart_SendString(uint8_t *str);

extern UART_HandleTypeDef MBMasterUartHandler;
extern UART_HandleTypeDef MBSlaveUartHandler;


#define MBMaster_Uart_printf(Data, ... )   UARTx_PRINTF(&MBMasterUartHandler,Data,##__VA_ARGS__)
#define MBSlave_Uart_printf(Data, ... )   UARTx_PRINTF(&MBSlaveUartHandler,Data,##__VA_ARGS__)
#endif /* __USART1_H */
