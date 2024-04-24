

  

#include ".\button\bsp_button.h" 

/**

  * @brief  配置按键用到的I/O口

  * @param  无

  * @retval 无

  */

void Button_GPIO_Config(void)

{

    GPIO_InitTypeDef GPIO_InitStructure;



    /*开启按键GPIO口的时钟*/
    Button1_GPIO_CLK_ENABLE();
    Button2_GPIO_CLK_ENABLE();

    /*选择按键的引脚*/	
    GPIO_InitStructure.Pin = Button1_PIN; 



    /*设置引脚为输入模式*/
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT; 


    /*设置引脚不上拉也不下拉*/
    GPIO_InitStructure.Pull = GPIO_NOPULL;

    /*使用上面的结构体初始化按键*/
    HAL_GPIO_Init(Button1_GPIO_PORT, &GPIO_InitStructure);

    /*选择按键的引脚*/
    GPIO_InitStructure.Pin = Button2_PIN; 


    /*使用上面的结构体初始化按键*/
    HAL_GPIO_Init(Button2_GPIO_PORT, &GPIO_InitStructure);

 
}



/**

  * @brief   检测是否有按键按下

  * @param   具体的端口和端口位

  *		@arg GPIOx: x可以是（A...G） 

  *		@arg GPIO_PIN 可以是GPIO_PIN_x（x可以是1...16）

  * @retval  按键的状态

  *		@arg Button_ON:按键按下

  *		@arg Button_OFF:按键没按下

  */

uint8_t Button_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)

{			

	/*检测是否有按键按下 */

	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == Button_ON )  

	{	 

		/*等待按键释放 */

		while(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == Button_ON);   

		return 	Button_ON;	 

	}

	else

		return Button_OFF;

}

/*********************************************END OF FILE**********************/

