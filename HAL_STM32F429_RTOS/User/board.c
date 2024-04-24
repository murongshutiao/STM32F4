#include "./board.h"




const uint32_t version = 0x12345678;


static void GPIO_CLK_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
}



void BSP_Init(void)
{

    //NVIC_SetVectorTable(FLASH_BASE,0x40000);

    /* 系统时钟初始化成216 MHz */
    SystemClock_Config();
    
    GPIO_CLK_Init();

    /* LED 端口初始化 */
    LED_GPIO_Config();
    // Button_GPIO_Config();
    Debug_Uart_Init();
    Debug_Uart_SendString("这是MPU6050测试程序\n");

    // Flash_SPI_Init();

    // Flash_Spi_test();
    //Fatfs_ExtFlash_Test();

    // MBMaster_Uart_Init();
    // MBSlave_Uart_Init();
    //MCO_Test();
    // DMA_MemToMem_Config();
    // DMA_MemToMem_Test();
    // Debug_Uart_Dma_Config();
    // Debug_Uart_Dma_Test();

    // EEPROM_I2C_Init();
    // EEPROM_I2C_Test();
    // I2C_Slave_Comm_Init();

    SDRAM_Init();
    // SDRAM_Test();
    //LCD_Init();
    //Lcd_RGB888_Cfg();
    //  Rheostat_Init();
    //TIMx_Configuration();
    // MultiTimer_Init();
    // MultiButton_Init();
    // MPU6050_Init();
    // MPU6050_Test();
    //WIFI_PDN_INIT();    /* 跟SD卡相关 */
    
//    CAN_Config();
//    CAN_SetMsg();

//    RTC_CLK_Config();
//    RTC_Test();

        //LwIP_Init();
    

}


extern struct netif gnetif;

void App_Task(void)
{
    /* 控制LED灯 */
    while (1)
    {
        multi_timer_yield();
        ethernetif_input(&gnetif);
        sys_check_timeouts();
    }    
}







