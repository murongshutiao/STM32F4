
#ifndef	_LLEDDRIVER_H
#define	_LLEDDRIVER_H

#ifndef	TRUE
#define	TRUE	1
#define	FALSE	0
typedef uint8_t bool;
#endif

//����LLEDΪLED LIGHT��д������LIGHT��дLд��ǰ��
//LED��(��OSʹ��)
typedef enum 
{
  	LLED_UNLOCKED,
  	LLED_LOCKED  
}tLLed_Lock;

//LED��״̬ 
typedef enum
{
	LLED_STATUS_ON,										//����״̬
	LLED_STATUS_OFF										//Ϩ��״̬
}tLLedStatus;

//LED ���ǿ�ƴ�����־
typedef enum
{
	LLED_TRIG_ON,										//������
	LLED_TRIG_OFF										//�����ر�
}tLLedTrig;

//LED�ƹ�����ʽ
typedef enum
{
	LLED_MODE_BRIGHT,									//������ʽ
	LLED_MODE_DARKNESS,									//Ϩ��ʽ
	LLED_MODE_FLASH,									//��˸��ʽ
	LLED_MODE_FLASH_INTERVAL							//��Ъ����˸��ʽ
}tLLedMode;
/*----------------------------------------------------------------------
	��Ъ����˸˵��:����������ִ��
	1	FIRST FLASH(�̻�) ,���趨Ƶ����˸N��;
	2	SECOND FLASH(�����),���趨Ƶ����˸M��;
	3	DARK,Ϩ��һ��ʱ��;
	ע:FIRST FLASH��SECOND FLASHӦ������Ϊ��ͬƵ����ʵ��
		��������;ͬʱ,���N��M����0,������,��M��N
		����ͬʱΪ	0.
		
	����,ͨ���˲�����ʵ�����¹���:
	1)	N ����+ M����+ ��Ъ;
	2)	N ����+ M����;
	3)	N ����+ ��Ъ;
	4)	N ����;
	�����϶����볤��������Ч��
----------------------------------------------------------------------*/
#define	LLED_FLASH_INTERVAL_FIRST		0				//��Ъ�����е�һ���׶�(��˸ N��)
#define	LLED_FLASH_INTERVAL_SECOND		1				//��Ъ�����еڶ����׶�(��˸M��)
#define	LLED_FLASH_INTERVAL_DARK		2				//��Ъ�����е������׶�(Ϩ��)
#define	LLED_FLASH_INTERVAL_FINISH		3				//��϶������һ�����

//LED����˸Ƶ�� ����ЩƵ����ʵ��û�бȽ����Ե�Ч��,���ö���ÿһ��Ƶ�ʣ�
#define	LLED_BASETIME					100				//����ʱ��(ms)
typedef enum
{
	LLED_FREQ_1_10Hz=100,								//0.1Hz
	LLED_FREQ_2_10Hz=50,								//0.2Hz
	LLED_FREQ_5_10Hz=20,								//0.5Hz
	LLED_FREQ_1Hz=10,									//1Hz
	LLED_FREQ_2Hz=5,									//2Hz
	LLED_FREQ_5Hz=2,									//5Hz
	LLED_FREQ_10Hz=1,									//10Hz
}tLLedFreq;//LED����˸Ƶ�� 

//���ֵ���˸�����޶�ֵ
#define	LLED_DUTY_MIN					10				//ռ�ձ���Сֵ
#define	LLED_DUTY_MAX					90				//ռ�ձ����ֵ
#define	LLED_SCAN_MIN					10				//ɨ��ʱ����Сֵ
#define	LLED_SCAN_MAX					50				//ɨ��ʱ�����ֵ
//LED����˸����
typedef struct
{
	tLLedFreq				freq;						//��˸Ƶ��
	tLLedStatus				endStatus;					//��˸����LED��״̬
	uint8_t					dutyRatio;					//ȡֵ��Χ[10/20/30/40/50/60/70/80/90]
	uint8_t					counter;					//��˸����[0--255],0��ʾ������˸
}tLLedFlashParam;
//��Ъ����˸����
typedef struct
{
	uint8_t					count;						//��Ъ��˸ִ�д���
	uint16_t				darkTime;					//Ϩ��ʱ��,��λms,����������Ϊɨ��ʱ�������ֵ
	tLLedFlashParam			first;						//��˸first����
	tLLedFlashParam			second;						//��˸second����
}tLLedFlashIntervalParam;
//LED����˸���Ъ����˸������
typedef struct
{
	uint8_t					max;						//������ֵ
	uint8_t					exec;						//ִ�м���ֵ
}tLLedCnt;

//LED����˸��������
typedef struct
{
	bool					cntDetect;					//��������־��ȡֵTRUE�ſ��Լ�����ֵ��
	uint8_t					intervalLevel;				//��¼��Ъ�����еĽ׶�
	uint16_t				count;						//������
	uint16_t				onCntMax;					//�������������ֵ
	uint16_t				offCntMax;					//Ϩ����������ֵ
	tLLedCnt				flashCnt;					//��˸������
	tLLedCnt				intervalCnt;				//��Ъ��˸������
	tLLedStatus				endStatus;					//��˸����LED��״̬
}tLLedFlashRun;
/*----------------------------------------------------------------------
��˸��ʼִ��ʱLED״̬˵��:
��ǰ״̬Ϊ:����,����ȴ�Ϩ��ʼִ��,ͬʱ������
				 һ��������ʼ����
��ǰ״̬Ϊ:Ϩ��,��ֱ�Ӵӵ�����ʼִ��,����ʼ����
----------------------------------------------------------------------*/
//LED�ƿ��Ʋ���������LED�ƿ������ʱ����ڲ�����
typedef struct
{
	tLLedMode				mode;						//LED�ƹ���ģʽ
	void*					runParam;					//LED�������в���
}tLLedCtrlParam;
/*----------------------------------------------------------------------
	����runParam˵��:
	1	��mode=LLED_MODE_BRIGHT/LLED_MODE_DARKNESSʱ
		runParamδʹ��,������ΪNULL;
	2	��mode=LLED_MODE_FLASHʱ
		runParam��ʹ��tLLedFlashParam���͵Ĳ���ָ��;
	3	��mode=LLED_MODE_FLASH_INTERVALʱ
		runParam��ʹ��tLLedFlashIntervalParam���͵Ĳ���ָ��;
----------------------------------------------------------------------*/
//LED����������ԭ�Ͷ���
typedef void(*tLLedVoid)(void);							//LED�޲κ���ԭ��
typedef bool(*tLLedCtrlFunc)(uint8_t,void*);			//LED�ƹ������ƺ���ԭ��
typedef void(*tLLedOutputFunc)(tLLedStatus);			//LED��������ƺ���ԭ��

//LED��Ӧ�ò��ṩ����������
typedef struct
{
	tLLedOutputFunc			*output;					//��װ��LED��������ƺ���
	tLLedVoid				init;						//��װ��LED�Ƶײ��ʼ������(��������Ϊ�գ�
	tLLedVoid				unInit;						//��װ��LED�Ƶײ�ȥ��ʼ������(��������Ϊ�գ�
	tLLedVoid				lock;						//��װ��LED�ƻ�������ȡ����(��������Ϊ�գ�
	tLLedVoid				unLock;						//��װ��LED�ƻ������黹����(��������Ϊ�գ�
}tLLedDrvFunc;

//LED��������װ����
typedef struct
{	
	uint8_t					scanUnit;					//LED��ɨ��ʱ��[10/20/30/40/50]
	uint8_t					lightMax;					//LED�������Ŀ
	tLLedDrvFunc			drvFunc;					//LED����������	
}tLLedInitParam;

//LED�ƻ����ṹ
typedef struct
{
	tLLedMode				mode;						//LED�ƹ���ģʽ
	tLLedTrig				trig;						//LED������Ϩ�𴥷���־
	tLLedStatus				status;						//LED��״̬
	tLLedFlashRun			run;						//LED����˸����
	tLLedFlashIntervalParam	flashIntervalParam;			//LED�Ƽ�Ъ����˸�������
}tLLed;

//LED �������ṩ��Ӧ�ò�Ĳ������
typedef struct
{
	tLLedVoid				poll;						//poll����
	tLLedCtrlFunc			ctrl;						//LED�ƿ��ƹ��ܺ���
}tLLedHandle;
/*----------------------------------------------------------------------
							ctrl���ܺ���ʹ��˵��:
							
	lightΪ��Ҫ���õ�LED�����		
	ledHandleΪLED�������
	1,����LED�Ƴ�������:
		tLLedCtrlParam	ctrlParam;

		ctrlParam.mode=LLED_MODE_BRIGHT;//LLED_MODE_DARKNESS;
		ctrlParam.runParam=NULL;
		ledHandle->ctrl(light,&ctrlParam);
	2,����LED����˸
		tLLedCtrlParam	ctrlParam;
		tLLedFlashParam	flashParam;

		ctrlParam.mode=LLED_MODE_FLASH;
		flashParam.freq=LLED_FREQ_1Hz;
		flashParam.dutyRatio=50;
		flashParam.endStatus=LLED_STATUS_OFF;
		flashParam.counter=LLED_FLASHKEEP_CNT;
		ctrlParam.runParam=&flashParam;
		ledHandle->ctrl(light,&ctrlParam);
	3,����LED��Ϊ��Ъ����˸
		tLLedCtrlParam			ctrlParam;
		tLLedFlashIntervalParam	flashIntervalParam;
		//��������3��1����˸(����ִ��)
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
//LED����������ʹ�õ����ݽṹ
typedef struct
{
	uint8_t					scanUnit;					//LEDɨ��ʱ�䵥λ,ms
	uint8_t					lightMax;					//LED�������Ŀ
	bool					needChange;					//LED����Ҫ�ı�״̬ʱ,��������ΪTRUE)
	tLLed_Lock				swLock;						//LED���������
	tLLed					*led;						//LED����
	tLLedDrvFunc			drvFunc;					//�ⲿ��װ����������
	tLLedHandle				handle;						//�ⲿ���õĹ��ܺ���(������˵���ں��氲װ������˵��)
}tLLedDriver;

#define	LLED_FLASHKEEP_CNT	0							//������˸(����ֵ����Ϊ0)

tLLedHandle* LLed_Initial(tLLedInitParam *pInitParam);
bool LLed_UnInitial(void);
#endif
