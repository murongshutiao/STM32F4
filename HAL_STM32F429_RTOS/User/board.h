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


