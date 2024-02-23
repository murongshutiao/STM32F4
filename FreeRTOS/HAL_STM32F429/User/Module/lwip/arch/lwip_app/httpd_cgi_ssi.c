
 
#include "lwip/debug.h"
#include "httpd.h"
#include "lwip/tcp.h"
#include "fs.h"
#include <string.h>
#include <stdlib.h>

#include "./led/bsp_led.h"


#define NUM_CONFIG_CGI_URIS     2       /* CGI��URI���� */
#define NUM_CONFIG_SSI_TAGS     4       /* SSI��TAG���� */


/* ����LED��BEEP��CGI handler */
const char *LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
const char *BEEP_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

static const char *ppcTAGs[] = /* SSI��Tag */
{
    "t", /* ADCֵ */
    "w", /* �¶�ֵ */
    "h", /* ʱ�� */
    "y"  /* ���� */
};

static const tCGI ppcURLs[] = /* cgi���� */
{
    {"/leds.cgi", LEDS_CGI_Handler},    
    {"/beep.cgi", BEEP_CGI_Handler},
};

/**
 * @breif       ��web�ͻ��������������ʱ��,ʹ�ô˺�����CGI handler����
 * @param       pcToFind    :
 * @param       pcParam     :
 * @param       iNumParams  :
 * @retval      ��
 */
static int FindCGIParameter(const char *pcToFind, char *pcParam[], int iNumParams)
{
    int iLoop;

    for (iLoop = 0; iLoop < iNumParams; iLoop++)
    {
        if (strcmp(pcToFind, pcParam[iLoop]) == 0)
        {
            return (iLoop);     /* ����iLOOP */
        }
    }

    return (-1);
}

/**
 * @breif       SSIHandler����Ҫ�õ��Ĵ���ADC�ĺ���
 * @param       pcInsert    :
 * @retval      ��
 */
void ADC_Handler(char *pcInsert)
{
//    char Digit1 = 0, Digit2 = 0, Digit3 = 0, Digit4 = 0;
//    uint32_t ADCVal = 0;

//    /* ��ȡADC��ֵ */
//    ADCVal = adc_get_result_average(5, 10); /* ��ȡADC1_CH5�ĵ�ѹֵ */

//    /* ת��Ϊ��ѹ ADCVval * 0.8mv */
//    ADCVal = (uint32_t)(ADCVal * 0.8);

//    Digit1 = ADCVal / 1000;
//    Digit2 = (ADCVal - (Digit1 * 1000)) / 100 ;
//    Digit3 = (ADCVal - ((Digit1 * 1000) + (Digit2 * 100))) / 10;
//    Digit4 = ADCVal - ((Digit1 * 1000) + (Digit2 * 100) + (Digit3 * 10));

//    /* ׼����ӵ�html�е����� */
//    *pcInsert       = (char)(Digit1 + 0x30);
//    *(pcInsert + 1) = (char)(Digit2 + 0x30);
//    *(pcInsert + 2) = (char)(Digit3 + 0x30);
//    *(pcInsert + 3) = (char)(Digit4 + 0x30);
}

/**
 * @breif       SSIHandler����Ҫ�õ��Ĵ����ڲ��¶ȴ������ĺ���
 * @param       pcInsert    :
 * @retval      ��
 */
void Temperate_Handler(char *pcInsert)
{
//    char Digit1 = 0, Digit2 = 0, Digit3 = 0, Digit4 = 0, Digit5 = 0;
//    short Temperate = 0;

//    /* ��ȡ�ڲ��¶�ֵ */
//    Temperate = adc_get_result(ADC_CHANNEL_0); /* ��ȡ�¶�ֵ �˴�������100�� */
//    Digit1 = Temperate / 10000;
//    Digit2 = (Temperate % 10000) / 1000;
//    Digit3 = (Temperate % 1000) / 100 ;
//    Digit4 = (Temperate % 100) / 10;
//    Digit5 = Temperate % 10;

//    /* ��ӵ�html�е����� */
//    *pcInsert = (char)(Digit1 + 0x30);
//    *(pcInsert + 1) = (char)(Digit2 + 0x30);
//    *(pcInsert + 2) = (char)(Digit3 + 0x30);
//    *(pcInsert + 3) = '.';
//    *(pcInsert + 4) = (char)(Digit4 + 0x30);
//    *(pcInsert + 5) = (char)(Digit5 + 0x30);
}

/**
 * @breif       SSIHandler����Ҫ�õ��Ĵ���RTCʱ��ĺ���
 * @param       pcInsert    :
 * @retval      ��
 */
void RTCTime_Handler(char *pcInsert)
{
//    RTC_TimeTypeDef RTC_TimeStruct;
//    uint8_t hour, min, sec;

//    HAL_RTC_GetTime(&g_rtc_handle, &RTC_TimeStruct, RTC_FORMAT_BIN);
//    hour = RTC_TimeStruct.Hours;
//    min = RTC_TimeStruct.Minutes;
//    sec = RTC_TimeStruct.Seconds;

//    *pcInsert = (char)((hour / 10) + 0x30);
//    *(pcInsert + 1) = (char)((hour % 10) + 0x30);
//    *(pcInsert + 2) = ':';
//    *(pcInsert + 3) = (char)((min / 10) + 0x30);
//    *(pcInsert + 4) = (char)((min % 10) + 0x30);
//    *(pcInsert + 5) = ':';
//    *(pcInsert + 6) = (char)((sec / 10) + 0x30);
//    *(pcInsert + 7) = (char)((sec % 10) + 0x30);
}

/**
 * @breif       SSIHandler����Ҫ�õ��Ĵ���RTC���ڵĺ���
 * @param       pcInsert    :
 * @retval      ��
 */
void RTCdate_Handler(char *pcInsert)
{
//    uint8_t year, month, date, week;
//    RTC_DateTypeDef RTC_DateStruct;

//    HAL_RTC_GetDate(&g_rtc_handle, &RTC_DateStruct, RTC_FORMAT_BIN);
//    year = RTC_DateStruct.Year;
//    month = RTC_DateStruct.Month;
//    date = RTC_DateStruct.Date;
//    week = RTC_DateStruct.WeekDay;

//    *pcInsert = '2';
//    *(pcInsert + 1) = '0';
//    *(pcInsert + 2) = (char)((year / 10) + 0x30);
//    *(pcInsert + 3) = (char)((year % 10) + 0x30);
//    *(pcInsert + 4) = '-';
//    *(pcInsert + 5) = (char)((month / 10) + 0x30);
//    *(pcInsert + 6) = (char)((month % 10) + 0x30);
//    *(pcInsert + 7) = '-';
//    *(pcInsert + 8) = (char)((date / 10) + 0x30);
//    *(pcInsert + 9) = (char)((date % 10) + 0x30);
//    *(pcInsert + 10) = ' ';
//    *(pcInsert + 11) = 'w';
//    *(pcInsert + 12) = 'e';
//    *(pcInsert + 13) = 'e';
//    *(pcInsert + 14) = 'k';
//    *(pcInsert + 15) = ':';
//    *(pcInsert + 16) = (char)(week + 0x30);
}

/**
 * @breif       SSI��Handler���
 * @param       iIndex      :
 * @param       pcInsert    :
 * @param       iInsertLen  :
 * @retval      ��
 */
static u16_t SSIHandler(int iIndex, char *pcInsert, int iInsertLen)
{
    switch (iIndex)
    {
        case 0:
            ADC_Handler(pcInsert);
            break;

        case 1:
            Temperate_Handler(pcInsert);
            break;

        case 2:
            RTCTime_Handler(pcInsert);
            break;

        case 3:
            RTCdate_Handler(pcInsert);
            break;
    }

    return strlen(pcInsert);
}

/**
 * @breif       CGI LED���ƾ��
 * @param       iIndex      : CGI���������
 * @param       iNumParams  : URI����������
 * @param       pcParam     : �������б� 
 * @param       pcValue     : ����ֵ
 * @retval      ��ת����ҳ
 */
const char *LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    uint8_t i = 0; /* ע������Լ���GET�Ĳ����Ķ�����ѡ��iֵ��Χ */

    iIndex = FindCGIParameter("LED1", pcParam, iNumParams);   /* �ҵ�led�������� */

    /* ֻ��һ��CGI��� iIndex=0 */
    if (iIndex != -1)
    {
        // LED1(1);    /* �ر����е�LED1�� */
        LED_RGBOFF;

        for (i = 0; i < iNumParams; i++)                /* ���CGI����: example GET /leds.cgi?led=2&led=4 */
        {
            if (strcmp(pcParam[i], "LED1") == 0)        /* ������"led" */
            {
                if (strcmp(pcValue[i], "LED1ON") == 0)  /* �ı�LED1״̬ */
                {
                    // LED1(0);    /* ��LED1 */
                    LED1_ON;
                    
                }
                else if (strcmp(pcValue[i], "LED1OFF") == 0)
                {
                    // LED1(1);    /* �ر�LED1 */
                    LED1_OFF;
                }
            }
        }
    }

    if (HAL_GPIO_ReadPin(LED1_GPIO_PORT,LED1_PIN) == 0)
    {
        return "/STM32F407LED_ON_BEEP_OFF.shtml";   /* LED1��,BEEP�� */
    }
    else if (HAL_GPIO_ReadPin(LED1_GPIO_PORT,LED1_PIN) == 0)
    {
        return "/STM32F407LED_ON_BEEP_ON.shtml";    /* LED1��,BEEP�� */
    }
    else if (HAL_GPIO_ReadPin(LED1_GPIO_PORT,LED1_PIN) == 1)
    {
        return "/STM32F407LED_OFF_BEEP_ON.shtml";   /* LED1��,BEEP�� */
    }
    else
    {
        return "/STM32F407LED_OFF_BEEP_OFF.shtml";  /*  LED1��,BEEP�� */
    }
}

/**
 * @breif       BEEP��CGI���ƾ��
 * @param       iIndex      : CGI���������
 * @param       iNumParams  :
 * @param       pcParam     :
 * @param       pcValue     :
 * @retval      ��
 */
const char *BEEP_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    uint8_t i = 0;

    iIndex = FindCGIParameter("BEEP", pcParam, iNumParams);   /* �ҵ�BEEP�������� */

//    if (iIndex != -1)   /* �ҵ�BEEP������ */
//    {
//        pcf8574_write_bit(BEEP_IO,0);        /* �ر� */

//        for (i = 0; i < iNumParams; i++)
//        {
//            if (strcmp(pcParam[i], "BEEP") == 0)             /* ����CGI���� */
//            {
//                if (strcmp(pcValue[i], "BEEPON") == 0)       /* ��BEEP */
//                {
//                    pcf8574_write_bit(BEEP_IO,0);
//                }
//                else if (strcmp(pcValue[i], "BEEPOFF") == 0) /* �ر�BEEP */
//                {
//                    pcf8574_write_bit(BEEP_IO,1);
//                }
//            }
//        }
//    }

//    if (HAL_GPIO_ReadPin(LED1_GPIO_PORT,LED1_GPIO_PIN) == 0 && pcf8574_read_bit(BEEP_IO) == 0)
//    {
//        return "/STM32F407LED_ON_BEEP_OFF.shtml";   /* LED1��,BEEP�� */
//    }
//    else if (HAL_GPIO_ReadPin(LED1_GPIO_PORT,LED1_GPIO_PIN) == 0 && pcf8574_read_bit(BEEP_IO) == 1)
//    {
//        return "/STM32F407LED_ON_BEEP_ON.shtml";    /* LED1��,BEEP�� */
//    }
//    else if (HAL_GPIO_ReadPin(LED1_GPIO_PORT,LED1_GPIO_PIN) == 1 && pcf8574_read_bit(BEEP_IO) == 1)
//    {
//        return "/STM32F407LED_OFF_BEEP_ON.shtml";   /* LED1��,BEEP�� */
//    }
//    else
//    {
//        return "/STM32F407LED_OFF_BEEP_OFF.shtml";  /* LED1��,BEEP�� */
//    }
}

/**
 * @breif       SSI�����ʼ��
 * @param       ��
 * @retval      ��
 */
void httpd_ssi_init(void)
{
    http_set_ssi_handler(SSIHandler, ppcTAGs, NUM_CONFIG_SSI_TAGS);   /* ����SSI��� */
}

/**
 * @breif       CGI�����ʼ��
 * @param       ��
 * @retval      ��
 */
void httpd_cgi_init(void)
{
    http_set_cgi_handlers(ppcURLs, NUM_CONFIG_CGI_URIS);      /* ����CGI��� */
}


