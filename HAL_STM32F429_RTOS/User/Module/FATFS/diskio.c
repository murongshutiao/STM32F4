/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "ff.h"
#include "board.h"




/*-----------------------------------------------------------------------*/
/* 获取设备状态                                                          */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (BYTE pdrv)		/* 物理编号 */
{

	DSTATUS status = STA_NOINIT;
	
	switch (pdrv) 
	{
		case ATA:	/* SD CARD */
			status = SD_status();
			break;
    
		case SPI_FLASH:      
			/* SPI Flash状态检测：读取SPI Flash 设备ID */
			if(sFLASH_ID == SPI_FLASH_ReadID())
			{
				/* 设备ID读取结果正确 */
				status &= ~STA_NOINIT;
			}
			else
			{
				/* 设备ID读取结果错误 */
				status = STA_NOINIT;;
			}
			break;

		case INTERNAL_FLASH:	/* SD CARD */
			break;
		default:status = STA_NOINIT;
	}
	return status;
}


/*-----------------------------------------------------------------------*/
/* 设备初始化                                                            */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (BYTE pdrv)				/* 物理编号 */
{
  uint16_t i;
	DSTATUS status = STA_NOINIT;	

	switch (pdrv) 
	{
		case ATA:	         /* SD CARD */
			status = SD_initialize();
			break;
    
		case SPI_FLASH:    /* SPI Flash */ 
			/* 初始化SPI Flash */
			Flash_SPI_Init();
			
			/* 延时一小段时间 */
			i=500;
			while(--i);	
			/* 唤醒SPI Flash */
			SPI_Flash_WAKEUP();
			/* 获取SPI Flash芯片状态 */
			status=disk_status(SPI_FLASH);
				break;
      
		default:
			status = STA_NOINIT;
	}
	return status;
}


/*-----------------------------------------------------------------------*/
/* 读扇区：读取扇区内容到指定存储区                                              */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
	BYTE pdrv,		/* 设备物理编号(0..) */
	BYTE *buff,		/* 数据缓存区 */
	DWORD sector,	/* 扇区首地址 */
	UINT count		/* 扇区个数(1..128) */
)
{
	DRESULT status = RES_PARERR;
	switch (pdrv) 
	{
		case ATA:	/* SD CARD */
			status = SD_read(buff,sector,count);
			break;
    
		case SPI_FLASH:
		/* 扇区偏移6MB，外部Flash文件系统空间放在SPI Flash后面10MB空间 */
		sector += 1536;      
		SPI_FLASH_BufferRead(buff, sector << 12, count << 12);
		status = RES_OK;
		break;
    
		default:
			status = RES_PARERR;
	}
	return status;
}

/*-----------------------------------------------------------------------*/
/* 写扇区：见数据写入指定扇区空间上                                      */
/*-----------------------------------------------------------------------*/
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			  /* 设备物理编号(0..) */
	const BYTE *buff,		/* 欲写入数据的缓存区 */
	DWORD sector,		  /* 扇区首地址 */
	UINT count			  /* 扇区个数(1..128) */
)
{
  	uint32_t write_addr; 
	DRESULT status = RES_PARERR;
	
	if (!count) 
	{
		return RES_PARERR;		/* Check parameter */
	}

	switch (pdrv) 
	{
		case ATA:	/* SD CARD */ 
		SD_write((uint8_t *)buff,sector,count);     
		break;

		case SPI_FLASH:
		/* 扇区偏移6MB，外部Flash文件系统空间放在SPI Flash后面10MB空间 */
		sector+=1536;
		write_addr = sector<<12;    
		SPI_FLASH_SectorErase(write_addr);
		SPI_FLASH_BufferWrite((uint8_t *)buff,write_addr,count<<12);
		status = RES_OK;
		break;
    
		default:
			status = RES_PARERR;
	}
	return status;
}
#endif


/*-----------------------------------------------------------------------*/
/* 其他控制                                                              */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* 物理编号 */
	BYTE cmd,		  /* 控制指令 */
	void *buff		/* 写入或者读取数据地址指针 */
)
{
	DRESULT status = RES_PARERR;
	switch (pdrv) 
	{
		case ATA:	/* SD CARD */
			switch (cmd) 
			{
				/* 扇区数量：2560*4096/1024/1024=10(MB) */
				case GET_SECTOR_COUNT:*(DWORD * )buff = 2560;break;
				/* 扇区大小  */
				case GET_SECTOR_SIZE :*(WORD * )buff = 512;break;
				/* 同时擦除扇区个数 */
				case GET_BLOCK_SIZE :*(DWORD * )buff = 1;break;        
      		}
      		status = RES_OK;
			break;
    
		case SPI_FLASH:
			switch (cmd) 
			{
				/* 扇区数量：2560*4096/1024/1024=10(MB) */
				case GET_SECTOR_COUNT:*(DWORD * )buff = 2560;break;
				/* 扇区大小  */
				case GET_SECTOR_SIZE :*(WORD * )buff = 4096;break;
				/* 同时擦除扇区个数 */
				case GET_BLOCK_SIZE :*(DWORD * )buff = 1;break;        
      		}
      		status = RES_OK;
			break;
    
		default:
			status = RES_PARERR;
	}
	return status;
}
#endif


__weak DWORD get_fattime(void) 
{
	/* 返回当前时间戳 */
	return	  ((DWORD)(2015 - 1980) << 25)	/* Year - bit31:25 */
			| ((DWORD)1 << 21)				/* Month - bit24:21 */
			| ((DWORD)1 << 16)				/* Mday - bit20:16 */
			| ((DWORD)0 << 11)				/* Hour - bit15:11 */
			| ((DWORD)0 << 5)				/* Min - bit10:5 */
			| ((DWORD)0 >> 1);				/* Sec - bit4:0 */
}





