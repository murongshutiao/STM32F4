#ifndef __BSP_BUTTON_H
#define	__BSP_BUTTON_H
#include "board.h"




 /** 按键按下标置宏
	* 按键按下为高电平，设置 Button_ON=1， Button_OFF=0
	* 若按键按下为低电平，把宏设置成Button_ON=0 ，Button_OFF=1 即可
	*/
#define Button_ON	1
#define Button_OFF	0

void Button_GPIO_Config(void);
uint8_t Button_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#endif /* __LED_H */

