#ifndef __BSP_BUTTON_H
#define	__BSP_BUTTON_H
#include "board.h"




 /** �������±��ú�
	* ��������Ϊ�ߵ�ƽ������ Button_ON=1�� Button_OFF=0
	* ����������Ϊ�͵�ƽ���Ѻ����ó�Button_ON=0 ��Button_OFF=1 ����
	*/
#define Button_ON	1
#define Button_OFF	0


//���Ŷ���
/*******************************************************/
#define Button1_PIN                  GPIO_PIN_0                 
#define Button1_GPIO_PORT            GPIOA                      
#define Button1_GPIO_CLK_ENABLE()    __GPIOA_CLK_ENABLE()

#define Button2_PIN                  GPIO_PIN_13                 
#define Button2_GPIO_PORT            GPIOC                      
#define Button2_GPIO_CLK_ENABLE()    __GPIOC_CLK_ENABLE()
/*******************************************************/


void Button_GPIO_Config(void);
uint8_t Button_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#endif /* __LED_H */

