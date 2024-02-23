#ifndef __BSP_ADC_H
#define	__BSP_ADC_H
#include "board.h"

// ADC DR寄存器宏定义，ADC转换后的数字值则存放在这里
#define RHEOSTAT_ADC_DR_ADDR                ((uint32_t)ADC1+0x4c)



void Rheostat_Init(void);

#endif /* __BSP_ADC_H */



