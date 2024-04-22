
/*------------------------------------------˵��:------------------------------------------------
�������Ѿ������벢������֤,������֤��������LedLight.c��ϸ˵��,�˴���
��һ�����ý���˵��:

1������֤ʱUSE_OS�ж���ʹ����FreeRTOS,��˲������¶���
	#if		USE_OS==OS_FREERTOS
	#include "FreeRTOS.h"
	#endif

2��LLED������Ҫ�߱����¹���:
	1)	ͨ�����ó�ʼ�����ṩ�������,����Ҫ�漰2������:
		A��pollָ�뺯��,�䱻Ӧ�ò��ڶ�ʱɨ��λ�õ���;
		B,	ctrlָ�뺯��,�䱻Ӧ�ò���Ҫ����LED��ʱ����;
		˵��: ����ctrl�ĵ��ò���˵��,���LLedDriver.h�ж�������˵������.
		
	2)	����������Ҫʵ��LED�ƵĶ���ģʽ���Ʋ���:
		A,	����/Ϩ�����;
		B,	��ͨ��˸���Ʋ���;
		C,	��Ъ����˸���Ʋ���;
		���ڼ�Ъ����˸���Ʋ���,���LLedDriver.h�ж�������˵������.
--------------------------------------------------------------------------------------------------
ע:	�������Ҫ,�ڿ��ǽ�Լ�ڴ�ʱ,���Կ��ǽ����������������Ϊָ�벢����Ҫִ��ʱ�������ڴ�,
	����ִ����Ϻ��ͷ��ڴ档Ŀǰ��ʱ�����޸ġ�
	tLLedDriver�ṹ��led�ڵ����±�����
	tLLedFlashRun 			run	
	tLLedFlashIntervalParam	flashIntervalParam;	
--------------------------------------------------------------------------------------------------*/
#define	OS_NONE							0
#define	OS_FREERTOS						1
#define	USE_OS							OS_FREERTOS
#if		USE_OS==OS_FREERTOS
#include "FreeRTOS.h"
#endif
#include "./LLedDriver/LLedDriver.h"

//LLED_INTERVAL_SECONDFLASH_DLY������������FIRST FLASH ��SECOND FLASHֱ�ӽ����Եļ��
#define	LLED_INTERVAL_SECONDFLASH_DLY	1
//
#define	LLEDSCAN_UINT_DEF				10						//LED��ɨ��Ĭ��ʱ�䵥λ(ms)
//���¶����ڴ��������ͷ�
#define	MALLOC_MEM(memSize)				pvPortMalloc(memSize)	//�ڴ������
#define	FREE_MEM(pMem)					vPortFree(pMem)			//�ڴ��ͷź�

tLLedDriver	*pLLedDrv=NULL;										//�����������ڲ�������ָ��
//===========================================================================
//LED��������������ģ��
/*--------------------------------------------------------------------------------------------------
*	ժҪ:	ִ��Ӧ�ò��ṩ�Ļ�ȡ�����������������ڲ��Ļ�ȡ���������
*	����:	��
*	����:	��
---------------------------------------------------------------------------------------------------*/
static void LLed_Lock(void)
{
	if	(pLLedDrv==NULL)
	return;
	if	(pLLedDrv->drvFunc.lock!=NULL)
	pLLedDrv->drvFunc.lock();
	else
	{
		while	(pLLedDrv->swLock==LLED_LOCKED);
		pLLedDrv->swLock=LLED_LOCKED;
	}
}
/*--------------------------------------------------------------------------------------------------
*	ժҪ:	ִ��Ӧ�ò��ṩ�Ĺ黹�����������������ڲ��Ĺ黹���������
*	����:	��
*	����:	��
---------------------------------------------------------------------------------------------------*/
static void LLed_UnLock(void)
{
	if	(pLLedDrv==NULL)
	return;
	if	(pLLedDrv->drvFunc.unLock!=NULL)
	pLLedDrv->drvFunc.unLock();
	else
	pLLedDrv->swLock=LLED_UNLOCKED;
}
/*--------------------------------------------------------------------------------------------------
*	ժҪ:	����LED����˸��ز���,������˸Ƶ����ռ�ձȼ��������Ϩ��
			����ֵ,��������˸���������״̬
*	����:	uint8_t light LED�����
*			tLLedFlashParam* pFlashParam��˸����ָ��
*	����:	TRUE ���óɹ�
*			FALSE����ʧ��
---------------------------------------------------------------------------------------------------*/
static bool LLed_FlashParamSetup(uint8_t light,tLLedFlashParam* pFlashParam)
{
	uint16_t	totalCnt;

    totalCnt=pFlashParam->freq*LLED_BASETIME/pLLedDrv->scanUnit;
	//����totalCnt�ļ��������ܡ�1,�����޷�ִ����˸
	if	(totalCnt<=1)
	return	FALSE;
	if	(pFlashParam->dutyRatio<LLED_DUTY_MIN||pFlashParam->dutyRatio>LLED_DUTY_MAX||(pFlashParam->dutyRatio%10)!=0)
	return	FALSE;
    //����ռ�ձȼ�����˸�����ļ�����ֵ
	pLLedDrv->led[light].run.onCntMax=totalCnt*pFlashParam->dutyRatio/100;
	if	(pLLedDrv->led[light].run.onCntMax==0)
	pLLedDrv->led[light].run.onCntMax=1;
   	//������˸Ϩ��ļ�����ֵ
  	pLLedDrv->led[light].run.offCntMax=totalCnt-pLLedDrv->led[light].run.onCntMax;
	pLLedDrv->led[light].run.count=0;
  	pLLedDrv->led[light].run.flashCnt.exec=0;
    pLLedDrv->led[light].run.flashCnt.max=pFlashParam->counter;
	pLLedDrv->led[light].run.endStatus=pFlashParam->endStatus;
	return	TRUE;
}
/*--------------------------------------------------------------------------------------------------
*	ժҪ:	LED����˸������ʱ���,����ʱ�䵽����л�����˸Ϩ��״̬
*	����:	LED�����
*	����:	TRUE �������
*			FALSE���������
---------------------------------------------------------------------------------------------------*/
static bool LLed_FlashOnCheck(uint8_t light)
{
	bool	output=FALSE;
	
	//��˸������ʱ
	pLLedDrv->led[light].run.count++;
	if	(pLLedDrv->led[light].run.count>=pLLedDrv->led[light].run.onCntMax)
	{
		//��ʱ�ﵽ�趨ֵ���л�����˸��״̬
		pLLedDrv->led[light].run.count=0;
		pLLedDrv->led[light].status=LLED_STATUS_OFF;
		if	(pLLedDrv->led[light].run.flashCnt.max!=LLED_FLASHKEEP_CNT)
		pLLedDrv->led[light].run.cntDetect=TRUE;
		output=TRUE;
	}
	return	output;
}
/*--------------------------------------------------------------------------------------------------
*	ժҪ:	LED����˸����ֵ����������Ƿ�ִ�����,����ִ�����ʱ����
*			LED�Ƶ�ָ�������״̬
*	����:	LED�����
*	����:	TRUE  �������
*			FALSE��������� 
*	˵��:	�˺�������LLed_FlashOffCheck�������������ģ��
---------------------------------------------------------------------------------------------------*/
static bool LLed_FlashCountCheck(uint8_t light)
{
	pLLedDrv->led[light].run.cntDetect=FALSE;
	//��˸��������,�������˸����
	pLLedDrv->led[light].run.flashCnt.exec++;
	if	(pLLedDrv->led[light].run.flashCnt.exec>=pLLedDrv->led[light].run.flashCnt.max)
	{
		//��˸�����ﵽ�趨ֵ��������˸
		if	(pLLedDrv->led[light].run.endStatus==LLED_STATUS_ON)
		{
			//ָ������LED��״̬Ϊ����
			pLLedDrv->led[light].status=LLED_STATUS_ON;
			pLLedDrv->led[light].mode=LLED_MODE_BRIGHT;
		}
		else
		{
			//ָ������LED��״̬Ϊ����
			pLLedDrv->led[light].status=LLED_STATUS_OFF;
			pLLedDrv->led[light].mode=LLED_MODE_DARKNESS;
		}
		return	FALSE;
	}
	else//��˸û�н���,����ִ��(��һ��ִ�е���״̬)
	{
		pLLedDrv->led[light].status=LLED_STATUS_ON;
		return	TRUE;
	}
}
/*--------------------------------------------------------------------------------------------------
*	ժҪ:	LED����˸Ϩ���ʱ���,����ʱ�䵽���ִ��LED����˸����ֵ����
*			�����,���ڼ���ִ��ʱ�л�����˸����״̬��
*	����:	LED�����
*	����:	TRUE �������
*			FALSE��������� 
---------------------------------------------------------------------------------------------------*/
static bool LLed_FlashOffCheck(uint8_t light)
{
	bool	output=FALSE;
	
	//��˸Ϩ���ʱ
	pLLedDrv->led[light].run.count++;
	if	(pLLedDrv->led[light].run.count>=pLLedDrv->led[light].run.offCntMax)
	{
		//��ʱ�ﵽ�趨ֵ
		pLLedDrv->led[light].run.count=0;
		if	(pLLedDrv->led[light].run.cntDetect==TRUE)
		output=LLed_FlashCountCheck(light);
		else//������˸,�����ִ��(��һ��ִ�е���״̬)
		{
			pLLedDrv->led[light].status=LLED_STATUS_ON;
			output=TRUE;
		}
	}
	return	output;
}
/*--------------------------------------------------------------------------------------------------
*	ժҪ:	LED�Ƽ�Ъ��˸ִ�н׶��л�,��Ҫ������¼����׶��л�
*			1,FIRST FLASH TO SECOND FLASH(LLED_FLASH_INTERVAL_FIRST --> LLED_FLASH_INTERVAL_SECOND)
*			2,SECOND FLASH TO DARK(LLED_FLASH_INTERVAL_SECOND --> LLED_FLASH_INTERVAL_DARK)
*			3,DARK TO FINISH(LLED_FLASH_INTERVAL_DARK --> LLED_FLASH_INTERVAL_FIN)
*	����:	LED�����
*	����:	�� 
---------------------------------------------------------------------------------------------------*/
static void LLed_FlashIntervalSwitchToNext(uint8_t light)
{
	if	(pLLedDrv->led[light].run.intervalLevel==LLED_FLASH_INTERVAL_FIRST)
	{
		if	(pLLedDrv->led[light].flashIntervalParam.second.counter>0)
		{
			//�л������еĵڶ��׶���˸
			LLed_FlashParamSetup(light,&pLLedDrv->led[light].flashIntervalParam.second);
			#if	LLED_INTERVAL_SECONDFLASH_DLY==1
			//Ϊ������SECOND FLASH��FIRST FLASH֮������Լ��,ʹSECOND FLASH�ӳ�һ�����������ڿ�ʼ
			pLLedDrv->led[light].status=LLED_STATUS_OFF;
			#else
			pLLedDrv->led[light].status=LLED_STATUS_ON;
			pLLedDrv->led[light].trig=LLED_TRIG_ON;
			#endif
			pLLedDrv->led[light].run.intervalLevel=LLED_FLASH_INTERVAL_SECOND;
		}
		else//�ڶ��׶ε�ִ�д���Ϊ0,�������������׶�
		pLLedDrv->led[light].run.intervalLevel=LLED_FLASH_INTERVAL_DARK;
	}
	else//�л������еĵ����׶�Ϩ��
	pLLedDrv->led[light].run.intervalLevel=LLED_FLASH_INTERVAL_DARK;
	//�����׶ε�Ϩ��ʱ��Ϊ0,������ִ��
	if	(pLLedDrv->led[light].run.intervalLevel==LLED_FLASH_INTERVAL_DARK&&pLLedDrv->led[light].flashIntervalParam.darkTime==0)
	pLLedDrv->led[light].run.intervalLevel=LLED_FLASH_INTERVAL_FINISH;
	pLLedDrv->led[light].mode=LLED_MODE_FLASH_INTERVAL;
}
/*--------------------------------------------------------------------------------------------------
*	ժҪ:	LED�Ƽ�Ъ��˸һ��ִ����ɴ���,������˸ִ�����ʱ,�����õ�
*			���ָ�������״̬;�������ִ����һ�μ�Ъ����˸��
*	����:	LED�����
*	����:	�� 
---------------------------------------------------------------------------------------------------*/
static void LLed_FlashIntervalFinish(uint8_t light)
{
	bool	goon=TRUE;
	
	if	(pLLedDrv->led[light].run.intervalCnt.max!=LLED_FLASHKEEP_CNT)
	{
		pLLedDrv->led[light].run.intervalCnt.exec++;
		if	(pLLedDrv->led[light].run.intervalCnt.exec>=pLLedDrv->led[light].run.intervalCnt.max)
		{
			if	(pLLedDrv->led[light].run.endStatus==LLED_STATUS_ON)
			{
				//ָ������LED��״̬Ϊ����
				pLLedDrv->led[light].status=LLED_STATUS_ON;
				pLLedDrv->led[light].mode=LLED_MODE_BRIGHT;
			}
			else
			{
				//ָ������LED��״̬Ϊ����
				pLLedDrv->led[light].status=LLED_STATUS_OFF;
				pLLedDrv->led[light].mode=LLED_MODE_DARKNESS;
			}
			goon=FALSE;
		}
	}
	if	(goon==TRUE)
	{
		LLed_FlashParamSetup(light,&pLLedDrv->led[light].flashIntervalParam.first);
		pLLedDrv->led[light].status=LLED_STATUS_ON;
		pLLedDrv->led[light].trig=LLED_TRIG_ON;
		pLLedDrv->led[light].run.intervalLevel=LLED_FLASH_INTERVAL_FIRST;
	}
}
/*--------------------------------------------------------------------------------------------------
*	ժҪ:	LED��POLLING,��Ҫִ��LED�������Ʋ���(Ҳִ�е�����Ϩ�����)
*			������LED�ƶ����ڳ�������ʱ,�˺�������������ִ�С�
*	����:	��
*	����:	��
---------------------------------------------------------------------------------------------------*/
static void LLed_Poll(void)
{
	uint8_t	light;
	bool	output;
	
	LLed_Lock();
	//û�а�װ����,�ܾ�ִ�л� û��LED��״̬��Ҫ�ı�,��ֱ���˳�
	if	(pLLedDrv==NULL||pLLedDrv->needChange==FALSE)
	{
		LLed_UnLock();
		return;
	}
	pLLedDrv->needChange=FALSE;
	//ѭ����������LED��
	for	(light=0;light<pLLedDrv->lightMax;light++)
	{
		//�������,������ÿ�ζ���Ҫ���,����״̬�����ı�ʱ��ִ��(��ǿ�����)
		output=FALSE;
		if	(pLLedDrv->led[light].mode==LLED_MODE_FLASH)
		{
			//��˸��������
			if	(pLLedDrv->led[light].status==LLED_STATUS_ON)
			output=LLed_FlashOnCheck(light);//��˸������ʱ��������л�����˸��״̬
			else
			output=LLed_FlashOffCheck(light);//��˸Ϩ���ʱ��������л�����˸��״̬��ͬʱ�������˸����
		}
		else if (pLLedDrv->led[light].mode==LLED_MODE_FLASH_INTERVAL)
		{
			//��Ъ����˸��������
			if	(pLLedDrv->led[light].run.intervalLevel==LLED_FLASH_INTERVAL_DARK)
			{
				pLLedDrv->led[light].run.count++;
				if	(pLLedDrv->led[light].run.count>=pLLedDrv->led[light].flashIntervalParam.darkTime)
				pLLedDrv->led[light].run.intervalLevel=LLED_FLASH_INTERVAL_FINISH;
			}
			else
			{
				//��Ъ����˸�ĵ�һ�����ڶ�����ִ����˸����
				if	(pLLedDrv->led[light].status==LLED_STATUS_ON)
				output=LLed_FlashOnCheck(light);
				else
				output=LLed_FlashOffCheck(light);
				if	(pLLedDrv->led[light].mode!=LLED_MODE_FLASH_INTERVAL)
				LLed_FlashIntervalSwitchToNext(light);
			}
			if	(pLLedDrv->led[light].run.intervalLevel==LLED_FLASH_INTERVAL_FINISH)
			LLed_FlashIntervalFinish(light);
		}
		if	(pLLedDrv->led[light].mode>=LLED_MODE_FLASH)
		pLLedDrv->needChange=TRUE;
		if	(pLLedDrv->led[light].trig==LLED_TRIG_ON||output==TRUE)
		{
			pLLedDrv->led[light].trig=LLED_TRIG_OFF;
			pLLedDrv->drvFunc.output[light](pLLedDrv->led[light].status);
		}
	}
	LLed_UnLock();
}
/*--------------------------------------------------------------------------------------------------
*	ժҪ:	����Ъ����˸�����Ϸ���
*	����:	tLLedFlashIntervalParam pFlashIntervalParam ��Ъ����˸����ָ��
*	����:	TRUE ���OK
*			FALSE���FAIL
---------------------------------------------------------------------------------------------------*/
static bool LLed_FlashIntervalParamCheck(tLLedFlashIntervalParam* pFlashIntervalParam)
{
	if	(pFlashIntervalParam->darkTime!=0&&(pFlashIntervalParam->darkTime%pLLedDrv->scanUnit)!=0)
	return	FALSE;
	if	(pFlashIntervalParam->first.counter==0||
		(pFlashIntervalParam->first.dutyRatio<LLED_DUTY_MIN||
		 pFlashIntervalParam->first.dutyRatio>LLED_DUTY_MAX||
		 (pFlashIntervalParam->first.dutyRatio%10)!=0))
	return	FALSE;
	if	(pFlashIntervalParam->second.counter!=0&&
		 (pFlashIntervalParam->second.dutyRatio<LLED_DUTY_MIN||
		  pFlashIntervalParam->second.dutyRatio>LLED_DUTY_MAX||
		  (pFlashIntervalParam->second.dutyRatio%10)!=0))
	return	FALSE;
	else
	return	TRUE;
}
/*--------------------------------------------------------------------------------------------------
*	ժҪ:	�����ṩ��LED�����,������ͬƵ�ʵ�LED�����
*	����:	uint8_t light���Ƚϵ�LED�����
*	����:	pLLedDrv->lightMax:δ�ҵ���ͬƵ�ʵ�LED��
*			<pLLedDrv->lightMax:�ҵ�����ͬƵ�ʵ�LED��
*	˵��: �˺�������LLed_SyncFlash�������������ģ��
---------------------------------------------------------------------------------------------------*/
static uint8_t	LLed_FindSameFreq(uint8_t light)
{
	uint8_t		loop;
	uint16_t 	syncedLedCnt;
	
	syncedLedCnt=pLLedDrv->led[light].run.onCntMax+pLLedDrv->led[light].run.offCntMax;
	for	(loop=0;loop<pLLedDrv->lightMax;loop++)
	{
		if	(loop!=light)
		{
			if	(pLLedDrv->led[loop].mode==LLED_MODE_FLASH)
			{
				uint8_t	syncLedCnt;
				//������ҵ�ͬ��LED����˸����ֵ
				syncLedCnt=pLLedDrv->led[loop].run.onCntMax+pLLedDrv->led[loop].run.offCntMax;
				if	(syncLedCnt==syncedLedCnt)
				{
					//�����Ƶ���˸����ֵ(Ƶ��)��ͬ
					break;
				}
			}
		}
	}
	return	loop;
}
/*--------------------------------------------------------------------------------------------------
*	ժҪ:	�����ṩ��LED�����,������ͬƵ�ʵ�LED�����,�����䱣����˸
*			��ͬ���ԡ���û���ҵ���ͬƵ�ʵ�LED��ʱ,��������˸�ĳ�ʼ״̬
*	����:	uint8_t light��ͬ����LED�����
*	����:	��
---------------------------------------------------------------------------------------------------*/
static void LLed_SyncFlash(uint8_t light)
{
	uint8_t syncLed;
	
	syncLed=LLed_FindSameFreq(light);
	pLLedDrv->led[light].run.cntDetect=FALSE;
	if	(syncLed<pLLedDrv->lightMax)
	{
		//�����Ƶ���˸����ֵ(Ƶ��)��ͬ����ͬ��
		if	(pLLedDrv->led[syncLed].status==LLED_STATUS_ON)
		{
			if	(pLLedDrv->led[syncLed].run.count<pLLedDrv->led[light].run.onCntMax)
			{
				//ͬ��LED�ƴ�����״̬
				pLLedDrv->led[light].run.count=pLLedDrv->led[syncLed].run.count;
				pLLedDrv->led[light].status=LLED_STATUS_ON;
				if	(pLLedDrv->led[light].run.flashCnt.max!=LLED_FLASHKEEP_CNT)
				pLLedDrv->led[light].run.cntDetect=TRUE;
			}
			else
			{
				//��ռ�ձȲ�ͬ,ʹ��ͬ���ĵ�������ֵ�Ѿ�������ͬ�������ֵ
				pLLedDrv->led[light].run.count=pLLedDrv->led[syncLed].run.count-pLLedDrv->led[light].run.onCntMax;
				pLLedDrv->led[light].status=LLED_STATUS_OFF;
			}
		}
		else
		{
			if	(pLLedDrv->led[syncLed].run.count<pLLedDrv->led[light].run.offCntMax)
			{
				//ͬ��LED�ƴ�����״̬
				pLLedDrv->led[light].run.count=pLLedDrv->led[syncLed].run.count;
				pLLedDrv->led[light].status=LLED_STATUS_OFF;
			}
			else
			{
				//��ռ�ձȲ�ͬ,ʹ��ͬ����Ϩ�����ֵ�Ѿ�������ͬ�������ֵ
				pLLedDrv->led[light].run.count=pLLedDrv->led[syncLed].run.count-pLLedDrv->led[light].run.offCntMax;
				pLLedDrv->led[light].status=LLED_STATUS_ON;
				if	(pLLedDrv->led[light].run.flashCnt.max!=LLED_FLASHKEEP_CNT)
				pLLedDrv->led[light].run.cntDetect=TRUE;
			}
		}
	}
	else
	{
		//û���ҵ�ͬ��LED��,�����ñ�ͬ��LED�ƴӵ�����ʼ
		pLLedDrv->led[light].run.count=0;
		if	(pLLedDrv->led[light].status==LLED_STATUS_ON)
		pLLedDrv->led[light].status=LLED_STATUS_OFF;
		else
		pLLedDrv->led[light].status=LLED_STATUS_ON;
	}
}
/*--------------------------------------------------------------------------------------------------
*	ժҪ:	����װ��pLLedDrv->handle.ctrl��LED�ƹ���״̬���ú���,���������볣��
*			��ͨ��˸���Ъ����˸������˸���Ъ����˸ģʽ��Ҫ�ṩ����
*	����:	uint8_t light LED�����;
*			void* pParam LED�ƿ��Ʋ���(tLLedCtrlParam����);
*			����tLLedCtrlParam�ڵĲ���runParam�谴�����·�ʽ�ṩ:
*			1	��mode=LLED_MODE_BRIGHT/LLED_MODE_DARKNESSʱ
*			runParamδʹ��,������ΪNULL;
*			2	��mode=LLED_MODE_FLASHʱ
*			runParam��ʹ��tLLedFlashParam���͵Ĳ���ָ��;
*			3	��mode=LLED_MODE_FLASH_INTERVALʱ
*			runParam��ʹ��tLLedFlashIntervalParam���͵Ĳ���ָ��;
*	����:	TRUE ���óɹ�;
*			FALSE����ʧ��;
---------------------------------------------------------------------------------------------------*/
static bool LLedFuncCtrl(uint8_t light,void*	pParam)
{
	tLLedCtrlParam	*pCtrlParam;
	bool			result=TRUE;
	
	pCtrlParam=(tLLedCtrlParam*)pParam;
    if	(pLLedDrv==NULL||light>=pLLedDrv->lightMax)
    return	FALSE;
    LLed_Lock();
	//
	pLLedDrv->led[light].run.cntDetect=FALSE;
	switch	(pCtrlParam->mode)
	{
		case	LLED_MODE_BRIGHT:
				if	(pLLedDrv->led[light].mode!=LLED_MODE_BRIGHT)
				{
				  	//����ָ����LED�ƹ����ڳ���
					pLLedDrv->led[light].mode=LLED_MODE_BRIGHT;
					pLLedDrv->led[light].status=LLED_STATUS_ON;
				}
				else
				result=FALSE;
				break;
		case	LLED_MODE_DARKNESS:
				if	(pLLedDrv->led[light].mode!=LLED_MODE_DARKNESS)
				{
				    //����ָ����LED�ƹ����볣��
					pLLedDrv->led[light].mode=LLED_MODE_DARKNESS;
					pLLedDrv->led[light].status=LLED_STATUS_OFF;
				}
				else
				result=FALSE;
				break;
		case	LLED_MODE_FLASH:
				if	(pLLedDrv->led[light].mode!=LLED_MODE_FLASH)
		    	{
		    		tLLedFlashParam*	pFlashParam;

			      	pFlashParam=(tLLedFlashParam*)pCtrlParam->runParam;
					//����LED����˸����
			      	result=LLed_FlashParamSetup(light,pFlashParam);
					if	(result==TRUE)
					{
						//ͬ����ͬƵ�ʵ�LED����˸(δ�ҵ�ʱ,�ӵ�����ʼ)
						LLed_SyncFlash(light);
						pLLedDrv->led[light].mode=LLED_MODE_FLASH;
					}
				}
				else
				result=FALSE;
				break;
		case	LLED_MODE_FLASH_INTERVAL:
				if	(pLLedDrv->led[light].mode!=LLED_MODE_FLASH_INTERVAL)
				{
					tLLedFlashIntervalParam*	pFlashIntervalParam;

					pFlashIntervalParam=(tLLedFlashIntervalParam*)pCtrlParam->runParam;
					result=LLed_FlashIntervalParamCheck(pFlashIntervalParam);
					if	(result==TRUE)
					{
						//��Ъ����˸Ӧ�ò�����Ҫ����ͬ����ͬƵ�ʵ�����
						LLed_FlashParamSetup(light,&pFlashIntervalParam->first);
						if	(pLLedDrv->led[light].status==LLED_STATUS_ON)
						pLLedDrv->led[light].status=LLED_STATUS_OFF;
						else
						pLLedDrv->led[light].status=LLED_STATUS_ON;
						pLLedDrv->led[light].run.intervalCnt.exec=0;
						pLLedDrv->led[light].run.intervalCnt.max=pFlashIntervalParam->count;
						pLLedDrv->led[light].run.intervalLevel=LLED_FLASH_INTERVAL_FIRST;
						pLLedDrv->led[light].flashIntervalParam=*pFlashIntervalParam;
						pLLedDrv->led[light].flashIntervalParam.darkTime/=pLLedDrv->scanUnit;
						pLLedDrv->led[light].mode=LLED_MODE_FLASH_INTERVAL;
					}
				}
				else
				result=FALSE;
				break;
	}
	if	(result==TRUE)
	{
		pLLedDrv->led[light].trig=LLED_TRIG_ON;
	    pLLedDrv->needChange=TRUE;
	}
   	LLed_UnLock();
   	return	result;
}
/*--------------------------------------------------------------------------------------------------
*	ժҪ:	�ṩ���ⲿ���õ�������װ��������Ҫ��������ڴ����롢����
*			���������ʼ���ȡ�
*	����:	tLLedInitParam *pInitParam��ʼ������
*			scanUnit	LED��ɨ��ʱ��[10/20/30/40/50],�糬���˷�Χ����װʧ��
*			drvFunc	LED����������,�������²���
*					output  	��װ��LED��������ƺ���
*					init		��װ��LED�Ƶײ��ʼ������(��������Ϊ��)
*					unInit	��װ��LED�Ƶײ�ȥ��ʼ������(��������Ϊ��)
*					lock		��װ��LED�ƻ�������ȡ����(��������Ϊ��)
*					unLock	��װ��LED�ƻ������黹����(��������Ϊ��)
*	����:	tLLedHandle LED�����������;
*	˵��: ����Ψһһ��ֱ���ṩ���ⲿ������õĺ�����
*			����,����ṩ��lock����,�Ҳ������ǲ���ϵͳ�Ļ�����,��ý���
*			��Դ�������init��(���ṩ�˺���).��Դ�ͷ�ͬʱ����unInit��.����
*			�ﵽ�Ϻõ�Ч��.
---------------------------------------------------------------------------------------------------*/
tLLedHandle* LLed_Initial(tLLedInitParam *pInitParam)
{
	bool		drvFunc=TRUE;
	uint8_t 	light;
	
	if	(pInitParam==NULL||pLLedDrv!=NULL)
	return	NULL;
	for	(light=0;light<pInitParam->lightMax;light++)
	{
		//�������LED����������Ƿ�Ϊ��
		if	(pInitParam->drvFunc.output[light]==NULL)
		{
			drvFunc=FALSE;
			break;
		}
	}
	//���ɨ��ʱ��
	if	(pInitParam->scanUnit>0&&(pInitParam->scanUnit<LLED_SCAN_MIN||pInitParam->scanUnit>LLED_SCAN_MAX||(pInitParam->scanUnit%10)!=0))
	drvFunc=FALSE;	
	//��黥����(�����ṩ�ɶԲ���,���ܵ�һΪ��)
	if	((pInitParam->drvFunc.lock!=NULL&&pInitParam->drvFunc.unLock==NULL)||(pInitParam->drvFunc.lock==NULL&&pInitParam->drvFunc.unLock!=NULL))
	drvFunc=FALSE;
	if	(drvFunc==TRUE)//�����ṩLED���������,����װʧ��
	{
		//��������ָ���ڴ�
		pLLedDrv=MALLOC_MEM(sizeof(tLLedDriver)+sizeof(tLLed)*pInitParam->lightMax+sizeof(tLLedOutputFunc)*pInitParam->lightMax);
		pLLedDrv->led=(tLLed*)((uint8_t*)pLLedDrv+sizeof(tLLedDriver));
		pLLedDrv->drvFunc.output=(tLLedOutputFunc*)((uint8_t*)pLLedDrv->led+sizeof(tLLed)*pInitParam->lightMax);
		pLLedDrv->needChange=FALSE;
		pLLedDrv->lightMax=pInitParam->lightMax;
		pLLedDrv->drvFunc=pInitParam->drvFunc;
		for	(light=0;light<pLLedDrv->lightMax;light++)
		pLLedDrv->drvFunc.output[light]=pInitParam->drvFunc.output[light];
		//��ʼ����ѯɨ��ʱ�䵥λ
		if	(pInitParam->scanUnit==0)
		pLLedDrv->scanUnit=LLEDSCAN_UINT_DEF;
		else
		pLLedDrv->scanUnit=pInitParam->scanUnit;
		if	(pLLedDrv->drvFunc.lock==NULL)
		pLLedDrv->swLock=LLED_UNLOCKED;
		//ִ��Ӳ���ײ��ʼ��
		if	(pLLedDrv->drvFunc.init!=NULL)
		pLLedDrv->drvFunc.init();
		//��ʼ�����(Ĭ�ϳ�ʼ���Ϩ��)
		for	(light=0;light<pLLedDrv->lightMax;light++)
		{
			//������˸,����ִ��Ϩ��(����cntDetect=FALSE,�򲻻����˸����)
			pLLedDrv->led[light].run.cntDetect=FALSE;
			pLLedDrv->led[light].status=LLED_STATUS_OFF;
			pLLedDrv->led[light].mode=LLED_MODE_DARKNESS;
			pLLedDrv->led[light].trig=LLED_TRIG_OFF;
			pLLedDrv->drvFunc.output[light](LLED_STATUS_OFF);
		}
		//����LED���ⲿ�������ƹ��ܺ�����POLL����ָ��
		pLLedDrv->handle.ctrl=LLedFuncCtrl;
		pLLedDrv->handle.poll=LLed_Poll;
		return	&pLLedDrv->handle;
	}
	else
	{
		//��װʧ��
		pLLedDrv=NULL;
		return	NULL;
	}
}
/*--------------------------------------------------------------------------------------------------
*	ժҪ:	�ṩ���ⲿ���õ�����ж�غ�������Ҫ��������ڴ��ͷż��ײ�
*			�ͼ�ȥ��ʼ������(���ṩ�˺���)��
*	����:	��
*	����:	TRUE ִ��ж�سɹ�
*			FALSEִ��ж��ʧ��
*	˵��: ���LLed_Lock������Ӧ�ò��ṩ�Ĳ���ϵͳ����������,��������Դ
*			��÷���init����,���ͷ���Դ����unInit����.
---------------------------------------------------------------------------------------------------*/
bool LLed_UnInitial(void)
{	
	bool	result=TRUE;
	
	LLed_Lock();
	if	(pLLedDrv!=NULL)	
	{		
		//ִ�еͼ�ȥ��ʼ������		
		if	(pLLedDrv->drvFunc.unInit!=NULL)		
		pLLedDrv->drvFunc.unInit();		
		FREE_MEM(pLLedDrv);		
		pLLedDrv=NULL;		
	}	
	else	
	result=FALSE;
	LLed_UnLock();
	return	result;
}
//===========================================================================
/*--END--*/
