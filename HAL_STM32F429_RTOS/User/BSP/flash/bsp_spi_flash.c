#include "./flash/bsp_spi_flash.h"

#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[] = "感谢您选用野火stm32开发板\r\nhttp://fire-stm32.taobao.com";
uint8_t Rx_Buffer[BufferSize];


SPI_HandleTypeDef FlashSpiHandler;

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;   

static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);


void Flash_Spi_Mode_Init(void)
{
    /*##-1- Configure the SPI peripheral #######################################*/
    /* Set the SPI parameters */
    FlashSpiHandler.Instance               = Flash_SPI;
    FlashSpiHandler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;       /* 时钟分配因子 */
    FlashSpiHandler.Init.Direction         = SPI_DIRECTION_2LINES;          /* 双向模式 */
    FlashSpiHandler.Init.CLKPhase          = SPI_PHASE_2EDGE;               /* CPHA */
    FlashSpiHandler.Init.CLKPolarity       = SPI_POLARITY_HIGH;             /* CPOL */
    FlashSpiHandler.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;    /* 不使用CRC */
    FlashSpiHandler.Init.CRCPolynomial     = 7;
    FlashSpiHandler.Init.DataSize          = SPI_DATASIZE_8BIT;             /* 数据长度 */
    FlashSpiHandler.Init.FirstBit          = SPI_FIRSTBIT_MSB;              /* MSB先行 */
    FlashSpiHandler.Init.NSS               = SPI_NSS_SOFT;                  /* NEE不使用SPI硬件控制 */
    FlashSpiHandler.Init.TIMode            = SPI_TIMODE_DISABLE;            /* 不启用中断模式 */
    FlashSpiHandler.Init.Mode = SPI_MODE_MASTER;                            /* 配置为主机模式 */

    HAL_SPI_Init(&FlashSpiHandler); 
    
    __HAL_SPI_ENABLE(&FlashSpiHandler); 

    /* 使 SPI_FLASH 进入 4 字节地址模式,这个地址模式跟NOR FLASH容量有关 */
	SPI_FLASH_Mode_Init();  
}


/**
  * @brief SPI MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param hspi: SPI handle pointer
  * @retval None
  */
void Flash_SPI_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
    Flash_SPI_SCK_GPIO_CLK_ENABLE();
    Flash_SPI_MISO_GPIO_CLK_ENABLE();
    Flash_SPI_MOSI_GPIO_CLK_ENABLE();
    Flash_SPI_CS_GPIO_CLK_ENABLE();
    /* Enable SPI clock */
    Flash_SPI_CLK_ENABLE(); 
    
    /*##-2- Configure peripheral GPIO ##########################################*/  
    /* SPI SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = Flash_SPI_SCK_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = Flash_SPI_SCK_AF;
    
    HAL_GPIO_Init(Flash_SPI_SCK_GPIO_PORT, &GPIO_InitStruct);
        
    /* SPI MISO GPIO pin configuration  */
    GPIO_InitStruct.Pin = Flash_SPI_MISO_PIN;
    GPIO_InitStruct.Alternate = Flash_SPI_MISO_AF;
    
    HAL_GPIO_Init(Flash_SPI_MISO_GPIO_PORT, &GPIO_InitStruct);
    
    /* SPI MOSI GPIO pin configuration  */
    GPIO_InitStruct.Pin = Flash_SPI_MOSI_PIN;
    GPIO_InitStruct.Alternate = Flash_SPI_MOSI_AF;  
    HAL_GPIO_Init(Flash_SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);   

    GPIO_InitStruct.Pin = FLASH_SPI_CS_PIN ;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(FLASH_SPI_CS_GPIO_PORT, &GPIO_InitStruct); 

    Flash_Spi_Mode_Init();
}




 /**
  * @brief  擦除FLASH扇区
  * @param  SectorAddr：要擦除的扇区地址
  * @retval 无
  */
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
    /* 发送FLASH写使能命令 */
    SPI_FLASH_WriteEnable();
    SPI_FLASH_WaitForWriteEnd();
    /* 擦除扇区 */
    /* 选择FLASH: CS低电平 */
    SPI_FLASH_CS_LOW();
    /* 发送扇区擦除指令*/
    SPI_FLASH_SendByte(W25X_SectorErase);
    // /*发送擦除扇区地址的高8位*/
    // SPI_FLASH_SendByte((SectorAddr & 0xFF000000) >> 24);
    /*发送擦除扇区地址的中前8位*/
    SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
    /* 发送擦除扇区地址的中后8位 */
    SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
    /* 发送擦除扇区地址的低8位 */
    SPI_FLASH_SendByte(SectorAddr & 0xFF);
    /* 停止信号 FLASH: CS 高电平 */
    SPI_FLASH_CS_HIGH();
    /* 等待擦除完毕*/
    SPI_FLASH_WaitForWriteEnd();
}


 /**
  * @brief  擦除FLASH扇区，整片擦除
  * @param  无
  * @retval 无
  */
void SPI_FLASH_BulkErase(void)
{
    /* 发送FLASH写使能命令 */
    SPI_FLASH_WriteEnable();

    /* 整块 Erase */
    /* 选择FLASH: CS低电平 */
    SPI_FLASH_CS_LOW();
    /* 发送整块擦除指令*/
    SPI_FLASH_SendByte(W25X_ChipErase);
    /* 停止信号 FLASH: CS 高电平 */
    SPI_FLASH_CS_HIGH();

    /* 等待擦除完毕*/
    SPI_FLASH_WaitForWriteEnd();
}

 /**
  * @brief  对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度，必须小于等于SPI_FLASH_PerWritePageSize
  * @retval 无
  */
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
    /* 发送FLASH写使能命令 */
    SPI_FLASH_WriteEnable();

    /* 选择FLASH: CS低电平 */
    SPI_FLASH_CS_LOW();
    /* 写页写指令*/
    SPI_FLASH_SendByte(W25X_PageProgram);

    /*发送写地址的高8位*/
    // SPI_FLASH_SendByte((WriteAddr & 0xFF000000) >> 24);
    /*发送写地址的中前8位*/
    SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
    /*发送写地址的中后8位*/
    SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
    /*发送写地址的低8位*/
    SPI_FLASH_SendByte(WriteAddr & 0xFF);

    if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
    {
        NumByteToWrite = SPI_FLASH_PerWritePageSize;
        FLASH_ERROR("SPI_FLASH_PageWrite too large!");
    }

    /* 写入数据*/
    while (NumByteToWrite--)
    {
        /* 发送当前要写入的字节数据 */
        SPI_FLASH_SendByte(*pBuffer);
        /* 指向下一字节数据 */
        pBuffer++;
    }

    /* 停止信号 FLASH: CS 高电平 */
    SPI_FLASH_CS_HIGH();

    /* 等待写入完毕*/
    SPI_FLASH_WaitForWriteEnd();
}


 /**
  * @brief  对FLASH写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param  WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度
  * @retval 无
  */
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
  Addr = WriteAddr % SPI_FLASH_PageSize;
	
	/*差count个数据值，刚好可以对齐到页地址*/
  count = SPI_FLASH_PageSize - Addr;	
	/*计算出要写多少整数页*/
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/*mod运算求余，计算出剩余不满一页的字节数*/
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

	 /* Addr=0,则WriteAddr 刚好按页对齐 aligned  */
  if (Addr == 0) 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*先把整数页都写了*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			
			/*若有多余的不满一页的数据，把它写完*/
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
	/* 若地址与 SPI_FLASH_PageSize 不对齐  */
  else 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
			/*当前页剩余的count个位置比NumOfSingle小，写不完*/
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
				
				/*先写满当前页*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;
				
				/*再写剩余的数据*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
      {				
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*地址不对齐多出的count分开处理，不加入这个运算*/
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;
			
			/*把整数页都写了*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*若有多余的不满一页的数据，把它写完*/
      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

 /**
  * @brief  读取FLASH数据
  * @param 	pBuffer，存储读出数据的指针
  * @param   ReadAddr，读取地址
  * @param   NumByteToRead，读取数据长度
  * @retval 无
  */
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
    /* 选择FLASH: CS低电平 */
    SPI_FLASH_CS_LOW();

    /* 发送 读 指令 */
    SPI_FLASH_SendByte(W25X_ReadData);

    /* 发送 读 地址高8位 */
    // SPI_FLASH_SendByte((ReadAddr & 0xFF000000) >> 24);
    /* 发送 读 地址中前8位 */
    SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    /* 发送 读 地址中后8位 */
    SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
    /* 发送 读 地址低8位 */
    SPI_FLASH_SendByte(ReadAddr & 0xFF);
    
        /* 读取数据 */
    while (NumByteToRead--)
    {
        /* 读取一个字节*/
        *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
        /* 指向下一个字节缓冲区 */
        pBuffer++;
    }

    /* 停止信号 FLASH: CS 高电平 */
    SPI_FLASH_CS_HIGH();
}


 /**
  * @brief  读取FLASH ID
  * @param 	无
  * @retval FLASH ID
  */
uint32_t SPI_FLASH_ReadID(void)
{
    uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    /* 开始通讯：CS低电平 */
    SPI_FLASH_CS_LOW();

    /* 发送JEDEC指令，读取ID */
    SPI_FLASH_SendByte(W25X_JedecDeviceID);

    /* 读取一个字节数据 */
    Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

    /* 读取一个字节数据 */
    Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

    /* 读取一个字节数据 */
    Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

    /* 停止通讯：CS高电平 */
    SPI_FLASH_CS_HIGH();

        /*把数据组合起来，作为函数的返回值*/
    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

    return Temp;
}

 /**
  * @brief  读取FLASH Device ID
  * @param 	无
  * @retval FLASH Device ID
  */
uint32_t SPI_FLASH_ReadDeviceID(void)
{
    uint32_t Temp = 0;

    /* Select the FLASH: Chip Select low */
    SPI_FLASH_CS_LOW();

    /* Send "RDID " instruction */
    SPI_FLASH_SendByte(W25X_DeviceID);
    SPI_FLASH_SendByte(Dummy_Byte);
    SPI_FLASH_SendByte(Dummy_Byte);
    SPI_FLASH_SendByte(Dummy_Byte);
    
    /* Read a byte from the FLASH */
    Temp = SPI_FLASH_SendByte(Dummy_Byte);

    /* Deselect the FLASH: Chip Select high */
    SPI_FLASH_CS_HIGH();

    return Temp;
}



/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{
    /* Select the FLASH: Chip Select low */
    SPI_FLASH_CS_LOW();

    /* Send "Read from Memory " instruction */
    SPI_FLASH_SendByte(W25X_ReadData);

    /* Send the 24-bit address of the address to read from -----------------------*/
    /* Send ReadAddr high nibble address byte */
    SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    /* Send ReadAddr medium nibble address byte */
    SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
    /* Send ReadAddr low nibble address byte */
    SPI_FLASH_SendByte(ReadAddr & 0xFF);
}


 /**
  * @brief  使用SPI读取一个字节的数据
  * @param  无
  * @retval 返回接收到的数据
  */
uint8_t SPI_FLASH_ReadByte(void)
{
    return (SPI_FLASH_SendByte(Dummy_Byte));
}


 /**
  * @brief  使用SPI发送一个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
    SPITimeout = SPIT_FLAG_TIMEOUT;

    /* 等待发送缓冲区为空，TXE事件 */
    while (__HAL_SPI_GET_FLAG( &FlashSpiHandler, SPI_FLAG_TXE ) == RESET)
    {
        if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
    }

    /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
    WRITE_REG(FlashSpiHandler.Instance->DR, byte);

    SPITimeout = SPIT_FLAG_TIMEOUT;

    /* 等待接收缓冲区非空，RXNE事件 */
    while (__HAL_SPI_GET_FLAG( &FlashSpiHandler, SPI_FLAG_RXNE ) == RESET)
    {
        if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
    }

    /* 读取数据寄存器，获取接收缓冲区数据 */
    return READ_REG(FlashSpiHandler.Instance->DR);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendHalfWord
* Description    : Sends a Half Word through the SPI interface and return the
*                  Half Word received from the SPI bus.
* Input          : Half Word : Half Word to send.
* Output         : None
* Return         : The value of the received Half Word.
*******************************************************************************/
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord)
{
  
    SPITimeout = SPIT_FLAG_TIMEOUT;

    /* Loop while DR register in not emplty */
    while (__HAL_SPI_GET_FLAG( &FlashSpiHandler,  SPI_FLAG_TXE ) == RESET)
    {
        if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
    }

    /* Send Half Word through the Flash_SPI peripheral */
    WRITE_REG(FlashSpiHandler.Instance->DR, HalfWord);

    SPITimeout = SPIT_FLAG_TIMEOUT;

    /* Wait to receive a Half Word */
    while (__HAL_SPI_GET_FLAG( &FlashSpiHandler, SPI_FLAG_RXNE ) == RESET)
    {
        if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
    }
    /* Return the Half Word read from the SPI bus */
    return READ_REG(FlashSpiHandler.Instance->DR);
}


 /**
  * @brief  向FLASH发送 写使能 命令
  * @param  none
  * @retval none
  */
void SPI_FLASH_WriteEnable(void)
{
    /* 通讯开始：CS低 */
    SPI_FLASH_CS_LOW();

    /* 发送写使能命令*/
    SPI_FLASH_SendByte(W25X_WriteEnable);

    /*通讯结束：CS高 */
    SPI_FLASH_CS_HIGH();
}



 /**
  * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
  * @param  none
  * @retval none
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
    uint8_t FLASH_Status = 0;

    /* 选择 FLASH: CS 低 */
    SPI_FLASH_CS_LOW();

    /* 发送 读状态寄存器 命令 */
    SPI_FLASH_SendByte(W25X_ReadStatusReg);

    SPITimeout = SPIT_FLAG_TIMEOUT;
    /* 若FLASH忙碌，则等待 */
    do
    {
        /* 读取FLASH芯片的状态寄存器 */
        FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);	 

        {
        if((SPITimeout--) == 0) 
        {
            SPI_TIMEOUT_UserCallback(4);
            return;
        }
        } 
    }
    while ((FLASH_Status & WIP_Flag) == SET); /* 正在写入标志 */

    /* 停止信号  FLASH: CS 高 */
    SPI_FLASH_CS_HIGH();
}


//进入掉电模式
void SPI_Flash_PowerDown(void)   
{ 
    /* 选择 FLASH: CS 低 */
    SPI_FLASH_CS_LOW();

    /* 发送 掉电 命令 */
    SPI_FLASH_SendByte(W25X_PowerDown);

    /* 停止信号  FLASH: CS 高 */
    SPI_FLASH_CS_HIGH();
}   



//唤醒
void SPI_Flash_WAKEUP(void)   
{
    /*选择 FLASH: CS 低 */
    SPI_FLASH_CS_LOW();

    /* 发上 上电 命令 */
    SPI_FLASH_SendByte(W25X_ReleasePowerDown);

    /* 停止信号 FLASH: CS 高 */
    SPI_FLASH_CS_HIGH();                   //等待TRES1
}   



 /**
  * @brief  使 SPI_FLASH 进入 4 字节地址模式
  * @param  none
  * @retval none
  */
void SPI_FLASH_Mode_Init(void)
{
	uint8_t Temp;
	
	/*选择 FLASH: CS 低 */
	SPI_FLASH_CS_LOW();
	
	/* 发送状态寄存器 3 命令 */
	SPI_FLASH_SendByte(W25X_ReadStatusRegister3); 
	
	Temp = SPI_FLASH_SendByte(Dummy_Byte);
	
	/* 停止信号 FLASH: CS 高 */
	SPI_FLASH_CS_HIGH();
	
	if((Temp&0x01) == 0)
	{
		/*选择 FLASH: CS 低 */
		SPI_FLASH_CS_LOW();
		
		/* 进入4字节模式 */
		SPI_FLASH_SendByte(W25X_Enter4ByteMode);
		
		/* 停止信号 FLASH: CS 高 */
		SPI_FLASH_CS_HIGH();
	}
}



/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
    /* 等待超时后的处理,输出错误信息 */
    FLASH_ERROR("SPI 等待超时!errorCode = %d",errorCode);
    return 0;
}


#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress


void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}


void Flash_Spi_test(void)
{


    //读取的ID存储位置
    __IO uint32_t DeviceID = 0;
    __IO uint32_t FlashID = 0;
    __IO TestStatus TransferStatus1 = FAILED;

	/* 获取 Flash Device ID */
	DeviceID = SPI_FLASH_ReadDeviceID();

    Delay( 200 );

	/* 获取 SPI Flash ID */
	FlashID = SPI_FLASH_ReadID();

	/* 检验 SPI Flash ID */
	if (FlashID == sFLASH_ID) 
	{	
		/* 擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除 */
		SPI_FLASH_SectorErase(FLASH_SectorToErase);	 	 
		
		/* 将发送缓冲区的数据写到flash中 */
		SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
		
		/* 将刚刚写入的数据读出来放到接收缓冲区中 */
		SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		
		/* 检查写入的数据与读出的数据是否相等 */
		TransferStatus1 = U8Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
		if( PASSED == TransferStatus1 )
		{    
			LED_GREEN;

		}
		else
		{        
			LED_RED;

		}
	}
	else
	{    
		LED_RED;

	}
	
	SPI_Flash_PowerDown();     
}
   
/*********************************************END OF FILE**********************/
