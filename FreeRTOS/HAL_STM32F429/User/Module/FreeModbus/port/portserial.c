/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "./modbus/bsp_uart_modbus.h"
#include "stm32f4xx.h"
/* ----------------------- static functions ---------------------------------*/
//static void prvvUARTTxReadyISR( void );
//static void prvvUARTRxISR( void );

/* ----------------------- Start implementation -----------------------------*/

BOOL xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    /**
     * set 485 mode receive and transmit control IO
     * @note MODBUS_MASTER_RT_CONTROL_PIN_INDEX need be defined by user
     */
	
		/* ???485??????usart.h?��?RT_MODBUS_MASTER_USE_CONTROL_PIN???? */
	#if defined(MODBUS_MASTER_USE_CONTROL_PIN)	
		modbus_master_control_init();
	#endif
	ModbusSlave_Uart_Init(ulBaudRate);
	return TRUE;				
}

void vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
		if(xRxEnable)
		{
			LL_USART_EnableIT_RXNE(ModbusSlave_Uart);
			#if defined(MODBUS_MASTER_USE_CONTROL_PIN)	
				HAL_GPIO_WritePin(MODBUS_MASTER_GPIO_PORT,MODBUS_MASTER_GPIO_PIN,MODBUS_MASTER_GPIO_PIN_LOW);
			#endif
		}
		else
		{
			LL_USART_DisableIT_RXNE(ModbusSlave_Uart);
			#if defined(MODBUS_MASTER_USE_CONTROL_PIN)
				HAL_GPIO_WritePin(MODBUS_MASTER_GPIO_PORT,MODBUS_MASTER_GPIO_PIN,MODBUS_MASTER_GPIO_PIN_HIGH);
			#endif
		}
			
		if(xTxEnable)
		{
			LL_USART_EnableIT_TXE(ModbusSlave_Uart);
			#if defined(MODBUS_MASTER_USE_CONTROL_PIN)
				HAL_GPIO_WritePin(MODBUS_MASTER_GPIO_PORT,MODBUS_MASTER_GPIO_PIN,MODBUS_MASTER_GPIO_PIN_HIGH);
			#endif
		}
		else
		{
			LL_USART_DisableIT_TXE(ModbusSlave_Uart);
			#if defined(MODBUS_MASTER_USE_CONTROL_PIN)	
				/* 485????????*/
				HAL_GPIO_WritePin(MODBUS_MASTER_GPIO_PORT,MODBUS_MASTER_GPIO_PIN,MODBUS_MASTER_GPIO_PIN_LOW);
			#endif
		}
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
	LL_USART_TransmitData8(ModbusSlave_Uart,ucByte);
	while(LL_USART_IsActiveFlag_TXE(ModbusSlave_Uart) == RESET);
			return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
	*(uint8_t *)pucByte = LL_USART_ReceiveData8( ModbusMaster_Uart );
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
//static 
void prvvSlaveUARTTxReadyISR( void )		//??????static????????????��????
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
//static 
void prvvSlaveUARTRxISR( void )				//??????static????????????��????
{
    pxMBFrameCBByteReceived(  );
}
