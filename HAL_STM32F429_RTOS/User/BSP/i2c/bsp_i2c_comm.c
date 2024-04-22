#include "./i2c/bsp_i2c_comm.h"


/* Buffer used for transmission */
uint8_t aTxBuffer[] = " ****I2C_TwoBoards communication based on Polling****  ****I2C_TwoBoards communication based on Polling****  ****I2C_TwoBoards communication based on Polling**** ";

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

/* Size of Transmission buffer */
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE
  
  
 /* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];




I2C_HandleTypeDef I2CMasterHandler;
I2C_HandleTypeDef I2CSlaveHandler;

void I2C_Master_Comm_Init(void)
{
    /*##-1- Configure the I2C peripheral #######################################*/
    I2CMasterHandler.Instance             = Master_I2C;
    
    I2CMasterHandler.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    I2CMasterHandler.Init.ClockSpeed      = 400000;
    I2CMasterHandler.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    I2CMasterHandler.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
    I2CMasterHandler.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    I2CMasterHandler.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
    I2CMasterHandler.Init.OwnAddress1     = MASTER_I2C_ADDRESS;
    I2CMasterHandler.Init.OwnAddress2     = 0xFE;
    
    if(HAL_I2C_Init(&I2CMasterHandler) != HAL_OK)
    {
        /* Initialization Error */
        // Error_Handler();    
    }
}

void I2C_Slave_Comm_Init(void)
{
    /*##-1- Configure the I2C peripheral #######################################*/
    I2CSlaveHandler.Instance             = Slave_I2C;
    
    I2CSlaveHandler.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    I2CSlaveHandler.Init.ClockSpeed      = 400000;
    I2CSlaveHandler.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    I2CSlaveHandler.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
    I2CSlaveHandler.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    I2CSlaveHandler.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
    I2CSlaveHandler.Init.OwnAddress1     = MASTER_I2C_ADDRESS;
    I2CSlaveHandler.Init.OwnAddress2     = 0xFE;
    
    if(HAL_I2C_Init(&I2CSlaveHandler) != HAL_OK)
    {
        /* Initialization Error */
        // Error_Handler();    
    }
}


void I2C_Master_Test(void)
{

    /* Timeout is set to 10S */
    while(HAL_I2C_Master_Transmit(&I2CMasterHandler, (uint16_t)MASTER_I2C_ADDRESS + 1, (uint8_t*)aTxBuffer, TXBUFFERSIZE, 10000)!= HAL_OK)
    {
        /* Error_Handler() function is called when Timeout error occurs.
        When Acknowledge failure occurs (Slave don't acknowledge it's address)
        Master restarts communication */
        if (HAL_I2C_GetError(&I2CMasterHandler) != HAL_I2C_ERROR_AF)
        {
        //   Error_Handler();
        }
    }

    /* Timeout is set to 10S */ 
    while(HAL_I2C_Master_Receive(&I2CMasterHandler, (uint16_t)MASTER_I2C_ADDRESS + 1, (uint8_t *)aRxBuffer, RXBUFFERSIZE, 10000) != HAL_OK)
    {
        /* Error_Handler() function is called when Timeout error occurs.
        When Acknowledge failure occurs (Slave don't acknowledge it's address)
        Master restarts communication */
        if (HAL_I2C_GetError(&I2CMasterHandler) != HAL_I2C_ERROR_AF)
        {
//        Error_Handler();
        }   
    }
}


void I2C_Slave_Test(void)
{ 
    /* Timeout is set to 10S  */
    if(HAL_I2C_Slave_Receive(&I2CSlaveHandler, (uint8_t *)aRxBuffer, RXBUFFERSIZE, 10000) != HAL_OK)
    {
       
    }


        /*##-3- Start the transmission process #####################################*/  
    /* While the I2C in reception process, user can transmit data through 
        "aTxBuffer" buffer */
    /* Timeout is set to 10S */
    if(HAL_I2C_Slave_Transmit(&I2CSlaveHandler, (uint8_t*)aTxBuffer, TXBUFFERSIZE, 10000)!= HAL_OK)
    {
    
    }

}











