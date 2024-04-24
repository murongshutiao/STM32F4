#ifndef __BSP_INTERNALFLASH_H
#define	__BSP_INTERNALFLASH_H
#include "board.h"


/* Base address of the Flash sectors */ 
#define ADDR_FLASH_SECTOR_0         ((uint32_t)0x08000000) /* Base address of Sector 0, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_1         ((uint32_t)0x08004000) /* Base address of Sector 1, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_2         ((uint32_t)0x08008000) /* Base address of Sector 2, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_3         ((uint32_t)0x0800C000) /* Base address of Sector 3, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_4         ((uint32_t)0x08010000) /* Base address of Sector 4, 64 Kbytes   */
#define ADDR_FLASH_SECTOR_5         ((uint32_t)0x08020000) /* Base address of Sector 5, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_6         ((uint32_t)0x08040000) /* Base address of Sector 6, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_7         ((uint32_t)0x08060000) /* Base address of Sector 7, 128 Kbytes  */

/* F429,8到11扇区可以被细分,及扇区可有8-11变为12-19扇区 */
#define ADDR_FLASH_SECTOR_8         ((uint32_t)0x08080000) /* Base address of Sector 7, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_9         ((uint32_t)0x080A0000) /* Base address of Sector 7, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_10        ((uint32_t)0x080C0000) /* Base address of Sector 7, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_11        ((uint32_t)0x080E0000) /* Base address of Sector 7, 128 Kbytes  */

int InternalFlash_Test(void);

int8_t InternalFlashEraseSector(uint32_t EraseStartAddr,uint32_t EraseEndAddr);
int8_t InternalFlash_EraseWriteData(uint32_t WriteStartAddr,uint32_t *pWriteData,uint32_t Num);
int8_t InternalFlash_WriteData(uint32_t WriteStartAddr,uint32_t *pWriteData,uint32_t Num);
void InternalFlash_ReadData(uint32_t ReadStartAddr,uint32_t *pReadData,uint32_t Num);
void InternalFlash_Test2(void);


#endif /* __INTERNAL_FLASH_H */

