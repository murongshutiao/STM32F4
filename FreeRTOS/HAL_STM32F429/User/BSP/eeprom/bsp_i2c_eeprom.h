#ifndef __BSP_I2C_EEPROM_H
#define	__BSP_I2C_EEPROM_H
#include "board.h"

#define  DATA_Size			256
#define  EEP_Firstpage      0x00


#define EEPROM_PAGESIZE 	   8    /* AT24C01/02每页有8个字节 */
//#define EEPROM_PAGESIZE      16   /* AT24C04/08A/16A每页有16个字节 */	

#define I2C_OWN_ADDRESS7      0X0A   /* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */


/*等待超时时间*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))
#define I2Cx_TIMEOUT_MAX                300
#define EEPROM_MAX_TRIALS               300

/*信息输出*/
#define EEPROM_DEBUG_ON         1

#define EEPROM_INFO           Debug_Uart_printf

// #define EEPROM_DEBUG(fmt,arg...)          do{\
//                                           if(EEPROM_DEBUG_ON)\
//                                           Debug_Uart_printf("<<-EEPROM-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
//                                           }while(0)

/* 
 * AT24C02 2kb = 2048bit = 2048/8 B = 256 B
 * 32 pages of 8 bytes each
 */

/* EEPROM Addresses defines */
#define EEPROM_Block0_ADDRESS 0xA0   /* E2 = 0 */
#define EEPROM_Block1_ADDRESS 0xA2 /* E2 = 0 */
#define EEPROM_Block2_ADDRESS 0xA4 /* E2 = 0 */
#define EEPROM_Block3_ADDRESS 0xA6 /* E2 = 0 */

#define EEPROM_ADDRESS        0xA0

extern I2C_HandleTypeDef  EEPROM_I2C_Handler; 


void EEPROM_I2C_Init(void);										  
uint32_t EEPROM_WriteByte( uint8_t WriteAddr,uint8_t* pBuffer);
uint32_t EEPROM_WritePage(uint8_t WriteAddr,uint8_t* pBuffer, uint8_t NumByteToWrite);
void EEPROM_WriteBuffer(uint8_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite);
uint32_t EEPROM_ReadBuffer( uint8_t ReadAddr,uint8_t* pBuffer, uint16_t NumByteToRead);
void EEPROM_I2C_Test(void);

#endif /* __I2C_EE_H */
