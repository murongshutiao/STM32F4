#ifndef __BSP_RTC_H
#define __BSP_RTC_H
#include "board.h"

extern RTC_HandleTypeDef Rtc_Handle;
// 时钟源宏定义
#define RTC_CLOCK_SOURCE_LSE      
//#define RTC_CLOCK_SOURCE_LSI  

// 异步分频因子
#define ASYNCHPREDIV         0X7F
// 同步分频因子
#define SYNCHPREDIV          0XFF

// 时间宏定义
#define RTC_H12_AMorPM			  RTC_HOURFORMAT12_AM  
#define HOURS                     1          // 0~23
#define MINUTES                   1          // 0~59
#define SECONDS                   1          // 0~59


// 日期宏定义
#define WEEKDAY                   1         // 1~7
#define DATE                      1         // 1~31
#define MONTH                     1         // 1~12
#define YEAR                      1         // 0~99

// 时间格式宏定义
#define RTC_Format_BINorBCD  RTC_FORMAT_BIN

// 备份域寄存器宏定义
#define RTC_BKP_DRX          RTC_BKP_DR0
// 写入到备份寄存器的数据宏定义
#define RTC_BKP_DATA         0X32F2 
                                  
void RTC_CLK_Config(void);
void RTC_TimeAndDate_Set(void);
void RTC_TimeAndDate_Show(void);
void RTC_AlarmSet(void);
void RTC_Test(void);

#endif // __RTC_H__
