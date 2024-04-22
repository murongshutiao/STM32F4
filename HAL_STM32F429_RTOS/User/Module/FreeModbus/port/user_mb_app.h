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


/* ���Թ��ܲ��� */
#define MB_SAMPLE_TEST_SLAVE_ADDR						1			//�ӻ��豸��ַ
#define MB_REG_START  											1			//����������ʼλ��
#define MB_SEND_REG_NUM    									4			//������������
#define MB_READ_REG_NUM    									1			//������������
#define WAITING_FOREVER											-1		//���õȴ�

/* ͨ���궨��ѡ���� */
#define MB_USER_HOLD 												1			//д������ּĴ���
#define MB_USER_COILS 											2			//д�����Ȧ
#define MB_USER_INPUT_REG										3			//������Ĵ���

extern void test(char MB);												//���Գ���


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
