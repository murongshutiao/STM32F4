
  
#include "./internalflash/bsp_internalflash.h"   

/*准备写入的测试数据*/
#define DATA_32                 ((uint32_t)0x87654321)


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* 要擦除内部FLASH的起始地址 */
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_5   
/* 要擦除内部FLASH的结束地址 */
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_7  


static uint32_t GetSector(uint32_t Address);

/**
  * @brief  InternalFlash_Test,对内部FLASH进行读写测试
  * @param  None
  * @retval None
  */
int InternalFlash_Test(void)
{
	/*要擦除的起始扇区(包含)及结束扇区(不包含)，如8-12，表示擦除8、9、10、11扇区*/
	uint32_t FirstSector = 0;
	uint32_t NbOfSectors = 0;
	uint32_t SECTORError = 0;	/* 擦写返回结果 */
	
	uint32_t Address = 0;

	__IO uint32_t Data32 = 0;
	__IO uint32_t MemoryProgramStatus = 0;
	static FLASH_EraseInitTypeDef EraseInitStruct;
	
	HAL_FLASH_Unlock();

	FirstSector = GetSector(FLASH_USER_START_ADDR);	/* 获取扇区号 */
	NbOfSectors = GetSector(FLASH_USER_END_ADDR)- FirstSector + 1;	/* 获取扇区数量 */
	
	/* 擦除用户区域 (用户区域指程序本身没有使用的空间，可以自定义)**/
	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;	/* 擦写类型:扇区 */
	EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;		/* 以“字”的大小进行操作 */ 
	EraseInitStruct.Sector        = FirstSector;				/* 擦写的扇区号 */
	EraseInitStruct.NbSectors     = NbOfSectors;				/* 擦写的扇区数量 */
	/* 开始擦除操作 */
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	{
		/*擦除出错，返回，实际应用中可加入处理 */
		return -1;
	}

	/* 以“字”的大小为单位写入数据 ********************************/
	Address = FLASH_USER_START_ADDR;

	while (Address < FLASH_USER_END_ADDR)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, DATA_32) == HAL_OK)
		{
		  Address = Address + 4;
		}
		else
		{ 
		  /*写入出错，返回，实际应用中可加入处理 */
				return -1;
		}
	}


	/* 给FLASH上锁，防止内容被篡改*/
	HAL_FLASH_Lock(); 


	/* 从FLASH中读取出数据进行校验***************************************/
	/*  MemoryProgramStatus = 0: 写入的数据正确
	  MemoryProgramStatus != 0: 写入的数据错误，其值为错误的个数 */
	Address = FLASH_USER_START_ADDR;
	MemoryProgramStatus = 0;

	while (Address < FLASH_USER_END_ADDR)
	{
		Data32 = *(__IO uint32_t*)Address;

		if (Data32 != DATA_32)
		{
		  MemoryProgramStatus++;  
		}

		Address = Address + 4;
	}  
	/* 数据校验不正确 */
	if(MemoryProgramStatus)
	{    
		return -1;
	}
	else /*数据校验正确*/
	{ 
		return 0;   
	}
}



/**
  * @brief  根据输入的地址给出它所在的sector
  *					例如：
						uwStartSector = GetSector(FLASH_USER_START_ADDR);
						uwEndSector = GetSector(FLASH_USER_END_ADDR);	
  * @param  Address：地址
  * @retval 地址所在的sector
  */
static uint32_t GetSector(uint32_t Address)
{
	uint32_t sector = 0;
	
	if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
	{
		sector = FLASH_SECTOR_0;  
	}
	else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
	{
		sector = FLASH_SECTOR_1;  
	}
	else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
	{
		sector = FLASH_SECTOR_2;  
	}
	else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
	{
		sector = FLASH_SECTOR_3;  
	}
	else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
	{
		sector = FLASH_SECTOR_4;  
	}
	else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
	{
		sector = FLASH_SECTOR_5;  
	}
	else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
	{
		sector = FLASH_SECTOR_6;  
	}
	else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
	{
		sector = FLASH_SECTOR_7;  
	}
	else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
	{
		sector = FLASH_SECTOR_8;  
	}
	else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
	{
		sector = FLASH_SECTOR_9; 
	}
	else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
	{
		sector = FLASH_SECTOR_10;  
	}
	else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_23))*/
	{
		sector = FLASH_SECTOR_11;  
	}
	return sector;
}


/* 使用此函数要先解锁 */
int8_t InternalFlashEraseSector(uint32_t EraseStartAddr,uint32_t EraseEndAddr)
{
	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t SECTORError = 0;
	uint32_t NbOfSectors = 0;
	uint32_t FirstSector = 0;	/* 第一个扇区 */
	
	FirstSector = GetSector(EraseStartAddr);	/* 获取扇区号 */
	NbOfSectors = GetSector(EraseEndAddr) - FirstSector + 1;	/* 获取扇区数量 */

	EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;	/* 擦写类型:扇区 */
	EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;		/* 以“字”的大小进行操作 */ 
	EraseInitStruct.Sector        = FirstSector;				/* 擦写的扇区号 */
	EraseInitStruct.NbSectors     = NbOfSectors;				/* 擦写的扇区数量 */

	/* 开始擦除操作 */
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	{
		/*擦除出错，返回，实际应用中可加入处理 */
		return -1;
	}

	return 0;
}


int8_t InternalFlash_WriteData(uint32_t WriteStartAddr,uint32_t *pWriteData,uint32_t Num)
{

	uint32_t Address = WriteStartAddr;
	uint32_t WriteEndAddr = WriteStartAddr + Num * 4;

	HAL_FLASH_Unlock();

	while (Address < WriteEndAddr)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, *pWriteData++) == HAL_OK)
		{
		  	Address += 4;
		}
		else
		{ 
		  	/*写入出错，返回，实际应用中可加入处理 */
			return -1;
		}
	}

	/* 给FLASH上锁，防止内容被篡改*/
	HAL_FLASH_Lock(); 
    
    return 0;
}




int8_t InternalFlash_EraseWriteData(uint32_t WriteStartAddr,uint32_t *pWriteData,uint32_t Num)
{

	uint32_t Address = WriteStartAddr;
	uint32_t WriteEndAddr = WriteStartAddr + Num * 4;

	HAL_FLASH_Unlock();

	InternalFlashEraseSector(WriteStartAddr,WriteEndAddr);

	while (Address < WriteEndAddr)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, *pWriteData++) == HAL_OK)
		{
		  	Address += 4;
		}
		else
		{ 
		  	/*写入出错，返回，实际应用中可加入处理 */
			return -1;
		}
	}

	/* 给FLASH上锁，防止内容被篡改*/
	HAL_FLASH_Lock(); 
    
    return 0;
}

void InternalFlash_ReadData(uint32_t ReadStartAddr,uint32_t *pReadData,uint32_t Num)
{
	uint32_t ReadEndAddr = ReadStartAddr + Num * 4;
	uint32_t Address = ReadStartAddr;
	uint32_t *pTemp = pReadData;

	while (Address < ReadEndAddr)
	{
		*pTemp++ = *(__IO uint32_t*)Address;
		Address += 4;
	} 
}



void InternalFlash_Test2(void)
{
	uint32_t TestData[20];
	uint32_t TestData2[20] = {0};
	

	for(uint8_t i=0;i<20;i++)
	{
		TestData[i] = i;
	}

	InternalFlash_EraseWriteData(FLASH_USER_START_ADDR,TestData,20);
	InternalFlash_ReadData(FLASH_USER_START_ADDR,TestData2,20);

}



