/**
  ******************************************************************************
  * @file    BSP/Src/sd.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the SD Driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "./sdio/sdio_test.h"
/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BLOCK_START_ADDR         0     /* Block start address      */
#define NUM_OF_BLOCKS            5   /* Total number of blocks   */
#define BUFFER_WORDS_SIZE        ((BLOCKSIZE * NUM_OF_BLOCKS) >> 2) /* Total data size in bytes */
    
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t aTxBuffer[BUFFER_WORDS_SIZE];
uint32_t aRxBuffer[BUFFER_WORDS_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  SD Demo
  * @param  None
  * @retval None
  */
void SD_Test(void)
{ 
  uint8_t SD_state = MSD_OK;
  
  SD_state = BSP_SD_Init();
  
  if(SD_state != MSD_OK)
  {
    printf("SD Initialization : FAIL.\r\n");	
  }		
   else
   {
    printf("SD Initialization :  OK.\r\n");		

    SD_state = BSP_SD_Erase(BLOCK_START_ADDR, NUM_OF_BLOCKS);
    /* Wait until SD cards are ready to use for new operation */
    while((BSP_SD_GetCardState() != SD_TRANSFER_OK))
    {
    }
    if(SD_state != MSD_OK)
    {
       printf("SD ERASE : FAILED.\r\n");  
    }
    else
    {
       printf("SD ERASE : OK.\r\n"); 
      /* Fill the buffer to write */
      Fill_Buffer(aTxBuffer, BUFFER_WORDS_SIZE, 0x22FF);
      SD_state = BSP_SD_WriteBlocks_DMA(aTxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
      /* Wait until SD cards are ready to use for new operation */
      while((BSP_SD_GetCardState() != SD_TRANSFER_OK))
      {
      }                               
      if(SD_state != MSD_OK)
      {
        printf("SD WRITE : FAILED.\r\n");
      }
      else
      {
        printf("SD WRITE : OK.\r\n");
        SD_state = BSP_SD_ReadBlocks_DMA(aRxBuffer, BLOCK_START_ADDR, NUM_OF_BLOCKS);
        /* Wait until SD cards are ready to use for new operation */
        while((BSP_SD_GetCardState() != SD_TRANSFER_OK))
        {
        }
        if(SD_state != MSD_OK)
        {
          printf("SD READ  : FAILED.\r\n");
        }
        else
        {
          printf("SD READ  : OK.\r\n");
          if(Buffercmp(aTxBuffer, aRxBuffer, BUFFER_WORDS_SIZE) > 0)
          {
            printf("SD COMPARE  : FAILED.\r\n");
          }
          else
          {
            printf("SD COMPARE  : OK.\r\n");
          }
        }
      }
    }
  } 
}

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLenght: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;
  
  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
  */
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }
    
    pBuffer1++;
    pBuffer2++;
  }
  return 0;
}


/**
	**************************************************************
	* Description : ��ʼ��WiFiģ��ʹ�����ţ�������WiFiģ��
	* Argument(s) : none.
	* Return(s)   : none.
	**************************************************************
	*/
void WIFI_PDN_INIT(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStruct;
	/*ʹ������ʱ��*/	
	__HAL_RCC_GPIOG_CLK_ENABLE();
	/*ѡ��Ҫ���Ƶ�GPIO����*/															   
	GPIO_InitStruct.Pin = GPIO_PIN_9;	
	/*�������ŵ��������Ϊ�������*/
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;      
	/*��������Ϊ����ģʽ*/
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	/*������������Ϊ���� */   
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST; 
	/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);	
	/*����WiFiģ��*/
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_RESET);  
}

static volatile DSTATUS Stat = STA_NOINIT;
extern SD_HandleTypeDef uSdHandle;
//���ͱ�־λ
extern volatile uint8_t TX_Flag;
//���ܱ�־λ
extern volatile uint8_t RX_Flag; 


DSTATUS SD_initialize(void)
{
    Stat = STA_NOINIT;
    if(BSP_SD_Init() == HAL_OK)
    {    
        Stat &= ~STA_NOINIT;
    }
    return Stat;
}



DSTATUS SD_status(void)
{

    Stat = STA_NOINIT;
    if(HAL_SD_GetCardState(&uSdHandle) == HAL_SD_CARD_TRANSFER)
    {
        Stat &= ~STA_NOINIT;
    }
    return Stat;
}



DRESULT SD_read(
                BYTE *buff,//���ݻ����� 
                DWORD sector, //�����׵�ַ
                UINT count)//��������(1..128)
{
  DRESULT res = RES_ERROR;
  uint32_t timeout;

  RX_Flag = 0;
  
  if(HAL_SD_ReadBlocks_DMA(&uSdHandle, (uint8_t*)buff,
                           (uint32_t) (sector),
                           count) == HAL_OK)
  {
    /* Wait that the reading process is completed or a timeout occurs */
    timeout = HAL_GetTick();
    while((RX_Flag == 0) && ((HAL_GetTick() - timeout) < SD_TIMEOUT))
    {
    }
    /* incase of a timeout return error */
    if (RX_Flag == 0)
    {
      res = RES_ERROR;
    }
    else
    {
      RX_Flag = 0;
      timeout = HAL_GetTick();

      while((HAL_GetTick() - timeout) < SD_TIMEOUT)
      {
        if (HAL_SD_GetCardState(&uSdHandle) == HAL_SD_CARD_TRANSFER)
        {
          res = RES_OK;
           break;
        }
      }
    }
  }

  return res;
}


  
DRESULT SD_write(BYTE lun,//��������������豸ʱ�õ�(0...)
                 const BYTE *buff,//���ݻ����� 
                 DWORD sector, //�����׵�ַ
                 UINT count)//��������(1..128)
{
    DRESULT res = RES_ERROR;
    uint32_t timeout;
  
    TX_Flag = 0;
    //������Ӧ��DCache
    if(HAL_SD_WriteBlocks_DMA(&uSdHandle, (uint8_t*)buff,
                             (uint32_t) (sector),
                             count) == HAL_OK)
    {
      /* Wait that the reading process is completed or a timeout occurs */
      timeout = HAL_GetTick();
      while((TX_Flag == 0) && ((HAL_GetTick() - timeout) < SD_TIMEOUT))
      {
      }
      /* incase of a timeout return error */
      if (TX_Flag == 0)
      {
        res = RES_ERROR;
      }
      else
      {
        TX_Flag = 0;
        timeout = HAL_GetTick();

        while((HAL_GetTick() - timeout) < SD_TIMEOUT)
        {
          if (HAL_SD_GetCardState(&uSdHandle) == HAL_SD_CARD_TRANSFER)
          {
            res = RES_OK;
            break;
          }
        }
      }
    }
    return res;
}



DRESULT SD_ioctl(BYTE lun,BYTE cmd, void *buff)
{
    DRESULT res = RES_ERROR;
    HAL_SD_CardInfoTypeDef CardInfo;

    if (Stat & STA_NOINIT) return RES_NOTRDY;

    switch (cmd)
    {
    /* Make sure that no pending write process */
    case CTRL_SYNC :
      res = RES_OK;
      break;

    /* Get number of sectors on the disk (DWORD) */
    case GET_SECTOR_COUNT :
      HAL_SD_GetCardInfo(&uSdHandle, &CardInfo);
      *(DWORD*)buff = CardInfo.LogBlockNbr;
      res = RES_OK;
      break;

    /* Get R/W sector size (WORD) */
    case GET_SECTOR_SIZE :
      HAL_SD_GetCardInfo(&uSdHandle, &CardInfo);
      *(WORD*)buff = CardInfo.LogBlockSize;
      res = RES_OK;
      break;

    /* Get erase block size in unit of sector (DWORD) */
    case GET_BLOCK_SIZE :
      HAL_SD_GetCardInfo(&uSdHandle, &CardInfo);
      *(DWORD*)buff = CardInfo.LogBlockSize / BLOCK_SIZE;
      res = RES_OK;
      break;

    default:
      res = RES_PARERR;
    }
    return res;//RES_OK;
}




FATFS SDCard_fs;           /* FatFs�����豸���� */
FIL SDCard_fnew;           /* �ļ����� */
FRESULT res_sd;  /* ���ؽ�� */
UINT SDCard_fnum;          /* �ļ��ɹ���д���� */
BYTE SDCard_ReadBuffer[1024] = {0};
BYTE SDCard_WriteBuffer[] = "murongshutiao,I love you,fengzhangyi,I love you!";

void Fatfs_SDCard_Test(void)
{
       //���ⲿSD�������ļ�ϵͳ���ļ�ϵͳ����ʱ���SD����ʼ��
   res_sd = f_mount(&SDCard_fs,"0:",1);  


   /*----------------------- ��ʽ������ ---------------------------*/  
   /* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
   if(res_sd == FR_NO_FILESYSTEM)
   {
      printf("��SD����û���ļ�ϵͳ���������и�ʽ��...\r\n");
      /* ��ʽ�� */
      res_sd=f_mkfs("0:",0,0);							

      if(res_sd == FR_OK)
      {
        printf("��SD���ѳɹ���ʽ���ļ�ϵͳ��\r\n");
        /* ��ʽ������ȡ������ */
        res_sd = f_mount(NULL,"0:",1);			
        /* ���¹���	*/			
        res_sd = f_mount(&SDCard_fs,"0:",1);
      }
      else
      {
        LED_RED;
        printf("������ʽ��ʧ�ܡ�����\r\n");
        while(1);
      }
   }
   else if(res_sd!=FR_OK)
   {
      printf("����SD�������ļ�ϵͳʧ�ܡ�(%d)\r\n",res_sd);
      printf("��������ԭ��SD����ʼ�����ɹ���\r\n");
      while(1);
   }
   else
   {
      printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
   }	
    /*----------------------- �ļ�ϵͳ���ԣ�д���� -----------------------------*/
    /* ���ļ�������ļ��������򴴽��� */
    printf("\r\n****** ���������ļ�д�����... ******\r\n");	
    res_sd = f_open(&SDCard_fnew, "0:FatFs��д�����ļ�.txt",FA_CREATE_ALWAYS | FA_WRITE );
    if ( res_sd == FR_OK )
    {
      printf("����/����FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
      /* ��ָ���洢������д�뵽�ļ��� */
      res_sd=f_write(&SDCard_fnew,SDCard_WriteBuffer,sizeof(SDCard_WriteBuffer),&SDCard_fnum);
      if(res_sd==FR_OK)
      {
        printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",SDCard_fnum);
        printf("�����ļ�д�������Ϊ��\r\n%s\r\n",SDCard_WriteBuffer);
      }
      else
      {
        printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_sd);
      }    
      /* ���ٶ�д���ر��ļ� */
      f_close(&SDCard_fnew);
    }
    else
    {	
      LED_RED;
      printf("������/�����ļ�ʧ�ܡ�\r\n");
    }
    
  /*------------------- �ļ�ϵͳ���ԣ������� ------------------------------------*/
    printf("****** ���������ļ���ȡ����... ******\r\n");
    res_sd = f_open(&SDCard_fnew, "0:FatFs��д�����ļ�.txt", FA_OPEN_EXISTING | FA_READ); 	 
    if(res_sd == FR_OK)
    {
      LED_GREEN;
      printf("�����ļ��ɹ���\r\n");
      res_sd = f_read(&SDCard_fnew, SDCard_ReadBuffer, sizeof(SDCard_ReadBuffer), &SDCard_fnum); 
      if(res_sd==FR_OK)
      {
        printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",SDCard_fnum);
        printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", SDCard_ReadBuffer);	
      }
      else
      {
        printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_sd);
      }		
    }
    else
    {
      LED_RED;
      printf("�������ļ�ʧ�ܡ�\r\n");
    }
}









/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
