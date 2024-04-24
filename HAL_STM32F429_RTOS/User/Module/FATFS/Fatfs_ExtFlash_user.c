#include "board.h"


FATFS ExtFlash_fs;           /* FatFs物理设备对象 */
FIL fnew;           /* 文件对象 */
FRESULT res_flash;  /* 返回结果 */
UINT fnum;          /* 文件成功读写数量 */
BYTE ReadBuffer[1024] = {0};
BYTE WriteBuffer[] = "murongshutiao,I love you,fengzhangyi,I love you!";





static void Fatfs_ExtFlash_WriteFile_Test(void)
{
    /* 打开文件，如果不存在则创建 */
	res_flash = f_open(&fnew, "1:fatfsfile.txt",FA_CREATE_ALWAYS | FA_WRITE );

	if (res_flash == FR_OK)
	{
        /* 向文件fnew写入内容*/
		res_flash = f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);

        if(res_flash == FR_OK)
        {
            /* 写入成功 */
        }
        else
        {
            /* 写入失败 */
        }    

        /* 关闭文件 */
        f_close(&fnew);
	}
	else
	{	
		LED_RED;
		/* 打开/创建文件失败 */
	}
}


static void Fatfs_ExtFlash_ReadFile_Test(void)
{
	res_flash = f_open(&fnew, "1:fatfsfile.txt", FA_OPEN_EXISTING | FA_READ); 	 

	if(res_flash == FR_OK)
	{

		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 

		if(res_flash == FR_OK)
		{
			/* 文件读取成功 */
		}
		else
		{
			/* 文件读取失败 */
		}		
	}
	else
	{
		/* 文件打开失败 */
	}

	
	f_close(&fnew);	/* 不再读写，关闭文件 */
}

/* Fatfs格式化读写测试 */
static void Fatfs_ExtFlash_FormatReadWrite_Test(void)
{
	res_flash = f_open(&fnew, "1:FatFs读写测试文件.txt",
								FA_OPEN_ALWAYS|FA_WRITE|FA_READ );

	if (res_flash == FR_OK)
	{
		/*  文件定位，用来确定文件写入的位置 */
		res_flash = f_lseek(&fnew,f_size(&fnew));/* 文件对象，当前文件大小 */

		if (res_flash == FR_OK)
		{
			/* 格式化写入，参数格式类似printf函数 */
			f_printf(&fnew,"\n在原来文件新添加一行内容\n");

			res_flash = f_lseek(&fnew,0);	/* 文件定位到文件起始位置 */
			res_flash = f_read(&fnew,ReadBuffer,f_size(&fnew),&fnum);/* 读取文件所有内容到缓存区 */

			if(res_flash == FR_OK)
			{
				/* 格式化读成功 */
			}
		}
		f_close(&fnew); 
	}
	else
	{
		/* 打开文件失败 */
	}
}


void Fatfs_ExtFlash_GetMsg_Test(void)
{

	FATFS *pfs;
	DWORD fre_clust, fre_sect, tot_sect;
	
	/* 获取设备信息和空簇大小,盘符，空闲簇(扇区)数,返回指向文件系统对象指针 */
	res_flash = f_getfree("1:", &fre_clust, &pfs);

	/* 计算得到总的扇区个数和空扇区个数，单位：字 */
	tot_sect = (pfs->n_fatent - 2) * pfs->csize;
	fre_sect = fre_clust * pfs->csize;
}
	
/* 目录创建和重命名功能测试 */
void Fatfs_ExtFlash_DirOperation_Test(void)
{
	DIR dir;
	
	res_flash = f_opendir(&dir,"1:TestDir");	/* 尝试打开目录 */

	if(res_flash != FR_OK)
	{
		res_flash = f_mkdir("1:TestDir");	/* 打开目录失败，就创建目录 */
	}
	else
	{
		
		res_flash = f_closedir(&dir);/* 如果目录已经存在，关闭它 */
		
		f_unlink("1:TestDir");		/* 删除目录/文件 */
	}

	if(res_flash == FR_OK)
	{
		/* 重命名并移动文件 */
		res_flash = f_rename("1:FatFs读写测试文件.txt","1:TestDir/testdir.txt");      
	} 

}


/* 文件系统外部flash测试 */
void Fatfs_ExtFlash_Test(void)
{
	
	res_flash = f_mount(&ExtFlash_fs,"1:",1);    /* 挂载外部flash,盘符1，立即挂载 */

	if(res_flash == FR_NO_FILESYSTEM)
	{
        /* 没有文件系统 */
		res_flash = f_mkfs("1:",0,0);	/* 格式化外部flash，盘符1，设备为硬盘，通过disk_ioctl获取大小 */						
		
		if(res_flash == FR_OK)
		{
			res_flash = f_mount(NULL,"1:",1);		/* 取消挂载 */			
			res_flash = f_mount(&ExtFlash_fs,"1:",1);        /* 重新挂载 */
		}
		else
		{
            /* 格式化失败 */
		}
	}
    else if(res_flash != FR_OK)
    {
        /* 挂载系统失败 */
    }
    else
    {
        /* 挂载系统成功 */
		Fatfs_ExtFlash_WriteFile_Test();
		Fatfs_ExtFlash_ReadFile_Test();

		f_mount(NULL,"1:",1);	/* 不再使用，取消挂载 */
    }

}


/**
  * @brief  scan_files 递归扫描FatFs内的文件
  * @param  path:初始扫描路径
  * @retval result:文件系统的返回值
  */
static FRESULT scan_files (char* path) 
{ 
	FRESULT res; 		//部分在递归过程被修改的变量，不用全局变量	
	FILINFO fno; 
	DIR dir; 
	int i;            
	char *fn;        // 文件名	
	

#if _USE_LFN /* 长文件名支持 */
	/* 简体中文需要2个字节保存一个“字”*/
	static char lfn[_MAX_LFN * 2 + 1]; 	
	fno.lfname = lfn; 
	fno.lfsize = sizeof(lfn); 
#endif 

	res = f_opendir(&dir, path); 	/* 打开目录 */
	if (res == FR_OK) 
	{ 
		i = strlen(path); 

		for (;;) 
		{ 
			//读取目录下的内容，再读会自动读下一个文件
			res = f_readdir(&dir, &fno); 								
			//为空时表示所有项目读取完毕，跳出
			if (res != FR_OK || fno.fname[0] == 0) break; 	
#if _USE_LFN 
      		fn = *fno.lfname ? fno.lfname : fno.fname; 
#else 
      		fn = fno.fname; 
#endif 
			//点表示当前目录，跳过			
			if (*fn == '.') continue; 	
			//目录，递归读取      
			if (fno.fattrib & AM_DIR)         
			{ 			
				//合成完整目录名        
				sprintf(&path[i], "/%s", fn); 		
				//递归遍历         
				res = scan_files(path);	
				path[i] = 0;         
				//打开失败，跳出循环        
				if (res != FR_OK) 
					break; 
      } 
			else 
			{ 
				printf("%s/%s\r\n", path, fn);								//输出文件名	
        /* 可以在这里提取特定格式的文件路径 */        
      }//else
    } //for
  } 
  return res; 
}








