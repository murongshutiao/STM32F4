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
    GPIO_InitStructure.Pull = GPIO_NOPULL ; //������������
    HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT, &GPIO_InitStructure);		
}

static void Rheostat_ADC_Mode_Config(void)
{
#ifdef DMA_ADC_STREAM
    // ����DMAʱ��
    DMA_ADC_STREAM_CLOCK();
    
    DMA_Init_Handle.Instance = DMA_ADC_STREAM;// ���ݴ���ͨ��
    DMA_Init_Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;	// ���ݴ��䷽��Ϊ���赽�洢��
    DMA_Init_Handle.Init.PeriphInc = DMA_PINC_DISABLE;// ����Ĵ���ֻ��һ������ַ���õ���
    DMA_Init_Handle.Init.MemInc = DMA_MINC_ENABLE; // �洢����ַ�̶�
    DMA_Init_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; // �������ݴ�СΪ���֣��������ֽ� 
    DMA_Init_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;	//	�洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
    DMA_Init_Handle.Init.Mode = DMA_CIRCULAR;   // ѭ������ģʽ
    DMA_Init_Handle.Init.Priority = DMA_PRIORITY_HIGH;  // DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
    DMA_Init_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;  // ��ֹDMA FIFO	��ʹ��ֱ��ģʽ
    // FIFO ��С��FIFOģʽ��ֹʱ�������������	
    DMA_Init_Handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    DMA_Init_Handle.Init.MemBurst = DMA_MBURST_SINGLE;
    DMA_Init_Handle.Init.PeriphBurst = DMA_PBURST_SINGLE;  
    DMA_Init_Handle.Init.Channel = DMA_ADC_CHANNEL; // ѡ�� DMA ͨ����ͨ������������
    //��ʼ��DMA�������൱��һ����Ĺܵ����ܵ������кܶ�ͨ��
    HAL_DMA_Init(&DMA_Init_Handle); 
    HAL_DMA_Start (&DMA_Init_Handle,RHEOSTAT_ADC_DR_ADDR,(uint32_t)&ADC_ConvertedValue,1);
#endif

    RHEOSTAT_ADC_CLK_ENABLE();

    // -------------------ADC Init �ṹ�� ���� ��ʼ��------------------------
    ADC_Handle.Instance = RHEOSTAT_ADC;
    ADC_Handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;// ADCʱ�ӷ�Ƶ 4��Ƶ
    ADC_Handle.Init.Resolution = ADC_RESOLUTION_12B;// ADC �ֱ���
    ADC_Handle.Init.ScanConvMode = DISABLE; // ��ֹɨ��ģʽ����ͨ���ɼ�����Ҫ
    ADC_Handle.Init.ContinuousConvMode = ENABLE;// ����ת��	
    ADC_Handle.Init.DiscontinuousConvMode = DISABLE;// ������ת��	
    ADC_Handle.Init.NbrOfDiscConversion   = 0;// ������ת������
    ADC_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;//��ֹ�ⲿ���ش���
    //ʹ������������ⲿ�����������ã�ע�͵�����
    //ADC_Handle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
    ADC_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;//�����Ҷ���
    ADC_Handle.Init.NbrOfConversion = 1;//ת��ͨ�� 1��
    ADC_Handle.Init.DMAContinuousRequests = ENABLE;//ʹ������ת�����󣬵�ͨ��ʹ��DMA��Ӱ��
    ADC_Handle.Init.EOCSelection          = DISABLE;    //ת����ɱ�־                
    HAL_ADC_Init(&ADC_Handle);  // ��ʼ��ADC

    //---------------------------------------------------------------------------
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL;
    ADC_Config.Rank         = 1;                    //����˳��,��һ������
    ADC_Config.SamplingTime = ADC_SAMPLETIME_56CYCLES;// ����ʱ����	
    ADC_Config.Offset       = 0;        /* ��֪��ʲô�� */
    // ���� ADC ͨ��ת��˳��Ϊ1����һ��ת��������ʱ��Ϊ3��ʱ������
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config);
    HAL_ADC_Start_IT(&ADC_Handle);
}

//// �����ж����ȼ�
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
  * @brief  ת������жϻص�������������ģʽ��
  * @param  AdcHandle : ADC���
  * @retval ��
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
    /* ��ȡ��� */
    ADC_ConvertedValue = HAL_ADC_GetValue(AdcHandle);
}


void ADC_IRQHandler(void)
{
    HAL_ADC_IRQHandler(&ADC_Handle);
}

