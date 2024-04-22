#include "./Module/MultiButton/multi_button_user.h"




uint8_t btn1_id = BTNID1;

struct Button btn1;

uint8_t read_button_GPIO(uint8_t button_id)
{
	uint8_t out;
    
	switch(button_id)
	{
		case BTNID1:
			out = HAL_GPIO_ReadPin(Button1_GPIO_PORT,Button1_PIN);
			break;

		default:
			return 0;
			break;
	}
    
    return out;
}

void BTN1_SINGLE_Click_Handler(void* btn)
{
    LED1_TOGGLE;
	// SD_Test();
	// Fatfs_SDCard_Test();
//	CAN_SetMsg();
//	HAL_CAN_Transmit_IT(&Can_Handle);
	// Jump_Dfu_BootLoader();
	//InternalFlash_Test2();
	
}

void BTN1_DOUBLE_Click_Handler(void* btn)
{
	LED2_TOGGLE;
}

void MultiButton_Init(void)
{
    Button_GPIO_Config();

    button_init(&btn1, read_button_GPIO, 1, btn1_id);
    button_attach(&btn1, SINGLE_CLICK,     BTN1_SINGLE_Click_Handler);
	button_attach(&btn1, DOUBLE_CLICK,     BTN1_DOUBLE_Click_Handler);

    button_start(&btn1);
}











