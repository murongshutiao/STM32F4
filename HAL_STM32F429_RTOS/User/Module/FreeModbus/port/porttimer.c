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

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "./basetimer/bsp_timer_basetimer.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortTimersInit( USHORT usTim1Timerout50us )		//��ʱ����ʼ��ֱ�ӷ���TRUE���Ѿ���mian������ʼ����
{
    ModbusSlaveTimer_Init(usTim1Timerout50us);
    return TRUE;
}


inline void vMBPortTimersEnable(  )		//ʹ�ܶ�ʱ���жϣ����õ��Ƕ�ʱ��4������Ϊ&htim4
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
    LL_TIM_EnableARRPreload(ModbusSlave_Tim);
    LL_TIM_EnableCounter(ModbusSlave_Tim);
    LL_TIM_ClearFlag_UPDATE(ModbusSlave_Tim);
    LL_TIM_EnableIT_UPDATE(ModbusSlave_Tim);
}

inline void vMBPortTimersDisable(  )	//ȡ����ʱ���ж�
{
    /* Disable any pending timers. */
    LL_TIM_DisableCounter(ModbusSlave_Tim);
    LL_TIM_DisableARRPreload(ModbusSlave_Tim);
    LL_TIM_DisableIT_UPDATE(ModbusSlave_Tim);
    LL_TIM_ClearFlag_UPDATE(ModbusSlave_Tim);
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
//static 
void prvvSlaveTIMERExpiredISR( void )	//modbus��ʱ����������Ҫ���ж���ʹ��
{
    ( void )pxMBPortCBTimerExpired(  );
}

