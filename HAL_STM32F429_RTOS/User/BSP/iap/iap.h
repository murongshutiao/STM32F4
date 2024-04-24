#ifndef __IAP_H
#define __IAP_H
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./internalflash/bsp_internalflash.h"

/**
 * 整体设计如下
 * 开机5秒内单击K2进入引导模式，双击可选择版本运行APP,否则按照最新版本运行。
*/

// #define STM32F4_Bootloader_Addr 0x1FFF0000  /* 系统bootloader地址 */
// #define STM32F4_Bootloader_RstAddr  (*((uint32_t *)(STM32F4_Bootloader_Addr + 4)))

// typedef  void (*pFunction)(void);

#define IAP_BUFFERSIZE  (128)// * 1024)    /* 128kBytes,一个flash扇区 */ 
#define IAP_WriteBufSize    (512)       /* (512 * 4) Bytes的写入flash缓存 */  

#define APP_ADDR1   ADDR_FLASH_SECTOR_7
#define APP_ADDR2   ADDR_FLASH_SECTOR_7


extern uint32_t g_iapbuf[IAP_WriteBufSize];



void Jump_Dfu_BootLoader(void);
void NVIC_SetVectorTable(uint32_t Addr,uint32_t offset);








#endif
