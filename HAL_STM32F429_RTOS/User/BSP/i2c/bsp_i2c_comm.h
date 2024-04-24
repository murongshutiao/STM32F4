#ifndef __BSP_I2C_COMM_H
#define __BSP_I2C_COMM_H
#include "board.h"

#define MASTER_I2C_ADDRESS  0x20


/* I2CÖ÷»ú */
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

/* I2C´Ó»ú */
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


void I2C_Master_Comm_Init(void);
void I2C_Master_Test(void);
void I2C_Slave_Comm_Init(void);
void I2C_Slave_Test(void);




#endif
