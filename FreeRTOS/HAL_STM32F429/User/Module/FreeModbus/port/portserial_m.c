/*
 * FreeModbus Libary: RT-Thread Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
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
 * File: $Id: portserial_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "./modbus/bsp_uart_modbus.h"
#include "stm32f4xx.h"

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
/* ----------------------- Static variables ---------------------------------*/

/* ----------------------- Defines ------------------------------------------*/
/* serial transmit event */
#define EVENT_SERIAL_TRANS_START    (1<<0)

/* ----------------------- static functions ---------------------------------*/
void prvvMasterUARTTxReadyISR(void);
void prvvMasterUARTRxISR(void);

//static void serial_soft_trans_irq(void* parameter);

/* ----------------------- Start implementation -----------------------------*/
/* modbus???????????? */
BOOL xMBMasterPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits,eMBParity eParity)
{
	#if defined(MODBUS_MASTER_USE_CONTROL_PIN)	
		modbus_master_control_init();
	#endif
	
	ModbusMaster_Uart_Init(ulBaudRate);
	return TRUE;
}


void vMBMasterPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
	if(xRxEnable)
	{
		LL_USART_EnableIT_RXNE(ModbusMaster_Uart);
	}
	else
	{
		LL_USART_DisableIT_RXNE(ModbusMaster_Uart);

	}
	
	if(xTxEnable)
	{
		LL_USART_EnableIT_TXE(ModbusMaster_Uart);

	}
	else
	{
		LL_USART_DisableIT_TXE(ModbusMaster_Uart);
	}
}

void vMBMasterPortClose(void)
{
	/* ?????? */
    return;
}

/* modbus?????????? */
BOOL xMBMasterPortSerialPutByte(CHAR ucByte)
{
	LL_USART_TransmitData8(ModbusMaster_Uart,ucByte);
	while(LL_USART_IsActiveFlag_TXE(ModbusMaster_Uart) == RESET);
    return TRUE;
}

/* modbus½ÓÊÕÒ»‚€×Ö¹ */
BOOL xMBMasterPortSerialGetByte(CHAR * pucByte)
{
	*(uint8_t *)pucByte = LL_USART_ReceiveData8( ModbusMaster_Uart );
    return TRUE;
}

/* 
 * Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
void prvvMasterUARTTxReadyISR(void)
{
	/* ???????? */
    pxMBMasterFrameCBTransmitterEmpty();
}

/* 
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvMasterUARTRxISR(void)
{
		/* ???????? */
    pxMBMasterFrameCBByteReceived();
}

/**
 * Software simulation serial transmit IRQ handler.
 *
 * @param parameter parameter
 */
//static void serial_soft_trans_irq(void* parameter) {
//	extern char Transmit_Flag ;
//    while (1)
//    {
//			
//        /* waiting for serial transmit start */
//				if( Transmit_Flag == 1)
//        /* execute modbus callback */
//        prvvUARTTxReadyISR();
//    }
//}


#endif
