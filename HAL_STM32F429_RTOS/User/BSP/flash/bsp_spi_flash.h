#ifndef __BSP_SPI_FLASH_H
#define __BSP_SPI_FLASH_H
#include "board.h"

/* Private typedef -----------------------------------------------------------*/
//#define  sFLASH_ID                       0xEF3015     //W25X16
//#define  sFLASH_ID                       0xEF4015	    //W25Q16
//#define  sFLASH_ID                        0XEF4017     //W25Q64
#define  sFLASH_ID                       0XEF4018     //W25Q128
//#define  sFLASH_ID                       0XEF4019     //W25Q256


//#define SPI_FLASH_PageSize            4096
#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

/* Private define ------------------------------------------------------------*/
/*命令定义-开头*******************************/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		  0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	  0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F 
#define W25X_Enter4ByteMode		    0xB7
#define W25X_ReadStatusRegister3      0x15

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */
#define Dummy_Byte                0xFF
/*命令定义-结尾*******************************/





/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*信息输出*/
#define FLASH_DEBUG_ON         0

#if (FLASH_DEBUG_ON == 1)
    #define FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
    #define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
    #define FLASH_DEBUG(fmt,arg...)          do{\
                                            if(FLASH_DEBUG_ON)\
                                            printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                            }while(0)
#else
    #define FLASH_INFO(fmt,arg...)          (void)0
    #define FLASH_ERROR(fmt,arg...)         (void)0
    #define FLASH_DEBUG(fmt,arg...)         (void)0

#endif


/* SPI外设 */
#define Flash_SPI                             SPI5
#define Flash_SPI_CLK_ENABLE()                __HAL_RCC_SPI5_CLK_ENABLE()
#define Flash_SPI_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOF_CLK_ENABLE()
#define Flash_SPI_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOF_CLK_ENABLE() 
#define Flash_SPI_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOF_CLK_ENABLE() 
#define Flash_SPI_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOF_CLK_ENABLE() 

#define Flash_SPI_FORCE_RESET()               __HAL_RCC_SPI5_FORCE_RESET()
#define Flash_SPI_RELEASE_RESET()             __HAL_RCC_SPI5_RELEASE_RESET()

/* Definition for SPIx Pins */
#define Flash_SPI_SCK_PIN                     GPIO_PIN_7
#define Flash_SPI_SCK_GPIO_PORT               GPIOF
#define Flash_SPI_SCK_AF                      GPIO_AF5_SPI5
#define Flash_SPI_MISO_PIN                    GPIO_PIN_8
#define Flash_SPI_MISO_GPIO_PORT              GPIOF
#define Flash_SPI_MISO_AF                     GPIO_AF5_SPI5
#define Flash_SPI_MOSI_PIN                    GPIO_PIN_9
#define Flash_SPI_MOSI_GPIO_PORT              GPIOF
#define Flash_SPI_MOSI_AF                     GPIO_AF5_SPI5

#define FLASH_SPI_CS_PIN                     GPIO_PIN_6               
#define FLASH_SPI_CS_GPIO_PORT               GPIOF   


#define	digitalHi(p,i)			    {p->BSRR=i;}			  //设置为高电平		
#define digitalLo(p,i)			    {p->BSRR=(uint32_t)i << 16;}				//输出低电平
#define SPI_FLASH_CS_LOW()      digitalLo(FLASH_SPI_CS_GPIO_PORT,FLASH_SPI_CS_PIN )
#define SPI_FLASH_CS_HIGH()     digitalHi(FLASH_SPI_CS_GPIO_PORT,FLASH_SPI_CS_PIN )


void Flash_SPI_Init(void);
void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
uint32_t SPI_FLASH_ReadID(void);
uint32_t SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);
void SPI_FLASH_Mode_Init(void);


uint8_t SPI_FLASH_ReadByte(void);
uint8_t SPI_FLASH_SendByte(uint8_t byte);
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

void Flash_Spi_test(void);

#endif /* __SPI_FLASH_H */

