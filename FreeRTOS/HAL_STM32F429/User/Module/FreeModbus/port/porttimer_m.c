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
 * File: $Id: porttimer_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "./basetimer/bsp_timer_basetimer.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbport.h"

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
/* ----------------------- Variables ----------------------------------------*/
static USHORT usT35TimeOut50us;

void prvvMasterTIMERExpiredISR(void);
static void timer_timeout_ind(void* parameter);



/* ----------------------- Start implementation -----------------------------*/
/* ��ʱ��Ӳ����ʼ�� */
BOOL xMBMasterPortTimersInit(USHORT usTimeOut50us)
{
    ModbusMasterTimer_Init(usTimeOut50us);
    return TRUE;
}

/* ���ö�ʱ������ΪT35ʱ�� */
void vMBMasterPortTimersT35Enable()
{
    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_T35);
    LL_TIM_EnableARRPreload(ModbusMaster_Tim);
    LL_TIM_EnableCounter(ModbusMaster_Tim);
    LL_TIM_ClearFlag_UPDATE(ModbusMaster_Tim);
    LL_TIM_EnableIT_UPDATE(ModbusMaster_Tim);
}

/* ���ö�ʱ�����չ㲥֡��ת����ʱʱ�俪ʼ���� */
void vMBMasterPortTimersConvertDelayEnable()
{
    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_CONVERT_DELAY);
}

/* ���ö�ʱ��������Ӧ��ʱʱ�俪ʼ���� */
void vMBMasterPortTimersRespondTimeoutEnable()
{
    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_RESPOND_TIMEOUT);

    /* ����Ӧ���ö�ʱ������ */
}

/* ʧ�ܶ�ʱ�� */
void vMBMasterPortTimersDisable()
{
    // NVIC_DisableIRQ(TIM8_TRG_COM_TIM14_IRQn);
    LL_TIM_DisableCounter(ModbusMaster_Tim);
    LL_TIM_DisableARRPreload(ModbusMaster_Tim);
    LL_TIM_DisableIT_UPDATE(ModbusMaster_Tim);
    LL_TIM_ClearFlag_UPDATE(ModbusMaster_Tim);
    
}

/* ��ʱ���жϻص����������r��ɕr���� */
void prvvMasterTIMERExpiredISR(void)
{
    (void)pxMBMasterPortCBTimerExpired();
}

static void timer_timeout_ind(void* parameter)
{
    prvvMasterTIMERExpiredISR();
}


#endif
