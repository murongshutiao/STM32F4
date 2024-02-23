#ifndef __COMMON_H
#define __COMMON_H
#include "board.h"

typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

typedef  void (*pFunction)(void);

typedef union
{
    uint32_t U32Data;
    uint8_t U8Data[4];
}U32ToU8_t;


void UARTx_PRINTF (UART_HandleTypeDef *xUart ,uint8_t * Data, ... );
TestStatus U8Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

uint32_t pU8ToU32(uint8_t *pBuffer);













#endif
