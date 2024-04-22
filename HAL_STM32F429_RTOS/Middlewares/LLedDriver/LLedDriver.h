
#ifndef	_LLEDDRIVER_H
#define	_LLEDDRIVER_H

#ifndef	TRUE
#define	TRUE	1
#define	FALSE	0
typedef uint8_t bool;
#endif

//以下LLED为LED LIGHT缩写，并将LIGHT缩写L写在前面
//LED锁(非OS使用)
typedef enum 
{
  	LLED_UNLOCKED,
  	LLED_LOCKED  
}tLLed_Lock;

//LED灯状态 
typedef enum
{
	LLED_STATUS_ON,										//点亮状态
	LLED_STATUS_OFF										//熄灭状态
}tLLedStatus;

//LED 输出强制触发标志
typedef enum
{
	LLED_TRIG_ON,										//触发打开
	LLED_TRIG_OFF										//触发关闭
}tLLedTrig;

//LED灯工作方式
typedef enum
{
	LLED_MODE_BRIGHT,									//常亮方式
	LLED_MODE_DARKNESS,									//熄灭方式
	LLED_MODE_FLASH,									//闪烁方式
	LLED_MODE_FLASH_INTERVAL							//间歇性闪烁方式
}tLLedMode;
/*----------------------------------------------------------------------
	间歇性闪烁说明:分三个步骤执行
	1	FIRST FLASH(短或长) ,以设定频率闪烁N次;
	2	SECOND FLASH(长或短),以设定频率闪烁M次;
	3	DARK,熄灭一定时间;
	注:FIRST FLASH与SECOND FLASH应该设置为不同频率以实现
		短闪或长闪;同时,如果N或M等于0,则跳过,但M与N
		不能同时为	0.
		
	另外,通过此参数可实现以下功能:
	1)	N 短闪+ M长闪+ 间歇;
	2)	N 短闪+ M长闪;
	3)	N 短闪+ 间歇;
	4)	N 短闪;
	或以上短闪与长闪交换的效果
----------------------------------------------------------------------*/
#define	LLED_FLASH_INTERVAL_FIRST		0				//间歇性运行第一个阶段(闪烁 N次)
#define	LLED_FLASH_INTERVAL_SECOND		1				//间歇性运行第二个阶段(闪烁M次)
#define	LLED_FLASH_INTERVAL_DARK		2				//间歇性运行第三个阶段(熄灭)
#define	LLED_FLASH_INTERVAL_FINISH		3				//间隙性运行一组完成

//LED灯闪烁频率 （有些频率其实并没有比较明显的效果,不用定义每一个频率）
#define	LLED_BASETIME					100				//基本时间(ms)
typedef enum
{
	LLED_FREQ_1_10Hz=100,								//0.1Hz
	LLED_FREQ_2_10Hz=50,								//0.2Hz
	LLED_FREQ_5_10Hz=20,								//0.5Hz
	LLED_FREQ_1Hz=10,									//1Hz
	LLED_FREQ_2Hz=5,									//2Hz
	LLED_FREQ_5Hz=2,									//5Hz
	LLED_FREQ_10Hz=1,									//10Hz
}tLLedFreq;//LED灯闪烁频率 

//部分灯闪烁参数限定值
#define	LLED_DUTY_MIN					10				//占空比最小值
#define	LLED_DUTY_MAX					90				//占空比最大值
#define	LLED_SCAN_MIN					10				//扫描时间最小值
#define	LLED_SCAN_MAX					50				//扫描时间最大值
//LED灯闪烁参数
typedef struct
{
	tLLedFreq				freq;						//闪烁频率
	tLLedStatus				endStatus;					//闪烁结束LED灯状态
	uint8_t					dutyRatio;					//取值范围[10/20/30/40/50/60/70/80/90]
	uint8_t					counter;					//闪烁次数[0--255],0表示持续闪烁
}tLLedFlashParam;
//间歇性闪烁参数
typedef struct
{
	uint8_t					count;						//间歇闪烁执行次数
	uint16_t				darkTime;					//熄灭时间,单位ms,但必须设置为扫描时间的整数值
	tLLedFlashParam			first;						//闪烁first参数
	tLLedFlashParam			second;						//闪烁second参数
}tLLedFlashIntervalParam;
//LED灯闪烁与间歇性闪烁计数器
typedef struct
{
	uint8_t					max;						//最大计数值
	uint8_t					exec;						//执行计数值
}tLLedCnt;

//LED灯闪烁运行数据
typedef struct
{
	bool					cntDetect;					//计数检测标志（取值TRUE才可以检测计数值）
	uint8_t					intervalLevel;				//记录间歇性运行的阶段
	uint16_t				count;						//计数器
	uint16_t				onCntMax;					//点亮计数器最大值
	uint16_t				offCntMax;					//熄灭计数器最大值
	tLLedCnt				flashCnt;					//闪烁计数器
	tLLedCnt				intervalCnt;				//间歇闪烁计数器
	tLLedStatus				endStatus;					//闪烁结束LED灯状态
}tLLedFlashRun;
/*----------------------------------------------------------------------
闪烁开始执行时LED状态说明:
当前状态为:点亮,则会先从熄灭开始执行,同时会在下
				 一个点亮开始计数
当前状态为:熄灭,则直接从点亮开始执行,并开始计数
----------------------------------------------------------------------*/
//LED灯控制参数（调用LED灯控制输出时的入口参数）
typedef struct
{
	tLLedMode				mode;						//LED灯工作模式
	void*					runParam;					//LED工作运行参数
}tLLedCtrlParam;
/*----------------------------------------------------------------------
	参数runParam说明:
	1	当mode=LLED_MODE_BRIGHT/LLED_MODE_DARKNESS时
		runParam未使用,可设置为NULL;
	2	当mode=LLED_MODE_FLASH时
		runParam需使用tLLedFlashParam类型的参数指针;
	3	当mode=LLED_MODE_FLASH_INTERVAL时
		runParam需使用tLLedFlashIntervalParam类型的参数指针;
----------------------------------------------------------------------*/
//LED灯驱动函数原型定义
typedef void(*tLLedVoid)(void);							//LED无参函数原型
typedef bool(*tLLedCtrlFunc)(uint8_t,void*);			//LED灯工作控制函数原型
typedef void(*tLLedOutputFunc)(tLLedStatus);			//LED灯输出控制函数原型

//LED灯应用层提供的驱动函数
typedef struct
{
	tLLedOutputFunc			*output;					//安装的LED灯输出控制函数
	tLLedVoid				init;						//安装的LED灯底层初始化函数(可以设置为空）
	tLLedVoid				unInit;						//安装的LED灯底层去初始化函数(可以设置为空）
	tLLedVoid				lock;						//安装的LED灯互斥锁获取函数(可以设置为空）
	tLLedVoid				unLock;						//安装的LED灯互斥锁归还函数(可以设置为空）
}tLLedDrvFunc;

//LED灯驱动安装参数
typedef struct
{	
	uint8_t					scanUnit;					//LED灯扫描时间[10/20/30/40/50]
	uint8_t					lightMax;					//LED灯最大数目
	tLLedDrvFunc			drvFunc;					//LED灯驱动函数	
}tLLedInitParam;

//LED灯基本结构
typedef struct
{
	tLLedMode				mode;						//LED灯工作模式
	tLLedTrig				trig;						//LED点亮及熄灭触发标志
	tLLedStatus				status;						//LED灯状态
	tLLedFlashRun			run;						//LED灯闪烁运行
	tLLedFlashIntervalParam	flashIntervalParam;			//LED灯间歇性闪烁保存参数
}tLLed;

//LED 灯驱动提供给应用层的操作句柄
typedef struct
{
	tLLedVoid				poll;						//poll函数
	tLLedCtrlFunc			ctrl;						//LED灯控制功能函数
}tLLedHandle;
/*----------------------------------------------------------------------
							ctrl功能函数使用说明:
							
	light为需要设置的LED灯序号		
	ledHandle为LED操作句柄
	1,控制LED灯常亮或常灭:
		tLLedCtrlParam	ctrlParam;

		ctrlParam.mode=LLED_MODE_BRIGHT;//LLED_MODE_DARKNESS;
		ctrlParam.runParam=NULL;
		ledHandle->ctrl(light,&ctrlParam);
	2,控制LED灯闪烁
		tLLedCtrlParam	ctrlParam;
		tLLedFlashParam	flashParam;

		ctrlParam.mode=LLED_MODE_FLASH;
		flashParam.freq=LLED_FREQ_1Hz;
		flashParam.dutyRatio=50;
		flashParam.endStatus=LLED_STATUS_OFF;
		flashParam.counter=LLED_FLASHKEEP_CNT;
		ctrlParam.runParam=&flashParam;
		ledHandle->ctrl(light,&ctrlParam);
	3,控制LED灯为间歇性闪烁
		tLLedCtrlParam			ctrlParam;
		tLLedFlashIntervalParam	flashIntervalParam;
		//以下设置3短1长闪烁(持续执行)
		ctrlParam.mode=LLED_MODE_FLASH_INTERVAL;
		flashIntervalParam.first.freq=LLED_FREQ_5Hz;
		flashIntervalParam.first.dutyRatio=50;
		flashIntervalParam.first.endStatus=LLED_STATUS_OFF;
		flashIntervalParam.first.counter=3;
		flashIntervalParam.second.freq=LLED_FREQ_1Hz;
		flashIntervalParam.second.dutyRatio=50;
		flashIntervalParam.second.endStatus=LLED_STATUS_OFF;
		flashIntervalParam.second.counter=1;
		flashIntervalParam.darkTime=1000;
		flashIntervalParam.count=LLED_FLASHKEEP_CNT;
		ctrlParam.runParam=&flashIntervalParam;
		ledHandle->ctrl(light,&ctrlParam);
-----------------------------------------------------------------------*/
//LED灯驱动程序使用的数据结构
typedef struct
{
	uint8_t					scanUnit;					//LED扫描时间单位,ms
	uint8_t					lightMax;					//LED灯最大数目
	bool					needChange;					//LED灯需要改变状态时,将被设置为TRUE)
	tLLed_Lock				swLock;						//LED软件互斥锁
	tLLed					*led;						//LED灯组
	tLLedDrvFunc			drvFunc;					//外部安装的驱动函数
	tLLedHandle				handle;						//外部调用的功能函数(此类型说明在后面安装驱动中说明)
}tLLedDriver;

#define	LLED_FLASHKEEP_CNT	0							//持续闪烁(计数值设置为0)

tLLedHandle* LLed_Initial(tLLedInitParam *pInitParam);
bool LLed_UnInitial(void);
#endif
