

  

#include ".\button\bsp_button.h" 

/**

  * @brief  ���ð����õ���I/O��

  * @param  ��

  * @retval ��

  */

void Button_GPIO_Config(void)

{

    GPIO_InitTypeDef GPIO_InitStructure;



    /*��������GPIO�ڵ�ʱ��*/
    Button1_GPIO_CLK_ENABLE();
    Button2_GPIO_CLK_ENABLE();

    /*ѡ�񰴼�������*/	
    GPIO_InitStructure.Pin = Button1_PIN; 



    /*��������Ϊ����ģʽ*/
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT; 


    /*�������Ų�����Ҳ������*/
    GPIO_InitStructure.Pull = GPIO_NOPULL;

    /*ʹ������Ľṹ���ʼ������*/
    HAL_GPIO_Init(Button1_GPIO_PORT, &GPIO_InitStructure);

    /*ѡ�񰴼�������*/
    GPIO_InitStructure.Pin = Button2_PIN; 


    /*ʹ������Ľṹ���ʼ������*/
    HAL_GPIO_Init(Button2_GPIO_PORT, &GPIO_InitStructure);

 
}



/**

  * @brief   ����Ƿ��а�������

  * @param   ����Ķ˿ںͶ˿�λ

  *		@arg GPIOx: x�����ǣ�A...G�� 

  *		@arg GPIO_PIN ������GPIO_PIN_x��x������1...16��

  * @retval  ������״̬

  *		@arg Button_ON:��������

  *		@arg Button_OFF:����û����

  */

uint8_t Button_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)

{			

	/*����Ƿ��а������� */

	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == Button_ON )  

	{	 

		/*�ȴ������ͷ� */

		while(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == Button_ON);   

		return 	Button_ON;	 

	}

	else

		return Button_OFF;

}

/*********************************************END OF FILE**********************/

