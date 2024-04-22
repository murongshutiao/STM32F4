#ifndef __BSP_TIMER_H
#define	__BSP_TIMER_H
#include "board.h"

#define BASIC_TIM           		TIM6
#define BASIC_TIM_CLK_ENABLE()     	__TIM6_CLK_ENABLE()

#define BASIC_TIM_IRQn				TIM6_DAC_IRQn
#define BASIC_TIM_IRQHandler    	TIM6_DAC_IRQHandler

#define MultiTimer_TIM           		TIM7
#define MultiTimer_TIM_CLK_ENABLE()     	__TIM7_CLK_ENABLE()

#define MultiTimer_TIM_IRQn				TIM7_IRQn
#define MultiTimer_TIM_IRQHandler    	TIM7_IRQHandler


void TIMx_Configuration(void);
void MultiTimer_TIM_Init(void);
uint64_t GetMultiTimer_Ticks(void);
#endif /* __BASIC_TIM_H */

