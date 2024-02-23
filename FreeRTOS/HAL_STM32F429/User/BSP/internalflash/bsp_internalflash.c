
  
#include "./internalflash/bsp_internalflash.h"   

/*׼��д��Ĳ�������*/
#define DATA_32                 ((uint32_t)0x87654321)


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Ҫ�����ڲ�FLASH����ʼ��ַ */
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_5   
/* Ҫ�����ڲ�FLASH�Ľ�����ַ */
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_7  


static uint32_t GetSector(uint32_t Address);

/**
  * @brief  InternalFlash_Test,���ڲ�FLASH���ж�д����
  * @param  None
  * @retval None
  */
int InternalFlash_Test(void)
{
	/*Ҫ��������ʼ����(����)����������(������)����8-12����ʾ����8��9��10��11����*/
	uint32_t FirstSector = 0;
	uint32_t NbOfSectors = 0;
	uint32_t SECTORError = 0;	/* ��д���ؽ�� */
	
	uint32_t Address = 0;

	__IO uint32_t Data32 = 0;
	__IO uint32_t MemoryProgramStatus = 0;
	static FLASH_EraseInitTypeDef EraseInitStruct;
	
	HAL_FLASH_Unlock();

	FirstSector = GetSector(FLASH_USER_START_ADDR);	/* ��ȡ������ */
	NbOfSectors = GetSector(FLASH_USER_END_ADDR)- FirstSector + 1;	/* ��ȡ�������� */
	
	/* �����û����� (�û�����ָ������û��ʹ�õĿռ䣬�����Զ���)**/
	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;	/* ��д����:���� */
	EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;		/* �ԡ��֡��Ĵ�С���в��� */ 
	EraseInitStruct.Sector        = FirstSector;				/* ��д�������� */
	EraseInitStruct.NbSectors     = NbOfSectors;				/* ��д���������� */
	/* ��ʼ�������� */
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	{
		/*�����������أ�ʵ��Ӧ���пɼ��봦�� */
		return -1;
	}

	/* �ԡ��֡��Ĵ�СΪ��λд������ ********************************/
	Address = FLASH_USER_START_ADDR;

	while (Address < FLASH_USER_END_ADDR)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, DATA_32) == HAL_OK)
		{
		  Address = Address + 4;
		}
		else
		{ 
		  /*д��������أ�ʵ��Ӧ���пɼ��봦�� */
				return -1;
		}
	}


	/* ��FLASH��������ֹ���ݱ��۸�*/
	HAL_FLASH_Lock(); 


	/* ��FLASH�ж�ȡ�����ݽ���У��***************************************/
	/*  MemoryProgramStatus = 0: д���������ȷ
	  MemoryProgramStatus != 0: д������ݴ�����ֵΪ����ĸ��� */
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
	/* ����У�鲻��ȷ */
	if(MemoryProgramStatus)
	{    
		return -1;
	}
	else /*����У����ȷ*/
	{ 
		return 0;   
	}
}



/**
  * @brief  ��������ĵ�ַ���������ڵ�sector
  *					���磺
						uwStartSector = GetSector(FLASH_USER_START_ADDR);
						uwEndSector = GetSector(FLASH_USER_END_ADDR);	
  * @param  Address����ַ
  * @retval ��ַ���ڵ�sector
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


/* ʹ�ô˺���Ҫ�Ƚ��� */
int8_t InternalFlashEraseSector(uint32_t EraseStartAddr,uint32_t EraseEndAddr)
{
	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t SECTORError = 0;
	uint32_t NbOfSectors = 0;
	uint32_t FirstSector = 0;	/* ��һ������ */
	
	FirstSector = GetSector(EraseStartAddr);	/* ��ȡ������ */
	NbOfSectors = GetSector(EraseEndAddr) - FirstSector + 1;	/* ��ȡ�������� */

	EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;	/* ��д����:���� */
	EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;		/* �ԡ��֡��Ĵ�С���в��� */ 
	EraseInitStruct.Sector        = FirstSector;				/* ��д�������� */
	EraseInitStruct.NbSectors     = NbOfSectors;				/* ��д���������� */

	/* ��ʼ�������� */
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	{
		/*�����������أ�ʵ��Ӧ���пɼ��봦�� */
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
		  	/*д��������أ�ʵ��Ӧ���пɼ��봦�� */
			return -1;
		}
	}

	/* ��FLASH��������ֹ���ݱ��۸�*/
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
		  	/*д��������أ�ʵ��Ӧ���пɼ��봦�� */
			return -1;
		}
	}

	/* ��FLASH��������ֹ���ݱ��۸�*/
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



