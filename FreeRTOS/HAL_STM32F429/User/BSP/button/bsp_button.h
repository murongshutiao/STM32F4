#ifndef __BSP_BUTTON_H
#define	__BSP_BUTTON_H
#include "board.h"




 /** �������±��ú�
	* ��������Ϊ�ߵ�ƽ������ Button_ON=1�� Button_OFF=0
	* ����������Ϊ�͵�ƽ���Ѻ����ó�Button_ON=0 ��Button_OFF=1 ����
	*/
#define Button_ON	1
#define Button_OFF	0

void Button_GPIO_Config(void);
uint8_t Button_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#endif /* __LED_H */

