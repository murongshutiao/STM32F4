#ifndef    USER_APP
#define USER_APP
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbutils.h"
                                                  
/* -----------------------Master Defines --------- 0----------------------------*/
#define M_DISCRETE_INPUT_START                    0
#define M_DISCRETE_INPUT_NDISCRETES               16
#define M_COIL_START                              0
#define M_COIL_NCOILS                             64
#define M_REG_INPUT_START                         0
#define M_REG_INPUT_NREGS                         100
#define M_REG_HOLDING_START                       0
#define M_REG_HOLDING_NREGS                       100
/* master mode: holding register's all address */
#define          M_HD_RESERVE                     0
/* master mode: input register's all address */   
#define          M_IN_RESERVE                     0
/* master mode: coil's all address */             
#define          M_CO_RESERVE                     0
/* master mode: discrete's all address */         
#define          M_DI_RESERVE                     0


/* 测试功能参数 */
#define MB_SAMPLE_TEST_SLAVE_ADDR						1			//从机设备地址
#define MB_REG_START  											1			//发送数据起始位置
#define MB_SEND_REG_NUM    									4			//发送数据数量
#define MB_READ_REG_NUM    									1			//接收数据数量
#define WAITING_FOREVER											-1		//永久等待

/* 通过宏定义选择功能 */
#define MB_USER_HOLD 												1			//写多个保持寄存器
#define MB_USER_COILS 											2			//写多个线圈
#define MB_USER_INPUT_REG										3			//读输入寄存器

extern void test(char MB);												//测试程序


/* -----------------------Slave Defines -------------------------------------*/
#define S_DISCRETE_INPUT_START                    0		
#define S_DISCRETE_INPUT_NDISCRETES               16
#define S_COIL_START                              0
#define S_COIL_NCOILS                             64
#define S_REG_INPUT_START                         0
#define S_REG_INPUT_NREGS                         100
#define S_REG_HOLDING_START                       1
#define S_REG_HOLDING_NREGS                       100
/* salve mode: holding register's all address */
#define          S_HD_RESERVE                     0
/* salve mode: input register's all address */
#define          S_IN_RESERVE                     0
/* salve mode: coil's all address */
#define          S_CO_RESERVE                     0
/* salve mode: discrete's all address */
#define          S_DI_RESERVE                     0




#endif
