#include "board.h"


FATFS ExtFlash_fs;           /* FatFs�����豸���� */
FIL fnew;           /* �ļ����� */
FRESULT res_flash;  /* ���ؽ�� */
UINT fnum;          /* �ļ��ɹ���д���� */
BYTE ReadBuffer[1024] = {0};
BYTE WriteBuffer[] = "murongshutiao,I love you,fengzhangyi,I love you!";





static void Fatfs_ExtFlash_WriteFile_Test(void)
{
    /* ���ļ�������������򴴽� */
	res_flash = f_open(&fnew, "1:fatfsfile.txt",FA_CREATE_ALWAYS | FA_WRITE );

	if (res_flash == FR_OK)
	{
        /* ���ļ�fnewд������*/
		res_flash = f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);

        if(res_flash == FR_OK)
        {
            /* д��ɹ� */
        }
        else
        {
            /* д��ʧ�� */
        }    

        /* �ر��ļ� */
        f_close(&fnew);
	}
	else
	{	
		LED_RED;
		/* ��/�����ļ�ʧ�� */
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
			/* �ļ���ȡ�ɹ� */
		}
		else
		{
			/* �ļ���ȡʧ�� */
		}		
	}
	else
	{
		/* �ļ���ʧ�� */
	}

	
	f_close(&fnew);	/* ���ٶ�д���ر��ļ� */
}

/* Fatfs��ʽ����д���� */
static void Fatfs_ExtFlash_FormatReadWrite_Test(void)
{
	res_flash = f_open(&fnew, "1:FatFs��д�����ļ�.txt",
								FA_OPEN_ALWAYS|FA_WRITE|FA_READ );

	if (res_flash == FR_OK)
	{
		/*  �ļ���λ������ȷ���ļ�д���λ�� */
		res_flash = f_lseek(&fnew,f_size(&fnew));/* �ļ����󣬵�ǰ�ļ���С */

		if (res_flash == FR_OK)
		{
			/* ��ʽ��д�룬������ʽ����printf���� */
			f_printf(&fnew,"\n��ԭ���ļ������һ������\n");

			res_flash = f_lseek(&fnew,0);	/* �ļ���λ���ļ���ʼλ�� */
			res_flash = f_read(&fnew,ReadBuffer,f_size(&fnew),&fnum);/* ��ȡ�ļ��������ݵ������� */

			if(res_flash == FR_OK)
			{
				/* ��ʽ�����ɹ� */
			}
		}
		f_close(&fnew); 
	}
	else
	{
		/* ���ļ�ʧ�� */
	}
}


void Fatfs_ExtFlash_GetMsg_Test(void)
{

	FATFS *pfs;
	DWORD fre_clust, fre_sect, tot_sect;
	
	/* ��ȡ�豸��Ϣ�Ϳմش�С,�̷������д�(����)��,����ָ���ļ�ϵͳ����ָ�� */
	res_flash = f_getfree("1:", &fre_clust, &pfs);

	/* ����õ��ܵ����������Ϳ�������������λ���� */
	tot_sect = (pfs->n_fatent - 2) * pfs->csize;
	fre_sect = fre_clust * pfs->csize;
}
	
/* Ŀ¼���������������ܲ��� */
void Fatfs_ExtFlash_DirOperation_Test(void)
{
	DIR dir;
	
	res_flash = f_opendir(&dir,"1:TestDir");	/* ���Դ�Ŀ¼ */

	if(res_flash != FR_OK)
	{
		res_flash = f_mkdir("1:TestDir");	/* ��Ŀ¼ʧ�ܣ��ʹ���Ŀ¼ */
	}
	else
	{
		
		res_flash = f_closedir(&dir);/* ���Ŀ¼�Ѿ����ڣ��ر��� */
		
		f_unlink("1:TestDir");		/* ɾ��Ŀ¼/�ļ� */
	}

	if(res_flash == FR_OK)
	{
		/* ���������ƶ��ļ� */
		res_flash = f_rename("1:FatFs��д�����ļ�.txt","1:TestDir/testdir.txt");      
	} 

}


/* �ļ�ϵͳ�ⲿflash���� */
void Fatfs_ExtFlash_Test(void)
{
	
	res_flash = f_mount(&ExtFlash_fs,"1:",1);    /* �����ⲿflash,�̷�1���������� */

	if(res_flash == FR_NO_FILESYSTEM)
	{
        /* û���ļ�ϵͳ */
		res_flash = f_mkfs("1:",0,0);	/* ��ʽ���ⲿflash���̷�1���豸ΪӲ�̣�ͨ��disk_ioctl��ȡ��С */						
		
		if(res_flash == FR_OK)
		{
			res_flash = f_mount(NULL,"1:",1);		/* ȡ������ */			
			res_flash = f_mount(&ExtFlash_fs,"1:",1);        /* ���¹��� */
		}
		else
		{
            /* ��ʽ��ʧ�� */
		}
	}
    else if(res_flash != FR_OK)
    {
        /* ����ϵͳʧ�� */
    }
    else
    {
        /* ����ϵͳ�ɹ� */
		Fatfs_ExtFlash_WriteFile_Test();
		Fatfs_ExtFlash_ReadFile_Test();

		f_mount(NULL,"1:",1);	/* ����ʹ�ã�ȡ������ */
    }

}


/**
  * @brief  scan_files �ݹ�ɨ��FatFs�ڵ��ļ�
  * @param  path:��ʼɨ��·��
  * @retval result:�ļ�ϵͳ�ķ���ֵ
  */
static FRESULT scan_files (char* path) 
{ 
	FRESULT res; 		//�����ڵݹ���̱��޸ĵı���������ȫ�ֱ���	
	FILINFO fno; 
	DIR dir; 
	int i;            
	char *fn;        // �ļ���	
	

#if _USE_LFN /* ���ļ���֧�� */
	/* ����������Ҫ2���ֽڱ���һ�����֡�*/
	static char lfn[_MAX_LFN * 2 + 1]; 	
	fno.lfname = lfn; 
	fno.lfsize = sizeof(lfn); 
#endif 

	res = f_opendir(&dir, path); 	/* ��Ŀ¼ */
	if (res == FR_OK) 
	{ 
		i = strlen(path); 

		for (;;) 
		{ 
			//��ȡĿ¼�µ����ݣ��ٶ����Զ�����һ���ļ�
			res = f_readdir(&dir, &fno); 								
			//Ϊ��ʱ��ʾ������Ŀ��ȡ��ϣ�����
			if (res != FR_OK || fno.fname[0] == 0) break; 	
#if _USE_LFN 
      		fn = *fno.lfname ? fno.lfname : fno.fname; 
#else 
      		fn = fno.fname; 
#endif 
			//���ʾ��ǰĿ¼������			
			if (*fn == '.') continue; 	
			//Ŀ¼���ݹ��ȡ      
			if (fno.fattrib & AM_DIR)         
			{ 			
				//�ϳ�����Ŀ¼��        
				sprintf(&path[i], "/%s", fn); 		
				//�ݹ����         
				res = scan_files(path);	
				path[i] = 0;         
				//��ʧ�ܣ�����ѭ��        
				if (res != FR_OK) 
					break; 
      } 
			else 
			{ 
				printf("%s/%s\r\n", path, fn);								//����ļ���	
        /* ������������ȡ�ض���ʽ���ļ�·�� */        
      }//else
    } //for
  } 
  return res; 
}








