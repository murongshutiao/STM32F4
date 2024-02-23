
  
#include "./tim/bsp_timer.h"

TIM_HandleTypeDef TIM_TimeBaseStructure;
TIM_HandleTypeDef TIM_MultiTimerStructure;


 /**
  * @brief  ������ʱ�� TIMx,x[6,7]�ж����ȼ�����
  * @param  ��
  * @retval ��
  */
static void TIMx_NVIC_Configuration(void)
{
    //������ռ���ȼ��������ȼ�
    HAL_NVIC_SetPriority(BASIC_TIM_IRQn, 0, 3);
    // �����ж���Դ
    HAL_NVIC_EnableIRQ(BASIC_TIM_IRQn);
}

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
static void TIM_Mode_Config(void)
{
	// ����TIMx_CLK,x[6,7] 
	BASIC_TIM_CLK_ENABLE(); 

	TIM_TimeBaseStructure.Instance = BASIC_TIM;
	/* �ۼ� TIM_Period�������һ�����»����ж�*/		
	//����ʱ����0������4999����Ϊ5000�Σ�Ϊһ����ʱ����
	TIM_TimeBaseStructure.Init.Period = 5000 - 1;       

	//��ʱ��ʱ��ԴTIMxCLK = 2 * PCLK1  
	//				PCLK1 = HCLK / 4 
	//				=> TIMxCLK=HCLK/2=SystemCoreClock/2=108MHz
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1) = 10000Hz
	TIM_TimeBaseStructure.Init.Prescaler = 8400 - 1;	

	// ��ʼ����ʱ��TIMx, x[2,3,4,5]
	HAL_TIM_Base_Init(&TIM_TimeBaseStructure);

	// ������ʱ�������ж�
	HAL_TIM_Base_Start_IT(&TIM_TimeBaseStructure);	
}

/**
  * @brief  ��ʼ��������ʱ����ʱ��1ms����һ���ж�
  * @param  ��
  * @retval ��
  */
void TIMx_Configuration(void)
{
	TIMx_NVIC_Configuration();	
	TIM_Mode_Config();
}


/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */	
void  BASIC_TIM_IRQHandler (void)
{
	HAL_TIM_IRQHandler(&TIM_TimeBaseStructure);	 	
}


/******************************************************************/
 /**
  * @brief  ������ʱ�� TIMx,x[6,7]�ж����ȼ�����
  * @param  ��
  * @retval ��
  */
static void MultiTimer_TIM_NVIC_Configuration(void)
{
    HAL_NVIC_SetPriority(MultiTimer_TIM_IRQn, 0, 3);
    HAL_NVIC_EnableIRQ(MultiTimer_TIM_IRQn);
}

static void MultiTimer_TIM_Mode_Config(void)
{
	// ����TIMx_CLK,x[6,7] 
	MultiTimer_TIM_CLK_ENABLE(); 

	TIM_MultiTimerStructure.Instance = MultiTimer_TIM;
	TIM_MultiTimerStructure.Init.Period = 1;//50 - 1;       
	TIM_MultiTimerStructure.Init.Prescaler = 42000 - 1;	

	HAL_TIM_Base_Init(&TIM_MultiTimerStructure);
	HAL_TIM_Base_Start_IT(&TIM_MultiTimerStructure);	
}

/**
  * @brief  ��ʼ��������ʱ����ʱ��1ms����һ���ж�
  * @param  ��
  * @retval ��
  */
void MultiTimer_TIM_Init(void)
{
	MultiTimer_TIM_NVIC_Configuration();	
	MultiTimer_TIM_Mode_Config();
}


/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */	
void  MultiTimer_TIM_IRQHandler (void)
{
	HAL_TIM_IRQHandler(&TIM_MultiTimerStructure);	 	
}

uint64_t MultiTimer_ticks = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == (&TIM_TimeBaseStructure))
    {
        LED1_TOGGLE;  //���������˸
    }

    if(htim == (&TIM_MultiTimerStructure))
    {
        MultiTimer_ticks++;
    }
}

uint64_t GetMultiTimer_Ticks(void)
{
    return MultiTimer_ticks;
}

/*********************************************END OF FILE**********************/
