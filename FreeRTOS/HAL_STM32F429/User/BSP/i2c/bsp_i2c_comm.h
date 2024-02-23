#ifndef __BSP_I2C_COMM_H
#define __BSP_I2C_COMM_H
#include "board.h"

#define MASTER_I2C_ADDRESS  0x20


void I2C_Master_Comm_Init(void);
void I2C_Master_Test(void);
void I2C_Slave_Comm_Init(void);
void I2C_Slave_Test(void);




#endif
