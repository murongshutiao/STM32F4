#ifndef __IAP_H
#define __IAP_H
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./internalflash/bsp_internalflash.h"

/**
 * �����������
 * ����5���ڵ���K2��������ģʽ��˫����ѡ��汾����APP,���������°汾���С�
*/

// #define STM32F4_Bootloader_Addr 0x1FFF0000  /* ϵͳbootloader��ַ */
// #define STM32F4_Bootloader_RstAddr  (*((uint32_t *)(STM32F4_Bootloader_Addr + 4)))

// typedef  void (*pFunction)(void);

#define IAP_BUFFERSIZE  (128)// * 1024)    /* 128kBytes,һ��flash���� */ 
#define IAP_WriteBufSize    (512)       /* (512 * 4) Bytes��д��flash���� */  

#define APP_ADDR1   ADDR_FLASH_SECTOR_7
#define APP_ADDR2   ADDR_FLASH_SECTOR_7


extern uint32_t g_iapbuf[IAP_WriteBufSize];



void Jump_Dfu_BootLoader(void);
void NVIC_SetVectorTable(uint32_t Addr,uint32_t offset);








#endif
