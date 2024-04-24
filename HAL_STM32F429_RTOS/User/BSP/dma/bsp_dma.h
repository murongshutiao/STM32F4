#ifndef __BSP_DMA_H
#define __BSP_DMA_H
#include "board.h"





void DMA_MemToMem_Config(void);
void DMA_MemToMem_Test(void);

//DMA
#define SENDBUFF_SIZE                     		10//发送的数据量


void Debug_Uart_Dma_Config(void);
void Debug_Uart_Dma_Test(void);


#endif

