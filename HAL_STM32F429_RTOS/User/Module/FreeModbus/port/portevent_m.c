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
 * File: $Id: portevent_m.c v 1.60 2013/08/13 15:07:05 Armink add Master Functions$
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbport.h"
#include "port.h"

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
extern BOOL xMBMasterRTUTimerExpired(void);


static eMBMasterEventType eMasterQueuedEvent;   /* 主机状态标志位 */
static BOOL    xMasterEventInQueue;             /* 主机事件变更标志位 */
/* ----------------------- Start implementation -----------------------------*/
BOOL xMBMasterPortEventInit( void )
{
    xMasterEventInQueue = TRUE;
	eMasterQueuedEvent = EV_MASTER_READY;
    return TRUE;
}

/* 主機modbus狀態配置 */
BOOL xMBMasterPortEventPost( eMBMasterEventType eEvent )
{
	eMasterQueuedEvent = eEvent;

    switch (eMasterQueuedEvent)
    {
        case EV_MASTER_READY:   
        case EV_MASTER_FRAME_RECEIVED:
        case EV_MASTER_EXECUTE:
        case EV_MASTER_FRAME_SENT:
        case EV_MASTER_ERROR_PROCESS:
            xMasterEventInQueue = TRUE;break;

    }
    return TRUE;
}

/* 主機modbus狀態獲取 */
BOOL xMBMasterPortEventGet( eMBMasterEventType * eEvent )
{
    if(xMasterEventInQueue != TRUE)
    {
        return FALSE;
    }

    xMasterEventInQueue = FALSE;

    switch (eMasterQueuedEvent)
    {
			case EV_MASTER_READY:   
					*eEvent = EV_MASTER_READY;			 		
					break;
			case EV_MASTER_FRAME_RECEIVED:
					*eEvent = EV_MASTER_FRAME_RECEIVED;
					break;
			case EV_MASTER_EXECUTE:
					*eEvent = EV_MASTER_EXECUTE;
					break;
			case EV_MASTER_FRAME_SENT:
					*eEvent = EV_MASTER_FRAME_SENT;
					break;
			case EV_MASTER_ERROR_PROCESS:
					*eEvent = EV_MASTER_ERROR_PROCESS;
					break;
            default:return FALSE;
    }
    return TRUE;
}
/**
 * This function is initialize the OS resource for modbus master.
 * Note:The resource is define by OS.If you not use OS this function can be empty.
 *
 */
void vMBMasterOsResInit( void )
{
    return ;
}

/**
 * This function is take Mobus Master running resource.
 * Note:The resource is define by Operating System.If you not use OS this function can be just return TRUE.
 *
 * @param lTimeOut the waiting time.
 *
 * @return resource taked result
 */
BOOL xMBMasterRunResTake( LONG lTimeOut )
{
    /*If waiting time is -1 .It will wait forever */
    return TRUE ;
}

/**
 * This function is release Mobus Master running resource.
 * Note:The resource is define by Operating System.If you not use OS this function can be empty.
 *
 */
void vMBMasterRunResRelease( void )
{
    /* release resource */
    return;
}

/**
 * This is modbus master respond timeout error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 * @param ucDestAddress destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMBMasterErrorCBRespondTimeout(UCHAR ucDestAddress, const UCHAR* pucPDUData,
        USHORT ucPDULength) {
    /**
     * @note This code is use OS's event mechanism for modbus master protocol stack.
     * If you don't use OS, you can change it.
     */
    xMasterEventInQueue = TRUE;
    eMasterQueuedEvent = EV_MASTER_ERROR_RESPOND_TIMEOUT;
    return ;

    /* You can add your code under here. */

}

/**
 * This is modbus master receive data error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 * @param ucDestAddress destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMBMasterErrorCBReceiveData(UCHAR ucDestAddress, const UCHAR* pucPDUData,
        USHORT ucPDULength) {
    /**
     * @note This code is use OS's event mechanism for modbus master protocol stack.
     * If you don't use OS, you can change it.
     */
    xMasterEventInQueue = TRUE;
    eMasterQueuedEvent = EV_MASTER_ERROR_RECEIVE_DATA;
    return;

    /* You can add your code under here. */

}

/**
 * This is modbus master execute function error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 * @param ucDestAddress destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMBMasterErrorCBExecuteFunction(UCHAR ucDestAddress, const UCHAR* pucPDUData,
        USHORT ucPDULength) {
    /**
     * @note This code is use OS's event mechanism for modbus master protocol stack.
     * If you don't use OS, you can change it.
     */
    xMasterEventInQueue = TRUE;
    eMasterQueuedEvent = EV_MASTER_ERROR_EXECUTE_FUNCTION;
    return;

    /* You can add your code under here. */

}

/**
 * This is modbus master request process success callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 */
void vMBMasterCBRequestScuuess( void ) 
{
    /**
     * @note This code is use OS's event mechanism for modbus master protocol stack.
     * If you don't use OS, you can change it.
     */
    xMasterEventInQueue = TRUE;
    eMasterQueuedEvent = EV_MASTER_PROCESS_SUCESS;
    return;

    /* You can add your code under here. */

}

/* 等待modbus主请求完成并返回结果 */
eMBMasterReqErrCode eMBMasterWaitRequestFinish( void ) 
{
    eMBMasterReqErrCode    eErrStatus = MB_MRE_NO_ERR;



    switch (eMasterQueuedEvent)
    {
        case EV_MASTER_PROCESS_SUCESS:  /* 请求处理成功 */
            break;
            
        case EV_MASTER_ERROR_RESPOND_TIMEOUT:  /* 请求处理超时 */
            eErrStatus = MB_MRE_TIMEDOUT;
            break;
        
        case EV_MASTER_ERROR_RECEIVE_DATA:  /* 接收数据错误 */ 
            eErrStatus = MB_MRE_REV_DATA;
            break;
        
        case EV_MASTER_ERROR_EXECUTE_FUNCTION:  /* 执行函数错误 */
            eErrStatus = MB_MRE_EXE_FUN;
            break;
    }
    return eErrStatus;
}

#endif
