#ifndef __BSP_ADC_H
#define	__BSP_ADC_H
#include "board.h"

// ADC DR�Ĵ����궨�壬ADCת���������ֵ����������
#define RHEOSTAT_ADC_DR_ADDR                ((uint32_t)ADC1+0x4c)

// ADC GPIO �궨��
#define RHEOSTAT_ADC_GPIO_PORT              GPIOC
#define RHEOSTAT_ADC_GPIO_PIN               GPIO_PIN_3
#define RHEOSTAT_ADC_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()
    
// ADC ��ź궨��
#define RHEOSTAT_ADC                        ADC1
#define RHEOSTAT_ADC_CLK_ENABLE()           __ADC1_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL                ADC_CHANNEL_13

// ADC �жϺ궨��
#define Rheostat_ADC_IRQ                    ADC_IRQn
#define Rheostat_ADC_INT_FUNCTION           ADC_IRQHandler

void Rheostat_Init(void);

#endif /* __BSP_ADC_H */



