
/*------------------------------------------说明:------------------------------------------------
此驱动已经被编译并调试验证,具体验证条件将在LedLight.c详细说明,此处仅
对一个设置进行说明:

1，在验证时USE_OS中定义使用了FreeRTOS,因此才有如下定义
	#if		USE_OS==OS_FREERTOS
	#include "FreeRTOS.h"
	#endif

2，LLED驱动主要具备以下功能:
	1)	通过调用初始化后提供操作句柄,其主要涉及2个功能:
		A，poll指针函数,其被应用层在定时扫描位置调用;
		B,	ctrl指针函数,其被应用层需要控制LED灯时调用;
		说明: 关于ctrl的调用参数说明,请见LLedDriver.h中定义的相关说明部分.
		
	2)	整个驱动主要实现LED灯的多种模式控制操作:
		A,	点亮/熄灭操作;
		B,	普通闪烁控制操作;
		C,	间歇性闪烁控制操作;
		关于间歇性闪烁控制操作,请见LLedDriver.h中定义的相关说明部分.
--------------------------------------------------------------------------------------------------
注:	如果有需要,在考虑节约内存时,可以考虑将以下两个变量设计为指针并在需要执行时才申请内存,
	并在执行完毕后释放内存。目前暂时不作修改。
	tLLedDriver结构中led内的以下变量：
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

//LLED_INTERVAL_SECONDFLASH_DLY设置用于增加FIRST FLASH 与SECOND FLASH直接较明显的间隔
#define	LLED_INTERVAL_SECONDFLASH_DLY	1
//
#define	LLEDSCAN_UINT_DEF				10						//LED灯扫描默认时间单位(ms)
//以下定义内存申请与释放
#define	MALLOC_MEM(memSize)				pvPortMalloc(memSize)	//内存申请宏
#define	FREE_MEM(pMem)					vPortFree(pMem)			//内存释放宏

tLLedDriver	*pLLedDrv=NULL;										//仅用于驱动内部的驱动指针
//===========================================================================
//LED驱动操作互斥锁模块
/*--------------------------------------------------------------------------------------------------
*	摘要:	执行应用层提供的获取互斥锁操作或驱动内部的获取软件互斥锁
*	参数:	无
*	返回:	无
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
*	摘要:	执行应用层提供的归还互斥锁操作或驱动内部的归还软件互斥锁
*	参数:	无
*	返回:	无
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
*	摘要:	设置LED灯闪烁相关参数,根据闪烁频率与占空比计算点亮与熄灭
			计数值,并设置闪烁次数与结束状态
*	参数:	uint8_t light LED灯序号
*			tLLedFlashParam* pFlashParam闪烁参数指针
*	返回:	TRUE 设置成功
*			FALSE设置失败
---------------------------------------------------------------------------------------------------*/
static bool LLed_FlashParamSetup(uint8_t light,tLLedFlashParam* pFlashParam)
{
	uint16_t	totalCnt;

    totalCnt=pFlashParam->freq*LLED_BASETIME/pLLedDrv->scanUnit;
	//这里totalCnt的计算结果不能≤1,否则无法执行闪烁
	if	(totalCnt<=1)
	return	FALSE;
	if	(pFlashParam->dutyRatio<LLED_DUTY_MIN||pFlashParam->dutyRatio>LLED_DUTY_MAX||(pFlashParam->dutyRatio%10)!=0)
	return	FALSE;
    //根据占空比计算闪烁点亮的计数器值
	pLLedDrv->led[light].run.onCntMax=totalCnt*pFlashParam->dutyRatio/100;
	if	(pLLedDrv->led[light].run.onCntMax==0)
	pLLedDrv->led[light].run.onCntMax=1;
   	//计算闪烁熄灭的计数器值
  	pLLedDrv->led[light].run.offCntMax=totalCnt-pLLedDrv->led[light].run.onCntMax;
	pLLedDrv->led[light].run.count=0;
  	pLLedDrv->led[light].run.flashCnt.exec=0;
    pLLedDrv->led[light].run.flashCnt.max=pFlashParam->counter;
	pLLedDrv->led[light].run.endStatus=pFlashParam->endStatus;
	return	TRUE;
}
/*--------------------------------------------------------------------------------------------------
*	摘要:	LED灯闪烁点亮计时检测,并在时间到达后切换至闪烁熄灭状态
*	参数:	LED灯序号
*	返回:	TRUE 输出控制
*			FALSE不输出控制
---------------------------------------------------------------------------------------------------*/
static bool LLed_FlashOnCheck(uint8_t light)
{
	bool	output=FALSE;
	
	//闪烁点亮计时
	pLLedDrv->led[light].run.count++;
	if	(pLLedDrv->led[light].run.count>=pLLedDrv->led[light].run.onCntMax)
	{
		//计时达到设定值，切换至闪烁灭状态
		pLLedDrv->led[light].run.count=0;
		pLLedDrv->led[light].status=LLED_STATUS_OFF;
		if	(pLLedDrv->led[light].run.flashCnt.max!=LLED_FLASHKEEP_CNT)
		pLLedDrv->led[light].run.cntDetect=TRUE;
		output=TRUE;
	}
	return	output;
}
/*--------------------------------------------------------------------------------------------------
*	摘要:	LED灯闪烁计数值计数并检测是否执行完毕,并在执行完毕时设置
*			LED灯到指定的输出状态
*	参数:	LED灯序号
*	返回:	TRUE  输出控制
*			FALSE不输出控制 
*	说明:	此函数是由LLed_FlashOffCheck函数分离出来的模块
---------------------------------------------------------------------------------------------------*/
static bool LLed_FlashCountCheck(uint8_t light)
{
	pLLedDrv->led[light].run.cntDetect=FALSE;
	//闪烁次数计数,并检测闪烁次数
	pLLedDrv->led[light].run.flashCnt.exec++;
	if	(pLLedDrv->led[light].run.flashCnt.exec>=pLLedDrv->led[light].run.flashCnt.max)
	{
		//闪烁次数达到设定值，结束闪烁
		if	(pLLedDrv->led[light].run.endStatus==LLED_STATUS_ON)
		{
			//指定结束LED灯状态为常亮
			pLLedDrv->led[light].status=LLED_STATUS_ON;
			pLLedDrv->led[light].mode=LLED_MODE_BRIGHT;
		}
		else
		{
			//指定结束LED灯状态为常灭
			pLLedDrv->led[light].status=LLED_STATUS_OFF;
			pLLedDrv->led[light].mode=LLED_MODE_DARKNESS;
		}
		return	FALSE;
	}
	else//闪烁没有结束,继续执行(下一次执行灯亮状态)
	{
		pLLedDrv->led[light].status=LLED_STATUS_ON;
		return	TRUE;
	}
}
/*--------------------------------------------------------------------------------------------------
*	摘要:	LED灯闪烁熄灭计时检测,并在时间到达后执行LED灯闪烁计数值计数
*			及检测,并在继续执行时切换至闪烁点亮状态。
*	参数:	LED灯序号
*	返回:	TRUE 输出控制
*			FALSE不输出控制 
---------------------------------------------------------------------------------------------------*/
static bool LLed_FlashOffCheck(uint8_t light)
{
	bool	output=FALSE;
	
	//闪烁熄灭计时
	pLLedDrv->led[light].run.count++;
	if	(pLLedDrv->led[light].run.count>=pLLedDrv->led[light].run.offCntMax)
	{
		//计时达到设定值
		pLLedDrv->led[light].run.count=0;
		if	(pLLedDrv->led[light].run.cntDetect==TRUE)
		output=LLed_FlashCountCheck(light);
		else//持续闪烁,则继续执行(下一次执行灯亮状态)
		{
			pLLedDrv->led[light].status=LLED_STATUS_ON;
			output=TRUE;
		}
	}
	return	output;
}
/*--------------------------------------------------------------------------------------------------
*	摘要:	LED灯间歇闪烁执行阶段切换,主要完成以下几个阶段切换
*			1,FIRST FLASH TO SECOND FLASH(LLED_FLASH_INTERVAL_FIRST --> LLED_FLASH_INTERVAL_SECOND)
*			2,SECOND FLASH TO DARK(LLED_FLASH_INTERVAL_SECOND --> LLED_FLASH_INTERVAL_DARK)
*			3,DARK TO FINISH(LLED_FLASH_INTERVAL_DARK --> LLED_FLASH_INTERVAL_FIN)
*	参数:	LED灯序号
*	返回:	无 
---------------------------------------------------------------------------------------------------*/
static void LLed_FlashIntervalSwitchToNext(uint8_t light)
{
	if	(pLLedDrv->led[light].run.intervalLevel==LLED_FLASH_INTERVAL_FIRST)
	{
		if	(pLLedDrv->led[light].flashIntervalParam.second.counter>0)
		{
			//切换至运行的第二阶段闪烁
			LLed_FlashParamSetup(light,&pLLedDrv->led[light].flashIntervalParam.second);
			#if	LLED_INTERVAL_SECONDFLASH_DLY==1
			//为了增加SECOND FLASH与FIRST FLASH之间的明显间隔,使SECOND FLASH延迟一个计数周期在开始
			pLLedDrv->led[light].status=LLED_STATUS_OFF;
			#else
			pLLedDrv->led[light].status=LLED_STATUS_ON;
			pLLedDrv->led[light].trig=LLED_TRIG_ON;
			#endif
			pLLedDrv->led[light].run.intervalLevel=LLED_FLASH_INTERVAL_SECOND;
		}
		else//第二阶段的执行次数为0,则跳过到第三阶段
		pLLedDrv->led[light].run.intervalLevel=LLED_FLASH_INTERVAL_DARK;
	}
	else//切换至运行的第三阶段熄灭
	pLLedDrv->led[light].run.intervalLevel=LLED_FLASH_INTERVAL_DARK;
	//第三阶段的熄灭时间为0,则跳过执行
	if	(pLLedDrv->led[light].run.intervalLevel==LLED_FLASH_INTERVAL_DARK&&pLLedDrv->led[light].flashIntervalParam.darkTime==0)
	pLLedDrv->led[light].run.intervalLevel=LLED_FLASH_INTERVAL_FINISH;
	pLLedDrv->led[light].mode=LLED_MODE_FLASH_INTERVAL;
}
/*--------------------------------------------------------------------------------------------------
*	摘要:	LED灯间歇闪烁一次执行完成处理,整个闪烁执行完毕时,则设置到
*			最后指定的输出状态;否则继续执行下一次间歇性闪烁。
*	参数:	LED灯序号
*	返回:	无 
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
				//指定结束LED灯状态为常亮
				pLLedDrv->led[light].status=LLED_STATUS_ON;
				pLLedDrv->led[light].mode=LLED_MODE_BRIGHT;
			}
			else
			{
				//指定结束LED灯状态为常灭
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
*	摘要:	LED灯POLLING,主要执行LED灯闪控制操作(也执行点亮与熄灭操作)
*			当所有LED灯都处于常亮或常灭时,此函数将不会往下执行。
*	参数:	无
*	返回:	无
---------------------------------------------------------------------------------------------------*/
static void LLed_Poll(void)
{
	uint8_t	light;
	bool	output;
	
	LLed_Lock();
	//没有安装驱动,拒绝执行或 没有LED灯状态需要改变,则直接退出
	if	(pLLedDrv==NULL||pLLedDrv->needChange==FALSE)
	{
		LLed_UnLock();
		return;
	}
	pLLedDrv->needChange=FALSE;
	//循环处理所有LED灯
	for	(light=0;light<pLLedDrv->lightMax;light++)
	{
		//输出控制,并不是每次都需要输出,仅在状态发生改变时才执行(或强制输出)
		output=FALSE;
		if	(pLLedDrv->led[light].mode==LLED_MODE_FLASH)
		{
			//闪烁操作处理
			if	(pLLedDrv->led[light].status==LLED_STATUS_ON)
			output=LLed_FlashOnCheck(light);//闪烁点亮计时，并检测切换至闪烁灭状态
			else
			output=LLed_FlashOffCheck(light);//闪烁熄灭计时，并检测切换至闪烁亮状态，同时，检测闪烁次数
		}
		else if (pLLedDrv->led[light].mode==LLED_MODE_FLASH_INTERVAL)
		{
			//间歇性闪烁操作处理
			if	(pLLedDrv->led[light].run.intervalLevel==LLED_FLASH_INTERVAL_DARK)
			{
				pLLedDrv->led[light].run.count++;
				if	(pLLedDrv->led[light].run.count>=pLLedDrv->led[light].flashIntervalParam.darkTime)
				pLLedDrv->led[light].run.intervalLevel=LLED_FLASH_INTERVAL_FINISH;
			}
			else
			{
				//间歇性闪烁的第一步及第二步均执行闪烁操作
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
*	摘要:	检查间歇性闪烁参数合法性
*	参数:	tLLedFlashIntervalParam pFlashIntervalParam 间歇性闪烁参数指针
*	返回:	TRUE 检查OK
*			FALSE检查FAIL
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
*	摘要:	根据提供的LED灯序号,查找相同频率的LED灯序号
*	参数:	uint8_t light被比较的LED灯序号
*	返回:	pLLedDrv->lightMax:未找到相同频率的LED灯
*			<pLLedDrv->lightMax:找到了相同频率的LED灯
*	说明: 此函数是由LLed_SyncFlash函数分离出来的模块
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
				//计算查找到同步LED灯闪烁计数值
				syncLedCnt=pLLedDrv->led[loop].run.onCntMax+pLLedDrv->led[loop].run.offCntMax;
				if	(syncLedCnt==syncedLedCnt)
				{
					//两个灯的闪烁计数值(频率)相同
					break;
				}
			}
		}
	}
	return	loop;
}
/*--------------------------------------------------------------------------------------------------
*	摘要:	根据提供的LED灯序号,查找相同频率的LED灯序号,并与其保持闪烁
*			的同步性。若没有找到相同频率的LED灯时,则设置闪烁的初始状态
*	参数:	uint8_t light被同步的LED灯序号
*	返回:	无
---------------------------------------------------------------------------------------------------*/
static void LLed_SyncFlash(uint8_t light)
{
	uint8_t syncLed;
	
	syncLed=LLed_FindSameFreq(light);
	pLLedDrv->led[light].run.cntDetect=FALSE;
	if	(syncLed<pLLedDrv->lightMax)
	{
		//两个灯的闪烁计数值(频率)相同，则同步
		if	(pLLedDrv->led[syncLed].status==LLED_STATUS_ON)
		{
			if	(pLLedDrv->led[syncLed].run.count<pLLedDrv->led[light].run.onCntMax)
			{
				//同步LED灯处于亮状态
				pLLedDrv->led[light].run.count=pLLedDrv->led[syncLed].run.count;
				pLLedDrv->led[light].status=LLED_STATUS_ON;
				if	(pLLedDrv->led[light].run.flashCnt.max!=LLED_FLASHKEEP_CNT)
				pLLedDrv->led[light].run.cntDetect=TRUE;
			}
			else
			{
				//因占空比不同,使得同步的点亮计数值已经超出被同步的最大值
				pLLedDrv->led[light].run.count=pLLedDrv->led[syncLed].run.count-pLLedDrv->led[light].run.onCntMax;
				pLLedDrv->led[light].status=LLED_STATUS_OFF;
			}
		}
		else
		{
			if	(pLLedDrv->led[syncLed].run.count<pLLedDrv->led[light].run.offCntMax)
			{
				//同步LED灯处于灭状态
				pLLedDrv->led[light].run.count=pLLedDrv->led[syncLed].run.count;
				pLLedDrv->led[light].status=LLED_STATUS_OFF;
			}
			else
			{
				//因占空比不同,使得同步的熄灭计数值已经超出被同步的最大值
				pLLedDrv->led[light].run.count=pLLedDrv->led[syncLed].run.count-pLLedDrv->led[light].run.offCntMax;
				pLLedDrv->led[light].status=LLED_STATUS_ON;
				if	(pLLedDrv->led[light].run.flashCnt.max!=LLED_FLASHKEEP_CNT)
				pLLedDrv->led[light].run.cntDetect=TRUE;
			}
		}
	}
	else
	{
		//没有找到同步LED灯,则设置被同步LED灯从点亮开始
		pLLedDrv->led[light].run.count=0;
		if	(pLLedDrv->led[light].status==LLED_STATUS_ON)
		pLLedDrv->led[light].status=LLED_STATUS_OFF;
		else
		pLLedDrv->led[light].status=LLED_STATUS_ON;
	}
}
/*--------------------------------------------------------------------------------------------------
*	摘要:	被安装在pLLedDrv->handle.ctrl的LED灯工作状态设置函数,包括常亮与常灭
*			普通闪烁与间歇性闪烁。在闪烁或间歇性闪烁模式需要提供参数
*	参数:	uint8_t light LED灯序号;
*			void* pParam LED灯控制参数(tLLedCtrlParam类型);
*			其中tLLedCtrlParam内的参数runParam需按照以下方式提供:
*			1	当mode=LLED_MODE_BRIGHT/LLED_MODE_DARKNESS时
*			runParam未使用,可设置为NULL;
*			2	当mode=LLED_MODE_FLASH时
*			runParam需使用tLLedFlashParam类型的参数指针;
*			3	当mode=LLED_MODE_FLASH_INTERVAL时
*			runParam需使用tLLedFlashIntervalParam类型的参数指针;
*	返回:	TRUE 设置成功;
*			FALSE设置失败;
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
				  	//设置指定的LED灯工作于常亮
					pLLedDrv->led[light].mode=LLED_MODE_BRIGHT;
					pLLedDrv->led[light].status=LLED_STATUS_ON;
				}
				else
				result=FALSE;
				break;
		case	LLED_MODE_DARKNESS:
				if	(pLLedDrv->led[light].mode!=LLED_MODE_DARKNESS)
				{
				    //设置指定的LED灯工作与常灭
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
					//设置LED灯闪烁参数
			      	result=LLed_FlashParamSetup(light,pFlashParam);
					if	(result==TRUE)
					{
						//同步相同频率的LED灯闪烁(未找到时,从点亮开始)
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
						//间歇性闪烁应该不再需要考虑同步相同频率的问题
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
*	摘要:	提供给外部调用的驱动安装函数。主要完成驱动内存申请、驱动
*			控制输出初始化等。
*	参数:	tLLedInitParam *pInitParam初始化参数
*			scanUnit	LED灯扫描时间[10/20/30/40/50],如超出此范围将安装失败
*			drvFunc	LED灯驱动函数,包括以下部分
*					output  	安装的LED灯输出控制函数
*					init		安装的LED灯底层初始化函数(可以设置为空)
*					unInit	安装的LED灯底层去初始化函数(可以设置为空)
*					lock		安装的LED灯互斥锁获取函数(可以设置为空)
*					unLock	安装的LED灯互斥锁归还函数(可以设置为空)
*	返回:	tLLedHandle LED驱动操作句柄;
*	说明: 这是唯一一个直接提供给外部必须调用的函数。
*			另外,如果提供了lock函数,且操作的是操作系统的互斥量,最好将其
*			资源申请放在init中(并提供此函数).资源释放同时放在unInit中.可以
*			达到较好的效果.
---------------------------------------------------------------------------------------------------*/
tLLedHandle* LLed_Initial(tLLedInitParam *pInitParam)
{
	bool		drvFunc=TRUE;
	uint8_t 	light;
	
	if	(pInitParam==NULL||pLLedDrv!=NULL)
	return	NULL;
	for	(light=0;light<pInitParam->lightMax;light++)
	{
		//检查所有LED灯输出函数是否为空
		if	(pInitParam->drvFunc.output[light]==NULL)
		{
			drvFunc=FALSE;
			break;
		}
	}
	//检查扫描时间
	if	(pInitParam->scanUnit>0&&(pInitParam->scanUnit<LLED_SCAN_MIN||pInitParam->scanUnit>LLED_SCAN_MAX||(pInitParam->scanUnit%10)!=0))
	drvFunc=FALSE;	
	//检查互斥锁(必须提供成对操作,不能单一为空)
	if	((pInitParam->drvFunc.lock!=NULL&&pInitParam->drvFunc.unLock==NULL)||(pInitParam->drvFunc.lock==NULL&&pInitParam->drvFunc.unLock!=NULL))
	drvFunc=FALSE;
	if	(drvFunc==TRUE)//必须提供LED灯输出函数,否则安装失败
	{
		//申请驱动指针内存
		pLLedDrv=MALLOC_MEM(sizeof(tLLedDriver)+sizeof(tLLed)*pInitParam->lightMax+sizeof(tLLedOutputFunc)*pInitParam->lightMax);
		pLLedDrv->led=(tLLed*)((uint8_t*)pLLedDrv+sizeof(tLLedDriver));
		pLLedDrv->drvFunc.output=(tLLedOutputFunc*)((uint8_t*)pLLedDrv->led+sizeof(tLLed)*pInitParam->lightMax);
		pLLedDrv->needChange=FALSE;
		pLLedDrv->lightMax=pInitParam->lightMax;
		pLLedDrv->drvFunc=pInitParam->drvFunc;
		for	(light=0;light<pLLedDrv->lightMax;light++)
		pLLedDrv->drvFunc.output[light]=pInitParam->drvFunc.output[light];
		//初始化轮询扫描时间单位
		if	(pInitParam->scanUnit==0)
		pLLedDrv->scanUnit=LLEDSCAN_UINT_DEF;
		else
		pLLedDrv->scanUnit=pInitParam->scanUnit;
		if	(pLLedDrv->drvFunc.lock==NULL)
		pLLedDrv->swLock=LLED_UNLOCKED;
		//执行硬件底层初始化
		if	(pLLedDrv->drvFunc.init!=NULL)
		pLLedDrv->drvFunc.init();
		//初始化输出(默认初始输出熄灭)
		for	(light=0;light<pLLedDrv->lightMax;light++)
		{
			//对于闪烁,首先执行熄灭(设置cntDetect=FALSE,则不会对闪烁计数)
			pLLedDrv->led[light].run.cntDetect=FALSE;
			pLLedDrv->led[light].status=LLED_STATUS_OFF;
			pLLedDrv->led[light].mode=LLED_MODE_DARKNESS;
			pLLedDrv->led[light].trig=LLED_TRIG_OFF;
			pLLedDrv->drvFunc.output[light](LLED_STATUS_OFF);
		}
		//设置LED灯外部操作控制功能函数及POLL函数指针
		pLLedDrv->handle.ctrl=LLedFuncCtrl;
		pLLedDrv->handle.poll=LLed_Poll;
		return	&pLLedDrv->handle;
	}
	else
	{
		//安装失败
		pLLedDrv=NULL;
		return	NULL;
	}
}
/*--------------------------------------------------------------------------------------------------
*	摘要:	提供给外部调用的驱动卸载函数。主要完成驱动内存释放及底层
*			低级去初始化操作(如提供此函数)。
*	参数:	无
*	返回:	TRUE 执行卸载成功
*			FALSE执行卸载失败
*	说明: 如果LLed_Lock包含了应用层提供的操作系统互斥量操作,则申请资源
*			最好放在init函数,且释放资源放在unInit函数.
---------------------------------------------------------------------------------------------------*/
bool LLed_UnInitial(void)
{	
	bool	result=TRUE;
	
	LLed_Lock();
	if	(pLLedDrv!=NULL)	
	{		
		//执行低级去初始化操作		
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
