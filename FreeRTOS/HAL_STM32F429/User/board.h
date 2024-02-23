#ifndef __BOARD_H
#define __BOARD_H

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/******************* Common File *******************/
#include "./Common/common.h"
#include "./Common/fonts/fonts.h"

#include "./rcc/bsp_rcc.h"
#include "./led/bsp_led.h"
#include "./button/bsp_button.h"
#include "./debug/bsp_uart_debug.h"
#include "./uart/bsp_uart_modbus.h"
#include "./i2c/bsp_i2c_comm.h"
#include "./flash/bsp_spi_flash.h"
#include "./sdram/bsp_fmc_sdram.h"
#include "./adc/bsp_adc.h"
#include "./tim/bsp_timer.h"
#include "./sdio/bsp_sdio_sd.h"
#include "./sdio/sdio_test.h"
#include "./can/bsp_can_comm.h"
#include "./internalflash/bsp_internalflash.h"
#include "./iap/iap.h"

/******************* Module File *******************/
#include "./Module/FATFS/ff.h"
#include "./Module/FATFS/diskio.h"
#include "./Module/MultiTimer/multi_timer.h"
#include "./Module/MultiTimer/multi_timer_user.h"
#include "./Module/MultiButton/multi_button.h"
#include "./Module/MultiButton/multi_button_user.h"
#include "./Module/letter_shell/port/shell_port.h"
#include "./Module/lwip/arch/ethernetif/ethernetif.h"
            
/******************* Sensor File *******************/
#include "./Sensor/mpu6050/bsp_i2c_mpu6050.h"

extern const uint32_t version;


/**
 *  时钟 
 * 源:HSE
 * SYSCLK = 168M
 * AHB = 168M
 * APB2 = 84M
 * APB1 = 42M
 * */
/**************** MCO引腳固定為A8和C9 ****************/





//引脚定义
/*******************************************************/
#define Button1_PIN                  GPIO_PIN_0                 
#define Button1_GPIO_PORT            GPIOA                      
#define Button1_GPIO_CLK_ENABLE()    __GPIOA_CLK_ENABLE()

#define Button2_PIN                  GPIO_PIN_13                 
#define Button2_GPIO_PORT            GPIOC                      
#define Button2_GPIO_CLK_ENABLE()    __GPIOC_CLK_ENABLE()
/*******************************************************/

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

//引脚定义
/*******************************************************/
#define MBMaster_USART                             USART2
#define MBMaster_USART_CLK_ENABLE()                __USART2_CLK_ENABLE();

#define MBMaster_USART_RX_GPIO_PORT                GPIOD
#define MBMaster_USART_RX_PIN                      GPIO_PIN_5
#define MBMaster_USART_RX_GPIO_CLK_ENABLE()        __GPIOD_CLK_ENABLE()
#define MBMaster_USART_RX_AF                       GPIO_AF7_USART2

#define MBMaster_USART_TX_GPIO_PORT                GPIOD
#define MBMaster_USART_TX_PIN                      GPIO_PIN_6
#define MBMaster_USART_TX_GPIO_CLK_ENABLE()        __GPIOD_CLK_ENABLE()
#define MBMaster_USART_TX_AF                       GPIO_AF7_USART2

#define MBMaster_USART_IRQHandler                  USART2_IRQHandler
#define MBMaster_USART_IRQ                 		    USART2_IRQn
/************************************************************/

//引脚定义
/*******************************************************/
#define MBSlave_USART                             USART3
#define MBSlave_USART_CLK_ENABLE()                __USART3_CLK_ENABLE()

#define MBSlave_USART_RX_GPIO_PORT                GPIOB
#define MBSlave_USART_RX_PIN                      GPIO_PIN_11
#define MBSlave_USART_RX_GPIO_CLK_ENABLE()        __GPIOB_CLK_ENABLE()
#define MBSlave_USART_RX_AF                       GPIO_AF7_USART2

#define MBSlave_USART_TX_GPIO_PORT                GPIOB
#define MBSlave_USART_TX_PIN                      GPIO_PIN_10
#define MBSlave_USART_TX_GPIO_CLK_ENABLE()        __GPIOB_CLK_ENABLE()
#define MBSlave_USART_TX_AF                       GPIO_AF7_USART3

#define MBSlave_USART_IRQHandler                  USART3_IRQHandler
#define MBSlave_USART_IRQ                 		    USART3_IRQn



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


/* I2C设备 */
#define EEPROM_I2C                             I2C1
#define EEPROM_I2C_CLK_ENABLE()                __HAL_RCC_I2C1_CLK_ENABLE()

/* Definition for I2Cx Pins */
#define EEPROM_I2C_SCL_GPIO_PORT              GPIOB
#define EEPROM_I2C_SCL_PIN                    GPIO_PIN_6
#define EEPROM_I2C_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define EEPROM_I2C_SCL_AF                     GPIO_AF4_I2C1

#define EEPROM_I2C_SDA_GPIO_PORT              GPIOB
#define EEPROM_I2C_SDA_PIN                    GPIO_PIN_7
#define EEPROM_I2C_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE() 
#define EEPROM_I2C_SDA_AF                     GPIO_AF4_I2C1

#define EEPROM_I2C_FORCE_RESET()               __HAL_RCC_I2C1_FORCE_RESET()
#define EEPROM_I2C_RELEASE_RESET()             __HAL_RCC_I2C1_RELEASE_RESET()

/* I2C主机 */
#define Master_I2C                             I2C2
#define Master_I2C_CLK_ENABLE()                __HAL_RCC_I2C2_CLK_ENABLE()

/* Definition for I2Cx Pins */
#define Master_I2C_SCL_GPIO_PORT              GPIOH
#define Master_I2C_SCL_PIN                    GPIO_PIN_4
#define Master_I2C_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOH_CLK_ENABLE()
#define Master_I2C_SCL_AF                     GPIO_AF4_I2C2

#define Master_I2C_SDA_GPIO_PORT              GPIOH
#define Master_I2C_SDA_PIN                    GPIO_PIN_5
#define Master_I2C_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOH_CLK_ENABLE() 
#define Master_I2C_SDA_AF                     GPIO_AF4_I2C2

#define Master_I2C_FORCE_RESET()               __HAL_RCC_I2C2_FORCE_RESET()
#define Master_I2C_RELEASE_RESET()             __HAL_RCC_I2C2_RELEASE_RESET()

/* I2C从机 */
#define Slave_I2C                             I2C2
#define Slave_I2C_CLK_ENABLE()                __HAL_RCC_I2C2_CLK_ENABLE()

/* Definition for I2Cx Pins */
#define Slave_I2C_SCL_GPIO_PORT              GPIOH
#define Slave_I2C_SCL_PIN                    GPIO_PIN_4
#define Slave_I2C_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOH_CLK_ENABLE()
#define Slave_I2C_SCL_AF                     GPIO_AF4_I2C2

#define Slave_I2C_SDA_GPIO_PORT              GPIOH
#define Slave_I2C_SDA_PIN                    GPIO_PIN_5
#define Slave_I2C_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOH_CLK_ENABLE() 
#define Slave_I2C_SDA_AF                     GPIO_AF4_I2C2

#define Slave_I2C_FORCE_RESET()               __HAL_RCC_I2C2_FORCE_RESET()
#define Slave_I2C_RELEASE_RESET()             __HAL_RCC_I2C2_RELEASE_RESET()

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

/* FMC接口外设,256Mbit,
    Bank1:0xC0000000 ~ 0xCFFFFFFF   :256MB
    Bank2:0xD0000000 ~ 0xDFFFFFFF   :256MB
    */
/*地址信号线*/  
#define FMC_A0_GPIO_PORT        GPIOF
#define FMC_A0_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_A0_GPIO_PIN         GPIO_PIN_0
#define FMC_A0_PINSOURCE        GPIO_PinSource0
#define FMC_A0_AF               GPIO_AF_FMC

#define FMC_A1_GPIO_PORT        GPIOF
#define FMC_A1_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_A1_GPIO_PIN         GPIO_PIN_1
#define FMC_A1_PINSOURCE        GPIO_PinSource1
#define FMC_A1_AF               GPIO_AF_FMC

#define FMC_A2_GPIO_PORT        GPIOF
#define FMC_A2_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_A2_GPIO_PIN         GPIO_PIN_2
#define FMC_A2_PINSOURCE        GPIO_PinSource2
#define FMC_A2_AF               GPIO_AF_FMC

#define FMC_A3_GPIO_PORT        GPIOF
#define FMC_A3_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_A3_GPIO_PIN         GPIO_PIN_3
#define FMC_A3_PINSOURCE        GPIO_PinSource3
#define FMC_A3_AF               GPIO_AF_FMC

#define FMC_A4_GPIO_PORT        GPIOF
#define FMC_A4_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_A4_GPIO_PIN         GPIO_PIN_4
#define FMC_A4_PINSOURCE        GPIO_PinSource4
#define FMC_A4_AF               GPIO_AF_FMC

#define FMC_A5_GPIO_PORT        GPIOF
#define FMC_A5_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_A5_GPIO_PIN         GPIO_PIN_5
#define FMC_A5_PINSOURCE        GPIO_PinSource5
#define FMC_A5_AF               GPIO_AF_FMC

#define FMC_A6_GPIO_PORT        GPIOF
#define FMC_A6_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_A6_GPIO_PIN         GPIO_PIN_12
#define FMC_A6_PINSOURCE        GPIO_PinSource12
#define FMC_A6_AF               GPIO_AF_FMC

#define FMC_A7_GPIO_PORT        GPIOF
#define FMC_A7_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_A7_GPIO_PIN         GPIO_PIN_13
#define FMC_A7_PINSOURCE        GPIO_PinSource13
#define FMC_A7_AF               GPIO_AF_FMC

#define FMC_A8_GPIO_PORT        GPIOF
#define FMC_A8_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_A8_GPIO_PIN         GPIO_PIN_14
#define FMC_A8_PINSOURCE        GPIO_PinSource14
#define FMC_A8_AF               GPIO_AF_FMC

#define FMC_A9_GPIO_PORT        GPIOF
#define FMC_A9_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_A9_GPIO_PIN         GPIO_PIN_15
#define FMC_A9_PINSOURCE        GPIO_PinSource15
#define FMC_A9_AF               GPIO_AF_FMC


#define FMC_A10_GPIO_PORT        GPIOG
#define FMC_A10_GPIO_CLK         __HAL_RCC_GPIOG_CLK_ENABLE()
#define FMC_A10_GPIO_PIN         GPIO_PIN_0
#define FMC_A10_PINSOURCE        GPIO_PinSource0
#define FMC_A10_AF               GPIO_AF_FMC


#define FMC_A11_GPIO_PORT        GPIOG
#define FMC_A11_GPIO_CLK         __HAL_RCC_GPIOG_CLK_ENABLE()
#define FMC_A11_GPIO_PIN         GPIO_PIN_1
#define FMC_A11_PINSOURCE        GPIO_PinSource1
#define FMC_A11_AF               GPIO_AF_FMC

/*数据信号线*/
#define FMC_D0_GPIO_PORT        GPIOD
#define FMC_D0_GPIO_CLK         __HAL_RCC_GPIOD_CLK_ENABLE()
#define FMC_D0_GPIO_PIN         GPIO_PIN_14
#define FMC_D0_PINSOURCE        GPIO_PinSource14
#define FMC_D0_AF               GPIO_AF_FMC

#define FMC_D1_GPIO_PORT        GPIOD
#define FMC_D1_GPIO_CLK         __HAL_RCC_GPIOD_CLK_ENABLE()
#define FMC_D1_GPIO_PIN         GPIO_PIN_15
#define FMC_D1_PINSOURCE        GPIO_PinSource15
#define FMC_D1_AF               GPIO_AF_FMC

#define FMC_D2_GPIO_PORT        GPIOD
#define FMC_D2_GPIO_CLK         __HAL_RCC_GPIOD_CLK_ENABLE()
#define FMC_D2_GPIO_PIN         GPIO_PIN_0
#define FMC_D2_PINSOURCE        GPIO_PinSource0
#define FMC_D2_AF               GPIO_AF_FMC

#define FMC_D3_GPIO_PORT        GPIOD
#define FMC_D3_GPIO_CLK         __HAL_RCC_GPIOD_CLK_ENABLE()
#define FMC_D3_GPIO_PIN         GPIO_PIN_1
#define FMC_D3_PINSOURCE        GPIO_PinSource1
#define FMC_D3_AF               GPIO_AF_FMC

#define FMC_D4_GPIO_PORT        GPIOE
#define FMC_D4_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_D4_GPIO_PIN         GPIO_PIN_7
#define FMC_D4_PINSOURCE        GPIO_PinSource7
#define FMC_D4_AF               GPIO_AF_FMC

#define FMC_D5_GPIO_PORT        GPIOE
#define FMC_D5_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_D5_GPIO_PIN         GPIO_PIN_8
#define FMC_D5_PINSOURCE        GPIO_PinSource8
#define FMC_D5_AF               GPIO_AF_FMC

#define FMC_D6_GPIO_PORT        GPIOE
#define FMC_D6_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_D6_GPIO_PIN         GPIO_PIN_9
#define FMC_D6_PINSOURCE        GPIO_PinSource9
#define FMC_D6_AF               GPIO_AF_FMC

#define FMC_D7_GPIO_PORT        GPIOE
#define FMC_D7_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_D7_GPIO_PIN         GPIO_PIN_10
#define FMC_D7_PINSOURCE        GPIO_PinSource10
#define FMC_D7_AF               GPIO_AF_FMC

#define FMC_D8_GPIO_PORT        GPIOE
#define FMC_D8_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_D8_GPIO_PIN         GPIO_PIN_11
#define FMC_D8_PINSOURCE        GPIO_PinSource11
#define FMC_D8_AF               GPIO_AF_FMC

#define FMC_D9_GPIO_PORT        GPIOE
#define FMC_D9_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_D9_GPIO_PIN         GPIO_PIN_12
#define FMC_D9_PINSOURCE        GPIO_PinSource12
#define FMC_D9_AF               GPIO_AF_FMC

#define FMC_D10_GPIO_PORT        GPIOE
#define FMC_D10_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_D10_GPIO_PIN         GPIO_PIN_13
#define FMC_D10_PINSOURCE        GPIO_PinSource13
#define FMC_D10_AF               GPIO_AF_FMC

#define FMC_D11_GPIO_PORT        GPIOE
#define FMC_D11_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_D11_GPIO_PIN         GPIO_PIN_14
#define FMC_D11_PINSOURCE        GPIO_PinSource14
#define FMC_D11_AF               GPIO_AF_FMC

#define FMC_D12_GPIO_PORT        GPIOE
#define FMC_D12_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_D12_GPIO_PIN         GPIO_PIN_15
#define FMC_D12_PINSOURCE        GPIO_PinSource15
#define FMC_D12_AF               GPIO_AF_FMC

#define FMC_D13_GPIO_PORT        GPIOD
#define FMC_D13_GPIO_CLK         __HAL_RCC_GPIOD_CLK_ENABLE()
#define FMC_D13_GPIO_PIN         GPIO_PIN_8
#define FMC_D13_PINSOURCE        GPIO_PinSource8
#define FMC_D13_AF               GPIO_AF_FMC

#define FMC_D14_GPIO_PORT        GPIOD
#define FMC_D14_GPIO_CLK         __HAL_RCC_GPIOD_CLK_ENABLE()
#define FMC_D14_GPIO_PIN         GPIO_PIN_9
#define FMC_D14_PINSOURCE        GPIO_PinSource9
#define FMC_D14_AF               GPIO_AF_FMC

#define FMC_D15_GPIO_PORT        GPIOD
#define FMC_D15_GPIO_CLK         __HAL_RCC_GPIOD_CLK_ENABLE()
#define FMC_D15_GPIO_PIN         GPIO_PIN_10
#define FMC_D15_PINSOURCE        GPIO_PinSource10
#define FMC_D15_AF               GPIO_AF_FMC


/*控制信号线*/  
#define FMC_CS_GPIO_PORT        GPIOH
#define FMC_CS_GPIO_CLK         __HAL_RCC_GPIOH_CLK_ENABLE()
#define FMC_CS_GPIO_PIN         GPIO_PIN_6
#define FMC_CS_PINSOURCE        GPIO_PinSource6
#define FMC_CS_AF               GPIO_AF_FMC

#define FMC_BA0_GPIO_PORT        GPIOG
#define FMC_BA0_GPIO_CLK         __HAL_RCC_GPIOG_CLK_ENABLE()
#define FMC_BA0_GPIO_PIN         GPIO_PIN_4
#define FMC_BA0_PINSOURCE        GPIO_PinSource4
#define FMC_BA0_AF               GPIO_AF_FMC

#define FMC_BA1_GPIO_PORT        GPIOG
#define FMC_BA1_GPIO_CLK         __HAL_RCC_GPIOG_CLK_ENABLE()
#define FMC_BA1_GPIO_PIN         GPIO_PIN_5
#define FMC_BA1_PINSOURCE        GPIO_PinSource5
#define FMC_BA1_AF               GPIO_AF_FMC

#define FMC_WE_GPIO_PORT        GPIOC
#define FMC_WE_GPIO_CLK         __HAL_RCC_GPIOC_CLK_ENABLE()
#define FMC_WE_GPIO_PIN         GPIO_PIN_0
#define FMC_WE_PINSOURCE        GPIO_PinSource0
#define FMC_WE_AF               GPIO_AF_FMC

#define FMC_RAS_GPIO_PORT        GPIOF
#define FMC_RAS_GPIO_CLK         __HAL_RCC_GPIOF_CLK_ENABLE()
#define FMC_RAS_GPIO_PIN         GPIO_PIN_11
#define FMC_RAS_PINSOURCE        GPIO_PinSource11
#define FMC_RAS_AF               GPIO_AF_FMC

#define FMC_CAS_GPIO_PORT        GPIOG
#define FMC_CAS_GPIO_CLK         __HAL_RCC_GPIOG_CLK_ENABLE()
#define FMC_CAS_GPIO_PIN         GPIO_PIN_15
#define FMC_CAS_PINSOURCE        GPIO_PinSource15
#define FMC_CAS_AF               GPIO_AF_FMC

#define FMC_CLK_GPIO_PORT        GPIOG
#define FMC_CLK_GPIO_CLK         __HAL_RCC_GPIOG_CLK_ENABLE()
#define FMC_CLK_GPIO_PIN         GPIO_PIN_8
#define FMC_CLK_PINSOURCE        GPIO_PinSource8
#define FMC_CLK_AF               GPIO_AF_FMC

#define FMC_CKE_GPIO_PORT        GPIOH
#define FMC_CKE_GPIO_CLK         __HAL_RCC_GPIOH_CLK_ENABLE()
#define FMC_CKE_GPIO_PIN         GPIO_PIN_7
#define FMC_CKE_PINSOURCE        GPIO_PinSource7
#define FMC_CKE_AF               GPIO_AF_FMC

/*UDQM LDQM*/
#define FMC_UDQM_GPIO_PORT        GPIOE
#define FMC_UDQM_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_UDQM_GPIO_PIN         GPIO_PIN_1
#define FMC_UDQM_PINSOURCE        GPIO_PinSource1
#define FMC_UDQM_AF               GPIO_AF_FMC

#define FMC_LDQM_GPIO_PORT        GPIOE
#define FMC_LDQM_GPIO_CLK         __HAL_RCC_GPIOE_CLK_ENABLE()
#define FMC_LDQM_GPIO_PIN         GPIO_PIN_0
#define FMC_LDQM_PINSOURCE        GPIO_PinSource0
#define FMC_LDQM_AF               GPIO_AF_FMC

/*********************** RGB888 LCD相关 ***********************/
//红色数据线
#define LTDC_R0_GPIO_PORT        	GPIOH
#define LTDC_R0_GPIO_CLK_ENABLE()   __GPIOH_CLK_ENABLE()
#define LTDC_R0_GPIO_PIN         	GPIO_PIN_2
#define LTDC_R0_AF			        GPIO_AF14_LTDC

#define LTDC_R1_GPIO_PORT        	GPIOH
#define LTDC_R1_GPIO_CLK_ENABLE()   __GPIOH_CLK_ENABLE()
#define LTDC_R1_GPIO_PIN         	GPIO_PIN_3
#define LTDC_R1_AF			        GPIO_AF14_LTDC

#define LTDC_R2_GPIO_PORT        	GPIOH
#define LTDC_R2_GPIO_CLK_ENABLE()	__GPIOH_CLK_ENABLE()
#define LTDC_R2_GPIO_PIN         	GPIO_PIN_8
#define LTDC_R2_AF			        GPIO_AF14_LTDC

#define LTDC_R3_GPIO_PORT        	GPIOB
#define LTDC_R3_GPIO_CLK_ENABLE()	__GPIOB_CLK_ENABLE()
#define LTDC_R3_GPIO_PIN         	GPIO_PIN_0
#define LTDC_R3_AF			        GPIO_AF9_LTDC

#define LTDC_R4_GPIO_PORT        	GPIOA
#define LTDC_R4_GPIO_CLK_ENABLE()	__GPIOA_CLK_ENABLE()
#define LTDC_R4_GPIO_PIN         	GPIO_PIN_11
#define LTDC_R4_AF			        GPIO_AF14_LTDC

#define LTDC_R5_GPIO_PORT        	GPIOA
#define LTDC_R5_GPIO_CLK_ENABLE()	__GPIOA_CLK_ENABLE()
#define LTDC_R5_GPIO_PIN         	GPIO_PIN_12
#define LTDC_R5_AF			        GPIO_AF14_LTDC

#define LTDC_R6_GPIO_PORT        	GPIOB
#define LTDC_R6_GPIO_CLK_ENABLE()	__GPIOB_CLK_ENABLE()
#define LTDC_R6_GPIO_PIN         	GPIO_PIN_1
#define LTDC_R6_AF			        GPIO_AF9_LTDC

#define LTDC_R7_GPIO_PORT        	GPIOG
#define LTDC_R7_GPIO_CLK_ENABLE()	__GPIOG_CLK_ENABLE()
#define LTDC_R7_GPIO_PIN         	GPIO_PIN_6
#define LTDC_R7_AF			        GPIO_AF14_LTDC
//绿色数据线
#define LTDC_G0_GPIO_PORT        	GPIOE
#define LTDC_G0_GPIO_CLK_ENABLE()	__GPIOE_CLK_ENABLE()
#define LTDC_G0_GPIO_PIN         	GPIO_PIN_5
#define LTDC_G0_AF			        GPIO_AF14_LTDC

#define LTDC_G1_GPIO_PORT        	GPIOE
#define LTDC_G1_GPIO_CLK_ENABLE()	__GPIOE_CLK_ENABLE()
#define LTDC_G1_GPIO_PIN         	GPIO_PIN_6
#define LTDC_G1_AF			        GPIO_AF14_LTDC

#define LTDC_G2_GPIO_PORT        	GPIOH
#define LTDC_G2_GPIO_CLK_ENABLE() 	__GPIOH_CLK_ENABLE()
#define LTDC_G2_GPIO_PIN         	GPIO_PIN_13
#define LTDC_G2_AF			        GPIO_AF14_LTDC

#define LTDC_G3_GPIO_PORT        	GPIOG
#define LTDC_G3_GPIO_CLK_ENABLE()	__GPIOG_CLK_ENABLE()
#define LTDC_G3_GPIO_PIN         	GPIO_PIN_10
#define LTDC_G3_AF			        GPIO_AF9_LTDC

#define LTDC_G4_GPIO_PORT        	GPIOH
#define LTDC_G4_GPIO_CLK_ENABLE()	__GPIOH_CLK_ENABLE()
#define LTDC_G4_GPIO_PIN         	GPIO_PIN_15
#define LTDC_G4_AF			        GPIO_AF14_LTDC

#define LTDC_G5_GPIO_PORT        	GPIOI
#define LTDC_G5_GPIO_CLK_ENABLE()	__GPIOI_CLK_ENABLE()
#define LTDC_G5_GPIO_PIN         	GPIO_PIN_0
#define LTDC_G5_AF			        GPIO_AF14_LTDC

#define LTDC_G6_GPIO_PORT        	GPIOC
#define LTDC_G6_GPIO_CLK_ENABLE()	__GPIOC_CLK_ENABLE()
#define LTDC_G6_GPIO_PIN         	GPIO_PIN_7
#define LTDC_G6_AF			        GPIO_AF14_LTDC

#define LTDC_G7_GPIO_PORT        	GPIOI
#define LTDC_G7_GPIO_CLK_ENABLE() 	__GPIOI_CLK_ENABLE()
#define LTDC_G7_GPIO_PIN         	GPIO_PIN_2
#define LTDC_G7_AF			        GPIO_AF14_LTDC

//蓝色数据线
#define LTDC_B0_GPIO_PORT        	GPIOE
#define LTDC_B0_GPIO_CLK_ENABLE()  	__GPIOE_CLK_ENABLE()
#define LTDC_B0_GPIO_PIN         	GPIO_PIN_4
#define LTDC_B0_AF			        GPIO_AF14_LTDC

#define LTDC_B1_GPIO_PORT        	GPIOG
#define LTDC_B1_GPIO_CLK_ENABLE() 	__GPIOG_CLK_ENABLE()
#define LTDC_B1_GPIO_PIN         	GPIO_PIN_12
#define LTDC_B1_AF			        GPIO_AF14_LTDC

#define LTDC_B2_GPIO_PORT        	GPIOD
#define LTDC_B2_GPIO_CLK_ENABLE()  	__GPIOD_CLK_ENABLE()
#define LTDC_B2_GPIO_PIN         	GPIO_PIN_6
#define LTDC_B2_AF			        GPIO_AF14_LTDC

#define LTDC_B3_GPIO_PORT        	GPIOG
#define LTDC_B3_GPIO_CLK_ENABLE() 	__GPIOD_CLK_ENABLE()
#define LTDC_B3_GPIO_PIN         	GPIO_PIN_11
#define LTDC_B3_AF			        GPIO_AF14_LTDC

#define LTDC_B4_GPIO_PORT        	GPIOI
#define LTDC_B4_GPIO_CLK_ENABLE() 	__GPIOI_CLK_ENABLE()
#define LTDC_B4_GPIO_PIN         	GPIO_PIN_4
#define LTDC_B4_AF			        GPIO_AF14_LTDC

#define LTDC_B5_GPIO_PORT        	GPIOA
#define LTDC_B5_GPIO_CLK_ENABLE()	__GPIOA_CLK_ENABLE()
#define LTDC_B5_GPIO_PIN         	GPIO_PIN_3
#define LTDC_B5_AF			        GPIO_AF14_LTDC

#define LTDC_B6_GPIO_PORT        	GPIOB
#define LTDC_B6_GPIO_CLK_ENABLE() 	__GPIOB_CLK_ENABLE()
#define LTDC_B6_GPIO_PIN         	GPIO_PIN_8
#define LTDC_B6_AF			        GPIO_AF14_LTDC

#define LTDC_B7_GPIO_PORT        	GPIOB
#define LTDC_B7_GPIO_CLK_ENABLE() 	__GPIOB_CLK_ENABLE()
#define LTDC_B7_GPIO_PIN         	GPIO_PIN_9
#define LTDC_B7_AF			        GPIO_AF14_LTDC

//控制信号线
/*像素时钟CLK*/
#define LTDC_CLK_GPIO_PORT              GPIOG
#define LTDC_CLK_GPIO_CLK_ENABLE()      __GPIOG_CLK_ENABLE()
#define LTDC_CLK_GPIO_PIN               GPIO_PIN_7
#define LTDC_CLK_AF			            GPIO_AF14_LTDC
/*水平同步信号HSYNC*/
#define LTDC_HSYNC_GPIO_PORT            GPIOI
#define LTDC_HSYNC_GPIO_CLK_ENABLE()    __GPIOI_CLK_ENABLE()
#define LTDC_HSYNC_GPIO_PIN             GPIO_PIN_10
#define LTDC_HSYNC_AF			        GPIO_AF14_LTDC
/*垂直同步信号VSYNC*/
#define LTDC_VSYNC_GPIO_PORT            GPIOI
#define LTDC_VSYNC_GPIO_CLK_ENABLE()    __GPIOI_CLK_ENABLE()
#define LTDC_VSYNC_GPIO_PIN             GPIO_PIN_9
#define LTDC_VSYNC_AF			        GPIO_AF14_LTDC

/*数据使能信号DE*/
#define LTDC_DE_GPIO_PORT               GPIOF
#define LTDC_DE_GPIO_CLK_ENABLE()       __GPIOF_CLK_ENABLE()
#define LTDC_DE_GPIO_PIN                GPIO_PIN_10
#define LTDC_DE_AF			            GPIO_AF14_LTDC
/*液晶屏使能信号DISP，高电平使能*/
#define LTDC_DISP_GPIO_PORT             GPIOD
#define LTDC_DISP_GPIO_CLK_ENABLE()     __GPIOD_CLK_ENABLE()
#define LTDC_DISP_GPIO_PIN              GPIO_PIN_4
/*液晶屏背光信号，高电平使能*/
#define LTDC_BL_GPIO_PORT               GPIOD
#define LTDC_BL_GPIO_CLK_ENABLE()       __GPIOD_CLK_ENABLE()
#define LTDC_BL_GPIO_PIN                GPIO_PIN_7


/* ADC */
// ADC GPIO 宏定义
#define RHEOSTAT_ADC_GPIO_PORT              GPIOC
#define RHEOSTAT_ADC_GPIO_PIN               GPIO_PIN_3
#define RHEOSTAT_ADC_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()
    
// ADC 序号宏定义
#define RHEOSTAT_ADC                        ADC1
#define RHEOSTAT_ADC_CLK_ENABLE()           __ADC1_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL                ADC_CHANNEL_13

// ADC 中断宏定义
#define Rheostat_ADC_IRQ                    ADC_IRQn
#define Rheostat_ADC_INT_FUNCTION           ADC_IRQHandler

/* CAN */
#define CANx                       CAN1
#define CAN_CLK_ENABLE()           __CAN1_CLK_ENABLE()
#define CAN_RX_IRQ				   CAN1_RX0_IRQn
#define CAN_RX_IRQHandler		   CAN1_RX0_IRQHandler

#define CAN_RX_PIN                 GPIO_PIN_8
#define CAN_TX_PIN                 GPIO_PIN_9
#define CAN_TX_GPIO_PORT           GPIOB
#define CAN_RX_GPIO_PORT           GPIOB
#define CAN_TX_GPIO_CLK_ENABLE()   __GPIOB_CLK_ENABLE()
#define CAN_RX_GPIO_CLK_ENABLE()   __GPIOB_CLK_ENABLE()
#define CAN_AF_PORT                GPIO_AF9_CAN1 



/********************* 文件系统 *********************/
/* 为每个设备定义一个物理编号 */


#define ATA			    0     // 预留SD卡使用
#define SPI_FLASH		1     // 外部SPI Flash
#define INTERNAL_FLASH		2     // 内部Flash
#define INTERNAL_RAM		3     // 内部Ram


void BSP_Init(void);
void App_Task(void);






#endif


