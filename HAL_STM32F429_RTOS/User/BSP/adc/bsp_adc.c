#include "./adc/bsp_adc.h"

__IO uint16_t ADC_ConvertedValue;
DMA_HandleTypeDef DMA_Init_Handle;
ADC_HandleTypeDef ADC_Handle;
ADC_ChannelConfTypeDef ADC_Config;

static void Rheostat_ADC_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RHEOSTAT_ADC_GPIO_CLK_ENABLE();
        
    GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;	    
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //不上拉不下拉
    HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT, &GPIO_InitStructure);		
}

static void Rheostat_ADC_Mode_Config(void)
{
#ifdef DMA_ADC_STREAM
    // 开启DMA时钟
    DMA_ADC_STREAM_CLOCK();
    
    DMA_Init_Handle.Instance = DMA_ADC_STREAM;// 数据传输通道
    DMA_Init_Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;	// 数据传输方向为外设到存储器
    DMA_Init_Handle.Init.PeriphInc = DMA_PINC_DISABLE;// 外设寄存器只有一个，地址不用递增
    DMA_Init_Handle.Init.MemInc = DMA_MINC_ENABLE; // 存储器地址固定
    DMA_Init_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; // 外设数据大小为半字，即两个字节 
    DMA_Init_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;	//	存储器数据大小也为半字，跟外设数据大小相同
    DMA_Init_Handle.Init.Mode = DMA_CIRCULAR;   // 循环传输模式
    DMA_Init_Handle.Init.Priority = DMA_PRIORITY_HIGH;  // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
    DMA_Init_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;  // 禁止DMA FIFO	，使用直连模式
    // FIFO 大小，FIFO模式禁止时，这个不用配置	
    DMA_Init_Handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    DMA_Init_Handle.Init.MemBurst = DMA_MBURST_SINGLE;
    DMA_Init_Handle.Init.PeriphBurst = DMA_PBURST_SINGLE;  
    DMA_Init_Handle.Init.Channel = DMA_ADC_CHANNEL; // 选择 DMA 通道，通道存在于流中
    //初始化DMA流，流相当于一个大的管道，管道里面有很多通道
    HAL_DMA_Init(&DMA_Init_Handle); 
    HAL_DMA_Start (&DMA_Init_Handle,RHEOSTAT_ADC_DR_ADDR,(uint32_t)&ADC_ConvertedValue,1);
#endif

    RHEOSTAT_ADC_CLK_ENABLE();

    // -------------------ADC Init 结构体 参数 初始化------------------------
    ADC_Handle.Instance = RHEOSTAT_ADC;
    ADC_Handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;// ADC时钟分频 4分频
    ADC_Handle.Init.Resolution = ADC_RESOLUTION_12B;// ADC 分辨率
    ADC_Handle.Init.ScanConvMode = DISABLE; // 禁止扫描模式，多通道采集才需要
    ADC_Handle.Init.ContinuousConvMode = ENABLE;// 连续转换	
    ADC_Handle.Init.DiscontinuousConvMode = DISABLE;// 非连续转换	
    ADC_Handle.Init.NbrOfDiscConversion   = 0;// 非连续转换个数
    ADC_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;//禁止外部边沿触发
    //使用软件触发，外部触发不用配置，注释掉即可
    //ADC_Handle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
    ADC_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;//数据右对齐
    ADC_Handle.Init.NbrOfConversion = 1;//转换通道 1个
    ADC_Handle.Init.DMAContinuousRequests = ENABLE;//使能连续转换请求，单通道使能DMA不影响
    ADC_Handle.Init.EOCSelection          = DISABLE;    //转换完成标志                
    HAL_ADC_Init(&ADC_Handle);  // 初始化ADC

    //---------------------------------------------------------------------------
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL;
    ADC_Config.Rank         = 1;                    //采样顺序,第一个采样
    ADC_Config.SamplingTime = ADC_SAMPLETIME_56CYCLES;// 采样时间间隔	
    ADC_Config.Offset       = 0;        /* 不知道什么用 */
    // 配置 ADC 通道转换顺序为1，第一个转换，采样时间为3个时钟周期
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
    HAL_ADC_Start_IT(&ADC_Handle);
}

//// 配置中断优先级
//static void Rheostat_ADC_NVIC_Config(void)
//{
//    HAL_NVIC_SetPriority(Rheostat_ADC_IRQ, 0, 0);
//    HAL_NVIC_EnableIRQ(Rheostat_ADC_IRQ);
//}

void Rheostat_Init(void)
{
	Rheostat_ADC_GPIO_Config();
	Rheostat_ADC_Mode_Config();
#ifndef DMA_ADC_STREAM
    Rheostat_ADC_NVIC_Config();
#endif
}

/**
  * @brief  转换完成中断回调函数（非阻塞模式）
  * @param  AdcHandle : ADC句柄
  * @retval 无
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
    /* 获取结果 */
    ADC_ConvertedValue = HAL_ADC_GetValue(AdcHandle);
}


void ADC_IRQHandler(void)
{
    HAL_ADC_IRQHandler(&ADC_Handle);
}

