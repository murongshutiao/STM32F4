#include "./Sensor/mpu6050/bsp_i2c_mpu6050.h"

  
I2C_HandleTypeDef I2C_Handle;					
/*******************************  Function ************************************/


#define MPU_ERROR 		I2C_ERROR
#define MPU_INFO 		I2C_INFO



/**
  * @brief  ��ʼ��I2C���ߣ�ʹ��I2Cǰ��Ҫ����
  * @param  ��
  * @retval ��
  */
void MPU6050_I2C_Init(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

	MPU6050_I2C_RCC_CLK_ENABLE();
	MPU6050_I2C_SCL_GPIO_CLK_ENABLE();
	MPU6050_I2C_SDA_GPIO_CLK_ENABLE();

	/* ����I2Cx����: SCL ----------------------------------------*/
	GPIO_InitStructure.Pin =  MPU6050_I2C_SCL_GPIO_PIN; 
	GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull= GPIO_NOPULL;
	GPIO_InitStructure.Alternate=MPU6050_I2C_AF; 
	HAL_GPIO_Init(MPU6050_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

	/* ����I2Cx����: SDA ----------------------------------------*/
	GPIO_InitStructure.Pin = MPU6050_I2C_SDA_GPIO_PIN;  
	HAL_GPIO_Init(MPU6050_I2C_SDA_GPIO_PORT, &GPIO_InitStructure); 
	
	if(HAL_I2C_GetState(&I2C_Handle) == HAL_I2C_STATE_RESET)
	{	
		/* ǿ�Ƹ�λI2C����ʱ�� */  
		MPU6050_I2C_FORCE_RESET(); 

		/* �ͷ�I2C����ʱ�Ӹ�λ */  
		MPU6050_I2C_RELEASE_RESET(); 
		
		/* I2C ���� */
		I2C_Handle.Instance = MPU6050_I2C;
		I2C_Handle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
		I2C_Handle.Init.ClockSpeed      = 400000;
		I2C_Handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
		I2C_Handle.Init.DutyCycle       = I2C_DUTYCYCLE_2;
		I2C_Handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
		I2C_Handle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
		I2C_Handle.Init.OwnAddress1     = 0;
		I2C_Handle.Init.OwnAddress2     = 0;     

		HAL_I2C_Init(&I2C_Handle);	
		/* ʹ��ģ���˲��� */
		HAL_I2CEx_AnalogFilter_Config(&I2C_Handle, I2C_ANALOGFILTER_ENABLE); 
	}
}



/**
  * @brief  Manages error callback by re-initializing I2C.
  * @param  Addr: I2C Address
  * @retval None
  */
static void I2Cx_Error(uint8_t Addr)
{
	/* �ָ�I2C�Ĵ���ΪĬ��ֵ */
	HAL_I2C_DeInit(&I2C_Handle); 
	/* ���³�ʼ��I2C���� */
	MPU6050_I2C_Init();
}



/**
  * @brief  д�Ĵ����������ṩ���ϲ�Ľӿ�
  * @param  slave_addr: �ӻ���ַ
  * @param 	reg_addr:�Ĵ�����ַ
  * @param  len��д��ĳ���
  * @param  data_ptr:ָ��Ҫд�������
  * @retval ����Ϊ0��������Ϊ��0
  */
int MPU6050_I2C_WriteRegister(unsigned char slave_addr,
                                        unsigned char reg_addr,
                                        unsigned short len, 
                                        unsigned char *data_ptr)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Write(&I2C_Handle, slave_addr, reg_addr, I2C_MEMADD_SIZE_8BIT,data_ptr, len,I2Cx_FLAG_TIMEOUT); 

	/* ���ͨѶ״̬ */
	if(status != HAL_OK)
	{
		/* ���߳����� */
		I2Cx_Error(slave_addr);
	}

	while (HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY)
	{
		
	}

	/* ���SENSOR�Ƿ����������һ�ζ�д���� */
	while (HAL_I2C_IsDeviceReady(&I2C_Handle, slave_addr, I2Cx_FLAG_TIMEOUT, I2Cx_FLAG_TIMEOUT) == HAL_TIMEOUT);

	/* �ȴ�������� */
	while (HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY)
	{
		
	}
	return status;
}



/**
  * @brief  ���Ĵ����������ṩ���ϲ�Ľӿ�
	* @param  slave_addr: �ӻ���ַ
	* @param 	reg_addr:�Ĵ�����ַ
	* @param len��Ҫ��ȡ�ĳ���
	*	@param data_ptr:ָ��Ҫ�洢���ݵ�ָ��
  * @retval ����Ϊ0��������Ϊ��0
  */
int MPU6050_I2C_ReadRegister(unsigned char slave_addr,
                                       unsigned char reg_addr,
                                       unsigned short len, 
                                       unsigned char *data_ptr)
{
	HAL_StatusTypeDef status = HAL_OK;
	status =HAL_I2C_Mem_Read(&I2C_Handle,slave_addr,reg_addr,I2C_MEMADD_SIZE_8BIT,data_ptr,len,I2Cx_FLAG_TIMEOUT);    
	/* ���ͨѶ״̬ */
	if(status != HAL_OK)
	{
		/* ���߳����� */
		I2Cx_Error(slave_addr);
	}
	while (HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY)
	{
		
	}
	/* ���SENSOR�Ƿ����������һ�ζ�д���� */
	while (HAL_I2C_IsDeviceReady(&I2C_Handle, slave_addr, I2Cx_FLAG_TIMEOUT, I2Cx_FLAG_TIMEOUT) == HAL_TIMEOUT);
	/* �ȴ�������� */
	while (HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY)
	{
		
	}
	return status;
}



/**
  * @brief   д���ݵ�MPU6050�Ĵ���
  * @param   reg_add:�Ĵ�����ַ
	* @param	 reg_data:Ҫд�������
  * @retval  
  */
void MPU6050_WriteReg(uint8_t reg_add,uint8_t reg_dat)
{
	MPU6050_I2C_WriteRegister(MPU6050_ADDRESS,reg_add,1,&reg_dat);
}

/**
  * @brief   ��MPU6050�Ĵ�����ȡ����
  * @param   reg_add:�Ĵ�����ַ
	* @param	 Read���洢���ݵĻ�����
	* @param	 num��Ҫ��ȡ��������
  * @retval  
  */
void MPU6050_ReadData(uint8_t reg_add,unsigned char* Read,uint8_t num)
{
	MPU6050_I2C_ReadRegister(MPU6050_ADDRESS,reg_add,num,Read);
}


/**
  * @brief   ��ʼ��MPU6050оƬ
  * @param   
  * @retval  
  */
void MPU6050_Init(void)
{
	MPU6050_I2C_Init();

	//�ڳ�ʼ��֮ǰҪ��ʱһ��ʱ�䣬��û����ʱ����ϵ�����ϵ����ݿ��ܻ����
	Delay(100);
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	     //�������״̬
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //�����ǲ�����
	MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x01);	  //���ü��ٶȴ�����������16Gģʽ
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
	Delay(200);
}

/**
  * @brief   ��ȡMPU6050��ID
  * @param   
  * @retval  ��������1���쳣����0
  */
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //��������ַ
	if(Re != 0x68)
	{
		MPU_ERROR("MPU6050 dectected error!\r\n��ⲻ��MPU6050ģ�飬����ģ���뿪����Ľ���");
		return 0;
	}
	else
	{
		MPU_INFO("MPU6050 ID = %d\r\n",Re);
		return 1;
	}
		
}



/**
  * @brief   ��ȡMPU6050�ļ��ٶ�����
  * @param   
  * @retval  
  */
void MPU6050ReadAcc(short *accData)
{
    uint8_t buf[6];
    MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
    accData[0] = (buf[0] << 8) | buf[1];	/* X */
    accData[1] = (buf[2] << 8) | buf[3];	/* Y */
    accData[2] = (buf[4] << 8) | buf[5];	/* Z */
}



/**
  * @brief   ��ȡMPU6050�ĽǼ��ٶ�����
  * @param   
  * @retval  
  */
void MPU6050ReadGyro(short *gyroData)
{
    uint8_t buf[6];
    MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
    gyroData[0] = (buf[0] << 8) | buf[1];	/* X */
    gyroData[1] = (buf[2] << 8) | buf[3];	/* Y */
    gyroData[2] = (buf[4] << 8) | buf[5];	/* Z */
}



/**
  * @brief   ��ȡMPU6050��ԭʼ�¶�����
  * @param   
  * @retval  
  */
void MPU6050ReadTemp(short *tempData)
{
	uint8_t buf[2];
    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
    *tempData = (buf[0] << 8) | buf[1];
}

/**
  * @brief   ��ȡMPU6050���¶����ݣ�ת�������϶�
  * @param   
  * @retval  
  */
void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	uint8_t buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
    temp3= (buf[0] << 8) | buf[1];	
	*Temperature=((double) temp3/340.0)+36.53;

}



void MPU6050_Test_CallBack(void)
{
	short Acel[3];	/* ���ٶ� */
	short Gyro[3];	/* �Ǽ��ٶ� */
	float Temp;		/* �¶� */

	MPU6050ReadAcc(Acel);
	MPU6050ReadGyro(Gyro);
	MPU6050_ReturnTemp(&Temp);

}

void MPU6050_Test(void)
{

	if(MPU6050ReadID() == 1)
	{
		/* ������ʱ�� */
		LED_RED;
		multi_timer_start(&Timer_MPU6050_Handler,5000,Timer_MPU6050_CallBack,&MPU6050_Test_CallBack);
	}
	else
	{
		LED_GREEN;
	}


}




