

#include "./eeprom/bsp_i2c_eeprom.h"

uint8_t I2c_Buf_Write[DATA_Size];
uint8_t I2c_Buf_Read[DATA_Size];


I2C_HandleTypeDef  EEPROM_I2C_Handler; 


/**
  * @brief  I2C 工作模式配置
  * @param  无
  * @retval 无
  */
static void I2C_Mode_Config(void)
{
   
    EEPROM_I2C_Handler.Instance             = EEPROM_I2C;
    
    EEPROM_I2C_Handler.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    EEPROM_I2C_Handler.Init.ClockSpeed      = 400000;
    EEPROM_I2C_Handler.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    EEPROM_I2C_Handler.Init.DutyCycle       = I2C_DUTYCYCLE_2;
    EEPROM_I2C_Handler.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    EEPROM_I2C_Handler.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
    EEPROM_I2C_Handler.Init.OwnAddress1     = I2C_OWN_ADDRESS7 ;
    EEPROM_I2C_Handler.Init.OwnAddress2     = 0; 
        /* Init the I2C */
    HAL_I2C_Init(&EEPROM_I2C_Handler);	

    HAL_I2CEx_AnalogFilter_Config(&EEPROM_I2C_Handler, I2C_ANALOGFILTER_ENABLE);    
}




/**
  * @brief I2C MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - DMA configuration for transmission request by peripheral 
  *           - NVIC configuration for DMA interrupt request enable
  * @param hi2c: I2C handle pointer
  * @retval None
  */
void EEPROM_I2C_Init(void)
{  
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    EEPROM_I2C_CLK_ENABLE(); 

    EEPROM_I2C_SCL_GPIO_CLK_ENABLE();
    EEPROM_I2C_SDA_GPIO_CLK_ENABLE();

    /*##-2- Configure peripheral GPIO ##########################################*/  
    /* I2C TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = EEPROM_I2C_SCL_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = EEPROM_I2C_SCL_AF;
    
    HAL_GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStruct);
        
    /* I2C RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = EEPROM_I2C_SDA_PIN;
    GPIO_InitStruct.Alternate = EEPROM_I2C_SDA_AF;
        
    HAL_GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStruct);
  
  	/* Force the I2C peripheral clock reset */  
	EEPROM_I2C_FORCE_RESET() ; 

	/* Release the I2C peripheral clock reset */  
	EEPROM_I2C_RELEASE_RESET(); 

    I2C_Mode_Config();
}





/**
  * @brief   将缓冲区中的数据写到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
void EEPROM_WriteBuffer( uint8_t WriteAddr,uint8_t* pBuffer, uint16_t NumByteToWrite)
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

    Addr = WriteAddr % EEPROM_PAGESIZE;
    count = EEPROM_PAGESIZE - Addr;
    NumOfPage =  NumByteToWrite / EEPROM_PAGESIZE;
    NumOfSingle = NumByteToWrite % EEPROM_PAGESIZE;
    
    /* If WriteAddr is I2C_PageSize aligned  */
    if(Addr == 0) 
    {
        /* If NumByteToWrite < I2C_PageSize */
        if(NumOfPage == 0) 
        {
            EEPROM_WritePage(WriteAddr, pBuffer, NumOfSingle);
        }
        /* If NumByteToWrite > I2C_PageSize */
        else  
        {
        while(NumOfPage--)
        {
            EEPROM_WritePage(WriteAddr, pBuffer, EEPROM_PAGESIZE); 
            WriteAddr +=  EEPROM_PAGESIZE;
            pBuffer += EEPROM_PAGESIZE;
        }

        if(NumOfSingle!=0)
        {
            EEPROM_WritePage(WriteAddr, pBuffer, NumOfSingle);
        }
        }
    }
    /* If WriteAddr is not I2C_PageSize aligned  */
    else 
    {
        /* If NumByteToWrite < I2C_PageSize */
        if(NumOfPage== 0) 
        {
            EEPROM_WritePage(WriteAddr, pBuffer, NumOfSingle);
        }
        /* If NumByteToWrite > I2C_PageSize */
        else
        {
            NumByteToWrite -= count;
            NumOfPage =  NumByteToWrite / EEPROM_PAGESIZE;
            NumOfSingle = NumByteToWrite % EEPROM_PAGESIZE;	
            
            if(count != 0)
            {  
                EEPROM_WritePage(WriteAddr, pBuffer, count);
                WriteAddr += count;
                pBuffer += count;
            } 
            
            while(NumOfPage--)
            {
                EEPROM_WritePage(WriteAddr, pBuffer, EEPROM_PAGESIZE);
                WriteAddr +=  EEPROM_PAGESIZE;
                pBuffer += EEPROM_PAGESIZE;  
            }
            if(NumOfSingle != 0)
            {
                EEPROM_WritePage(WriteAddr, pBuffer, NumOfSingle); 
            }
        }
    }  
}

/**
  * @brief   写一个字节到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址 
  * @retval  无
  */
uint32_t EEPROM_WriteByte( uint8_t WriteAddr,uint8_t* pBuffer)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Write(&EEPROM_I2C_Handler, EEPROM_ADDRESS, (uint16_t)WriteAddr, I2C_MEMADD_SIZE_8BIT, pBuffer, 1, 100); 

	/* Check the communication status */
	if(status != HAL_OK)
	{
	/* Execute user timeout callback */
	//I2Cx_Error(Addr);
	}
	while (HAL_I2C_GetState(&EEPROM_I2C_Handler) != HAL_I2C_STATE_READY)
	{
		
	}

	/* Check if the EEPROM is ready for a new operation */
	while (HAL_I2C_IsDeviceReady(&EEPROM_I2C_Handler, EEPROM_ADDRESS, EEPROM_MAX_TRIALS, I2Cx_TIMEOUT_MAX) == HAL_TIMEOUT);

	/* Wait for the end of the transfer */
	while (HAL_I2C_GetState(&EEPROM_I2C_Handler) != HAL_I2C_STATE_READY)
	{
		
	}
	return status;
}

/**
  * @brief   在EEPROM的一个写循环中可以写多个字节，但一次写入的字节数
  *          不能超过EEPROM页的大小，AT24C02每页有8个字节
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
uint32_t EEPROM_WritePage(uint8_t WriteAddr,uint8_t* pBuffer, uint8_t NumByteToWrite)
{
	HAL_StatusTypeDef status = HAL_OK;
	/* Write EEPROM_PAGESIZE */
	status=HAL_I2C_Mem_Write(&EEPROM_I2C_Handler, EEPROM_ADDRESS,WriteAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t*)(pBuffer),NumByteToWrite, 100);

	while (HAL_I2C_GetState(&EEPROM_I2C_Handler) != HAL_I2C_STATE_READY)
	{
		
	}

	/* Check if the EEPROM is ready for a new operation */
	while (HAL_I2C_IsDeviceReady(&EEPROM_I2C_Handler, EEPROM_ADDRESS, EEPROM_MAX_TRIALS, I2Cx_TIMEOUT_MAX) == HAL_TIMEOUT);

	/* Wait for the end of the transfer */
	while (HAL_I2C_GetState(&EEPROM_I2C_Handler) != HAL_I2C_STATE_READY)
	{
		
	}
	return status;
}

/**
  * @brief   从EEPROM里面读取一块数据 
  * @param   
  *		@arg pBuffer:存放从EEPROM读取的数据的缓冲区指针
  *		@arg WriteAddr:接收数据的EEPROM的地址
  *     @arg NumByteToWrite:要从EEPROM读取的字节数
  * @retval  无
  */
uint32_t EEPROM_ReadBuffer( uint8_t ReadAddr,uint8_t* pBuffer, uint16_t NumByteToRead)
{
	HAL_StatusTypeDef status = HAL_OK;
	
	status = HAL_I2C_Mem_Read(&EEPROM_I2C_Handler,EEPROM_ADDRESS,ReadAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)pBuffer, NumByteToRead,1000);

	return status;
}



void EEPROM_I2C_Test(void)
{
	uint16_t i;

	EEPROM_INFO("写入的数据");

	for ( i=0; i<DATA_Size; i++ ) //填充缓冲
	{   
		I2c_Buf_Write[i] =i;
        EEPROM_INFO("%d ", I2c_Buf_Write[i]);
		if(i%16 == 15)  
EEPROM_INFO("\n\r");            
	}

	//将I2c_Buf_Write中顺序递增的数据写入EERPOM中 
	EEPROM_WriteBuffer(EEP_Firstpage, I2c_Buf_Write, DATA_Size);

	EEPROM_INFO("读出的数据");
	//将EEPROM读出数据顺序保持到I2c_Buf_Read中
	EEPROM_ReadBuffer( EEP_Firstpage,I2c_Buf_Read, DATA_Size); 
	//将I2c_Buf_Read中的数据通过串口打印
	for (i=0; i<DATA_Size; i++)
	{	
		if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
		{
			EEPROM_INFO("%d ", I2c_Buf_Read[i]);
			EEPROM_INFO("错误:I2C EEPROM写入与读出的数据不一致");
			//return 0;
		}
		EEPROM_INFO("%d ", I2c_Buf_Read[i]);
		if(i%16 == 15)    
		EEPROM_INFO("\n\r");

	}
	EEPROM_INFO("I2C(AT24C02)读写测试成功");
	//return 1;    
}


/*********************************************END OF FILE**********************/
