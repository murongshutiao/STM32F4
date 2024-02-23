#include "./sdram/bsp_fmc_sdram.h"

SDRAM_HandleTypeDef SdramHandler;

static FMC_SDRAM_CommandTypeDef Sdram_Cmd;


/**
  * @brief  �ӳ�һ��ʱ��
  * @param  �ӳٵ�ʱ�䳤��
  * @retval None
  */
static void SDRAM_delay(__IO uint32_t nCount)
{
    __IO uint32_t index = 0; 
    for(index = (100000 * nCount); index != 0; index--)
    {
    }
}




/**
  * @brief  ��ʼ������SDRAM��IO
  * @param  ��
  * @retval ��
  */
static void SDRAM_GPIO_Config(void)
{		
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* ʹ��SDRAM��ص�GPIOʱ�� */

    /*��ַ�ź���*/
    FMC_A0_GPIO_CLK; FMC_A1_GPIO_CLK;  FMC_A2_GPIO_CLK;
    FMC_A3_GPIO_CLK; FMC_A4_GPIO_CLK;FMC_A5_GPIO_CLK;
    FMC_A6_GPIO_CLK; FMC_A7_GPIO_CLK; FMC_A8_GPIO_CLK;
    FMC_A9_GPIO_CLK; FMC_A10_GPIO_CLK;FMC_A11_GPIO_CLK; 
    /*�����ź���*/
    FMC_D0_GPIO_CLK; FMC_D1_GPIO_CLK ; FMC_D2_GPIO_CLK ; 
    FMC_D3_GPIO_CLK ; FMC_D4_GPIO_CLK ; FMC_D5_GPIO_CLK ;
    FMC_D6_GPIO_CLK; FMC_D7_GPIO_CLK ; FMC_D8_GPIO_CLK ;
    FMC_D9_GPIO_CLK ;FMC_D10_GPIO_CLK; FMC_D11_GPIO_CLK;
    FMC_D12_GPIO_CLK; FMC_D13_GPIO_CLK; FMC_D14_GPIO_CLK;
    FMC_D15_GPIO_CLK;  
    /*�����ź���*/
    FMC_CS_GPIO_CLK ; FMC_BA0_GPIO_CLK; FMC_BA1_GPIO_CLK ;
    FMC_WE_GPIO_CLK ; FMC_RAS_GPIO_CLK ; FMC_CAS_GPIO_CLK;
    FMC_CLK_GPIO_CLK ; FMC_CKE_GPIO_CLK; FMC_UDQM_GPIO_CLK;
    FMC_LDQM_GPIO_CLK;


    /*-- GPIO ���� -----------------------------------------------------*/

    /* ͨ�� GPIO ���� */       
    GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;//����Ϊ���ù���
    GPIO_InitStructure.Pull      = GPIO_PULLUP;
    GPIO_InitStructure.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStructure.Alternate = GPIO_AF12_FMC;
    
    /*��ַ�ź��� �����������*/
    GPIO_InitStructure.Pin = FMC_A0_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A0_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = FMC_A1_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A1_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = FMC_A2_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A2_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = FMC_A3_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A3_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = FMC_A4_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A4_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.Pin = FMC_A5_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A5_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A6_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A6_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A7_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A7_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A8_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A8_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A9_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A9_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A10_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A10_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_A11_GPIO_PIN; 
    HAL_GPIO_Init(FMC_A11_GPIO_PORT, &GPIO_InitStructure);

    
    
    /*�����ź��� �����������*/
    GPIO_InitStructure.Pin = FMC_D0_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D0_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D1_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D1_GPIO_PORT, &GPIO_InitStructure);
 
    GPIO_InitStructure.Pin = FMC_D2_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D2_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D3_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D3_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D4_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D4_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D5_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D5_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D6_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D6_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D7_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D7_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D8_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D8_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D9_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D9_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D10_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D10_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D11_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D11_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D12_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D12_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D13_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D13_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D14_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D14_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_D15_GPIO_PIN; 
    HAL_GPIO_Init(FMC_D15_GPIO_PORT, &GPIO_InitStructure);

    
    /*�����ź���*/
    GPIO_InitStructure.Pin = FMC_CS_GPIO_PIN; 
    HAL_GPIO_Init(FMC_CS_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_BA0_GPIO_PIN; 
    HAL_GPIO_Init(FMC_BA0_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_BA1_GPIO_PIN;
    HAL_GPIO_Init(FMC_BA1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_WE_GPIO_PIN; 
    HAL_GPIO_Init(FMC_WE_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_RAS_GPIO_PIN; 
    HAL_GPIO_Init(FMC_RAS_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_CAS_GPIO_PIN; 
    HAL_GPIO_Init(FMC_CAS_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_CLK_GPIO_PIN; 
    HAL_GPIO_Init(FMC_CLK_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_CKE_GPIO_PIN; 
    HAL_GPIO_Init(FMC_CKE_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_UDQM_GPIO_PIN; 
    HAL_GPIO_Init(FMC_UDQM_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = FMC_LDQM_GPIO_PIN; 
    HAL_GPIO_Init(FMC_LDQM_GPIO_PORT, &GPIO_InitStructure);
}


/**
  * @brief  ��SDRAMоƬ���г�ʼ������
  * @param  None. 
  * @retval None.
  */
static void SDRAM_InitSequence(void)
{
    uint32_t tmpr = 0;
    
    /* ������������ṩ��SDRAM��ʱ�� */
    Sdram_Cmd.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
    Sdram_Cmd.CommandTarget = FMC_COMMAND_TARGET_BANK;
    Sdram_Cmd.AutoRefreshNumber = 1;
    Sdram_Cmd.ModeRegisterDefinition = 0;
    HAL_SDRAM_SendCommand(&SdramHandler, &Sdram_Cmd, SDRAM_TIMEOUT);

    /* Step 2: Insert 100 us minimum delay */ 
    /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
    SDRAM_delay(1);
        

    /* ������������е�bankԤ��� */ 
    Sdram_Cmd.CommandMode = FMC_SDRAM_CMD_PALL;
    Sdram_Cmd.CommandTarget = FMC_COMMAND_TARGET_BANK;
    Sdram_Cmd.AutoRefreshNumber = 1;
    Sdram_Cmd.ModeRegisterDefinition = 0;
    HAL_SDRAM_SendCommand(&SdramHandler, &Sdram_Cmd, SDRAM_TIMEOUT);   
    
    /* ��������Զ�ˢ�� */   
    Sdram_Cmd.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
    Sdram_Cmd.CommandTarget = FMC_COMMAND_TARGET_BANK;
    Sdram_Cmd.AutoRefreshNumber = 4;
    Sdram_Cmd.ModeRegisterDefinition = 0;
    HAL_SDRAM_SendCommand(&SdramHandler, &Sdram_Cmd, SDRAM_TIMEOUT);
    
    /* ����sdram�Ĵ������� */
    tmpr = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_2          |
                    SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                    SDRAM_MODEREG_CAS_LATENCY_3           |
                    SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                    SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
    
    /* �����������SDRAM�Ĵ��� */
    Sdram_Cmd.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
    Sdram_Cmd.CommandTarget = FMC_COMMAND_TARGET_BANK;
    Sdram_Cmd.AutoRefreshNumber = 1;
    Sdram_Cmd.ModeRegisterDefinition = tmpr;
    HAL_SDRAM_SendCommand(&SdramHandler, &Sdram_Cmd, SDRAM_TIMEOUT);
  

    /* ����ˢ�¼����� */
    /* (15.62 us x Freq) - 20 */
        /* Step 6: Set the refresh rate counter */
    /* Set the device refresh rate */
    HAL_SDRAM_ProgramRefreshRate(&SdramHandler, 1386); 

}



/**
  * @brief  ��ʼ������ʹ��SDRAM��FMC��GPIO�ӿڣ�
  *         ��������SDRAM��д����ǰ��Ҫ������
  * @param  None
  * @retval None
  */
void SDRAM_Init(void)
{
  
    /* ����FMC�ӿ���ص� GPIO*/
    SDRAM_GPIO_Config();
    
    /* ʹ�� FMC ʱ�� */
    __HAL_RCC_FMC_CLK_ENABLE();

    FMC_SDRAM_TimingTypeDef SdramTiming;

    /** Perform the SDRAM1 memory initialization sequence
     */
    SdramHandler.Instance = FMC_SDRAM_DEVICE;
    SdramHandler.Init.SDBank = FMC_SDRAM_BANK2;      /* ����ΪFMC_BANK2 */
    SdramHandler.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;    /* �п� */
    SdramHandler.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;         /* �е�ַ��� */
    SdramHandler.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;      /* ���ݿ�� */
    SdramHandler.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4; /* SDRAM�ڲ���bank���� */
    SdramHandler.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;              /* CADLatencyʱ�Ӹ��� */
    SdramHandler.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;  /* ��ʹ��д���� */
    SdramHandler.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;          /* ����ͬ��ʱ�Ӳ��� */
    SdramHandler.Init.ReadBurst = FMC_SDRAM_RBURST_DISABLE;              /* ʹ��ͻ����ģʽ */
    SdramHandler.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1;           /* ����CAS���ӳٶ�����HCLKʱ�Ӻ��ٶ�ȡ����*/
    
    /* SdramTiming */
    SdramTiming.LoadToActiveDelay = 2;
    SdramTiming.ExitSelfRefreshDelay = 7;
    SdramTiming.SelfRefreshTime = 4;
    SdramTiming.RowCycleDelay = 7;
    SdramTiming.WriteRecoveryTime = 2;
    SdramTiming.RPDelay = 2;
    SdramTiming.RCDDelay = 2;

    HAL_SDRAM_Init(&SdramHandler, &SdramTiming);  

    /* FMC SDRAM device initialization sequence */
    SDRAM_InitSequence(); /* ��ʼ��Sdram�豸 */
  
}


/**
  * @brief  �ԡ��֡�Ϊ��λ��sdramд������ 
  * @param  pBuffer: ָ�����ݵ�ָ�� 
  * @param  uwWriteAddress: Ҫд���SDRAM�ڲ���ַ
  * @param  uwBufferSize: Ҫд�����ݴ�С
  * @retval None.
  */
void SDRAM_WriteBuffer(uint32_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize)
{
    __IO uint32_t write_pointer = (uint32_t)uwWriteAddress;

    /* ��ֹд���� */
    HAL_SDRAM_WriteProtection_Disable(&SdramHandler);
    /* ���SDRAM��־���ȴ���SDRAM���� */ 
    while(HAL_SDRAM_GetState(&SdramHandler) != RESET)
    {
    }

    /* ѭ��д������ */
    for (; uwBufferSize != 0; uwBufferSize--) 
    {
        /* �������ݵ�SDRAM */
        *(uint32_t *) (SDRAM_BANK_ADDR + write_pointer) = *pBuffer++;

        /* ��ַ����*/
        write_pointer += 4;
    }
    
}

/**
  * @brief  ��SDRAM�ж�ȡ���� 
  * @param  pBuffer: ָ��洢���ݵ�buffer
  * @param  ReadAddress: Ҫ��ȡ���ݵĵ�ʮ
  * @param  uwBufferSize: Ҫ��ȡ�����ݴ�С
  * @retval None.
  */
void SDRAM_ReadBuffer(uint32_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize)
{
    __IO uint32_t write_pointer = (uint32_t)uwReadAddress;
    
    
    /* ���SDRAM��־���ȴ���SDRAM���� */  
    while ( HAL_SDRAM_GetState(&SdramHandler) != RESET)
    {
    }
    
    /*��ȡ���� */
    for(; uwBufferSize != 0x00; uwBufferSize--)
    {
        *pBuffer++ = *(__IO uint32_t *)(SDRAM_BANK_ADDR + write_pointer );
            
        /* ��ַ����*/
        write_pointer += 4;
    } 
}


/**
  * @brief  ����SDRAM�Ƿ����� 
  * @param  None
  * @retval ��������1���쳣����0
  */
uint8_t SDRAM_Test(void)
{
    uint32_t counter = 0;
    
    /* 8λ������ */
    uint8_t ubWritedata_8b = 0, ubReaddata_8b = 0;  
    
    /* 16λ������ */
    uint16_t uhWritedata_16b = 0, uhReaddata_16b = 0; 
    
    SDRAM_INFO("���ڼ��SDRAM����8λ��16λ�ķ�ʽ��дsdram...");


    /*��8λ��ʽ��д���ݣ���У��*/
    
    /* ��SDRAM����ȫ������Ϊ0 ��IS42S16400J_SIZE����8λΪ��λ�� */
    for (counter = 0x00; counter < IS42S16400J_SIZE; counter++)
    {
        *(__IO uint8_t*) (SDRAM_BANK_ADDR + counter) = (uint8_t)0x0;
    }
    
    /* ������SDRAMд������  8λ */
    for (counter = 0; counter < IS42S16400J_SIZE; counter++)
    {
        *(__IO uint8_t*) (SDRAM_BANK_ADDR + counter) = (uint8_t)(ubWritedata_8b + counter);
    }
    
    /* ��ȡ SDRAM ���ݲ����*/
    for(counter = 0; counter<IS42S16400J_SIZE;counter++ )
    {
        ubReaddata_8b = *(__IO uint8_t*)(SDRAM_BANK_ADDR + counter);  //�Ӹõ�ַ��������
        
        if(ubReaddata_8b != (uint8_t)(ubWritedata_8b + counter))      //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
        {
            SDRAM_ERROR("8λ���ݶ�д����");
            return 0;
        }
    }
        
    
    /*��16λ��ʽ��д���ݣ������*/
    
    /* ��SDRAM����ȫ������Ϊ0 */
    for (counter = 0x00; counter < IS42S16400J_SIZE/2; counter++)
    {
        *(__IO uint16_t*) (SDRAM_BANK_ADDR + 2*counter) = (uint16_t)0x00;
    }
    
    /* ������SDRAMд������  16λ */
    for (counter = 0; counter < IS42S16400J_SIZE/2; counter++)
    {
        *(__IO uint16_t*) (SDRAM_BANK_ADDR + 2*counter) = (uint16_t)(uhWritedata_16b + counter);
    }
    
        /* ��ȡ SDRAM ���ݲ����*/
    for(counter = 0; counter<IS42S16400J_SIZE/2;counter++ )
    {
        uhReaddata_16b = *(__IO uint16_t*)(SDRAM_BANK_ADDR + 2*counter);  //�Ӹõ�ַ��������
        
        if(uhReaddata_16b != (uint16_t)(uhWritedata_16b + counter))      //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
        {
            SDRAM_ERROR("16λ���ݶ�д����");

            return 0;
        }
    }

    
    SDRAM_INFO("SDRAM��д����������"); 
    /*���������return 1 */
    return 1;
  
}



