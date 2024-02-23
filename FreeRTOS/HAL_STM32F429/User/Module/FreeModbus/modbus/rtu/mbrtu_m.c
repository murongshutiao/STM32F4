/* 
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2013 China Beijing Armink <armink.ztl@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: mbrtu_m.c,v 1.60 2013/08/17 11:42:56 Armink Add Master Functions $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbrtu.h"
#include "mbframe.h"

#include "mbcrc.h"
#include "mbport.h"

#if MB_MASTER_RTU_ENABLED > 0
/* ----------------------- Defines ------------------------------------------*/
#define MB_SER_PDU_SIZE_MIN     4       /*!< Minimum size of a Modbus RTU frame. */
#define MB_SER_PDU_SIZE_MAX     256     /*!< Maximum size of a Modbus RTU frame. */
#define MB_SER_PDU_SIZE_CRC     2       /*!< Size of CRC field in PDU. */
#define MB_SER_PDU_ADDR_OFF     0       /*!< ������ַƫ�� */
#define MB_SER_PDU_PDU_OFF      1       /*!< Э��PDU��ƫ�� */

/* ----------------------- 485״̬����  ---------------------------------*/
typedef enum
{
    STATE_M_RX_INIT,              /*!< ��ʼ��. */
    STATE_M_RX_IDLE,              /*!< ���Ք�����. */
    STATE_M_RX_RCV,               /*!< Frame is beeing received. */
    STATE_M_RX_ERROR,              /*!< If the frame is invalid. */
} eMBMasterRcvState;        /* ��������״̬���� */

/* ����״̬ */
typedef enum
{
    STATE_M_TX_IDLE,              /*!< ������Ϊ����״̬. */
    STATE_M_TX_XMIT,              /*!< ������Ϊ����״̬ */
    STATE_M_TX_XFWR,              /*!< ������������ɣ��ȴ�����. */
} eMBMasterSndState;

/* ----------------------- Static variables ---------------------------------*/
static volatile eMBMasterSndState eSndState;    /* ��������ǰ����״̬ */
static volatile eMBMasterRcvState eRcvState;    /* ��������ǰ����״̬ */

static volatile UCHAR  ucMasterRTUSndBuf[MB_PDU_SIZE_MAX];  /* �������ͻ����� */
static volatile UCHAR  ucMasterRTURcvBuf[MB_SER_PDU_SIZE_MAX];/* �������ջ�����*/
static volatile USHORT usMasterSendPDULength;

static volatile UCHAR *pucMasterSndBufferCur;   /* modbus�������ݵ�ָ�� */
static volatile USHORT usMasterSndBufferCount;  /* modbus�������ݵĳ��� */

static volatile USHORT usMasterRcvBufferPos;
static volatile BOOL   xFrameIsBroadcast = FALSE;   /* �㲥֡��־ */

static volatile eMBMasterTimerMode eMasterCurTimerMode;

/* ----------------------- Start implementation -----------------------------*/
eMBErrorCode eMBMasterRTUInit(UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    ULONG           usTimerT35_50us;

    ENTER_CRITICAL_SECTION(  );

    /* ��ʼ��Ӳ�������ݲ��������õ���bitλʱ�� */
    /* ��żУ�����Ϊ��У�飬����ֹͣλһ�� */
    if(xMBMasterPortSerialInit(ucPort, ulBaudRate, 8, eParity ) != TRUE )
    {
        eStatus = MB_EPORTERR;
    }
    else
    {
        /* If baudrate > 19200 then we should use the fixed timer values
         * t35 = 1750us. Otherwise t35 must be 3.5 times the character time.
         */
        if( ulBaudRate > 19200 )
        {
            usTimerT35_50us = 35;       /* 1800us. */
        }
        else
        {
      
             /*
             * ChTimeValue = 3.5 * Ticks_per_1s / Baudrate * 11bits
                           = 3.5 * 2 * 11 * (1S / 50uS) / (Baudrate * 2)
                           = (3.5 * 2 * 11 * 20000) / (Baudrate * 2)
             */
            usTimerT35_50us = ( 7UL * 220000UL ) / ( 2UL * ulBaudRate );

        }

        /* Modbus������ʱ����ʼ�� */
        if( xMBMasterPortTimersInit((USHORT)usTimerT35_50us) != TRUE )
        {
            eStatus = MB_EPORTERR;
        }
    }
    //EXIT_CRITICAL_SECTION(  );

    return eStatus;
}

 /**
  * @brief  RTU��ʼ��
  * @param  None
  * @retval None
  * @note   
 */
void eMBMasterRTUStart( void )
{
    ENTER_CRITICAL_SECTION(  );
    eRcvState = STATE_M_RX_INIT;        /* ���ý��������ʼ����B */
    vMBMasterPortSerialEnable( TRUE, FALSE );   /* ���ý���ʹ�� */
    vMBMasterPortTimersT35Enable( );    /* ����ΪT35ģʽ */

    EXIT_CRITICAL_SECTION(  );
}

void eMBMasterRTUStop( void )
{
    ENTER_CRITICAL_SECTION(  );
    vMBMasterPortSerialEnable( FALSE, FALSE );
    vMBMasterPortTimersDisable(  );
    EXIT_CRITICAL_SECTION(  );
}

eMBErrorCode eMBMasterRTUReceive( UCHAR * pucRcvAddress, UCHAR ** pucFrame, USHORT * pusLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    ENTER_CRITICAL_SECTION(  );
    assert_param( usMasterRcvBufferPos < MB_SER_PDU_SIZE_MAX );

    /* ����Э�鳤���жϺ�CRCУ�� */
    if( ( usMasterRcvBufferPos >= MB_SER_PDU_SIZE_MIN )
        && ( usMBCRC16( ( UCHAR * ) ucMasterRTURcvBuf, usMasterRcvBufferPos ) == 0 ) )
    {
        /* ��ȡ�ӻ���ַ */
        *pucRcvAddress = ucMasterRTURcvBuf[MB_SER_PDU_ADDR_OFF];

        /* ��ȡ�������ݵĳ��� */
        *pusLength = ( USHORT )( usMasterRcvBufferPos - MB_SER_PDU_PDU_OFF - MB_SER_PDU_SIZE_CRC );

        /* ����Э���PDU���׵�ַ */
        *pucFrame = (UCHAR *)&ucMasterRTURcvBuf[MB_SER_PDU_PDU_OFF];
    }
    else
    {
        eStatus = MB_EIO;
    }

    //EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

/* �������ͺ��� */
eMBErrorCode eMBMasterRTUSend( UCHAR ucSlaveAddress, const UCHAR * pucFrame, USHORT usLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          usCRC16;

    /* ���ڵ��ַ */
    if ( ucSlaveAddress > MB_MASTER_TOTAL_SLAVE_NUM ) 
    {
        return MB_EINVAL;
    }

    ENTER_CRITICAL_SECTION(  );

    if(eRcvState == STATE_M_RX_IDLE)
    {
        /* First byte before the Modbus-PDU is the slave address. */
        pucMasterSndBufferCur = ( UCHAR * ) pucFrame - 1;
        usMasterSndBufferCount = 1;

        /* ����ӻ���ַ */
        pucMasterSndBufferCur[MB_SER_PDU_ADDR_OFF] = ucSlaveAddress;
        usMasterSndBufferCount += usLength;

        /* ����CRC */
        usCRC16 = usMBCRC16( ( UCHAR * ) pucMasterSndBufferCur, usMasterSndBufferCount );
        ucMasterRTUSndBuf[usMasterSndBufferCount++] = ( UCHAR )( usCRC16 & 0xFF );
        ucMasterRTUSndBuf[usMasterSndBufferCount++] = ( UCHAR )( usCRC16 >> 8 );

        /* Activate the transmitter. */
        eSndState = STATE_M_TX_XMIT;    /* ����Ϊ����״̬ */
        vMBMasterPortSerialEnable(FALSE,TRUE);   /* ���÷���ʹ��,������TXE�ж� */
    }
    else
    {
        eStatus = MB_EIO;
    }
    EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

/* �����Д����{�� */
BOOL xMBMasterRTUReceiveFSM( void )
{
    BOOL            xTaskNeedSwitch = FALSE;
    UCHAR           ucByte;

    assert_param((eSndState == STATE_M_TX_IDLE) || (eSndState == STATE_M_TX_XFWR));

    /* ���ڻ�ȡһ���ֽ� */
    (void)xMBMasterPortSerialGetByte((CHAR *)&ucByte );

    switch (eRcvState)
    {
        case STATE_M_RX_INIT:   
        vMBMasterPortTimersT35Enable(); /* ���յ�һ���ַ�,���¼�ʱ */
        break;

        case STATE_M_RX_ERROR:
        vMBMasterPortTimersT35Enable();
        break;

        case STATE_M_RX_IDLE:
    	/* In time of respond timeout,the receiver receive a frame.
    	 * Disable timer of respond timeout and change the transmiter state to idle.
    	 */
    	vMBMasterPortTimersDisable( );
    	eSndState = STATE_M_TX_IDLE;

        usMasterRcvBufferPos = 0;
        ucMasterRTURcvBuf[usMasterRcvBufferPos++] = ucByte;
        eRcvState = STATE_M_RX_RCV; /* ����Ϊ�������״̬ */

        /* ��ʱ�����¼�ʱ. */
        vMBMasterPortTimersT35Enable( );
        break;

        /* We are currently receiving a frame. Reset the timer after
         * every character received. If more than the maximum possible
         * number of bytes in a modbus frame is received the frame is
         * ignored.
         */
    case STATE_M_RX_RCV:
        if( usMasterRcvBufferPos < MB_SER_PDU_SIZE_MAX )
        {
            ucMasterRTURcvBuf[usMasterRcvBufferPos++] = ucByte;
        }
        else
        {
            eRcvState = STATE_M_RX_ERROR;
        }
        vMBMasterPortTimersT35Enable(); /* ��δ�����꣬���¼�ʱ */
        break;
    }
    return xTaskNeedSwitch;
}

/*
  @note:�˺����ڷ����ж��е���
*/
BOOL xMBMasterRTUTransmitFSM( void )
{
    BOOL            xNeedPoll = FALSE;

    assert_param(eRcvState == STATE_M_RX_IDLE );

    switch (eSndState)
    {
        case STATE_M_TX_IDLE:   /* ����״̬ */
            /* ʹ�ܽ��գ�ֹͣ���� */
            vMBMasterPortSerialEnable(TRUE, FALSE);
            break;

        case STATE_M_TX_XMIT:   /* ����״̬ */
            if( usMasterSndBufferCount != 0 )   /* �жϴ������ֽ��� */
            {
                /* ����һ���ֽ� */
                xMBMasterPortSerialPutByte((CHAR)*pucMasterSndBufferCur );
                pucMasterSndBufferCur++;  /* next byte in sendbuffer. */
                usMasterSndBufferCount--;
            }
            else
            {
                /* ������ɣ�ִ���±ߴ��� */
                xFrameIsBroadcast = ( ucMasterRTUSndBuf[MB_SER_PDU_ADDR_OFF] == MB_ADDRESS_BROADCAST ) ? TRUE : FALSE;

                /* �л�Ϊ����ʹ�ܣ��رշ��� */
                vMBMasterPortSerialEnable(TRUE,FALSE);
                eSndState = STATE_M_TX_XFWR;/* ����Ϊ������ɣ��ȴ�����״̬ */

                /* If the frame is broadcast ,master will enable timer of convert delay,
                * else master will enable timer of respond timeout. */
                if(xFrameIsBroadcast == TRUE )
                {
                    vMBMasterPortTimersConvertDelayEnable( );
                }
                else
                {
                    vMBMasterPortTimersRespondTimeoutEnable( );
                }
            }
            break;

            default:
        break;
    }

    return xNeedPoll;
}

/* ���r����B�C���к��� */
BOOL xMBMasterRTUTimerExpired(void)
{
	BOOL xNeedPoll = FALSE;

    /* ��������B̎�� */
	switch (eRcvState)
	{
        /* ��ʼ����������״̬����Ϊ���� */
        case STATE_M_RX_INIT:   /* ���r��ǰһ��B */
		xNeedPoll = xMBMasterPortEventPost(EV_MASTER_READY);
		break;

        case STATE_M_RX_RCV:
        /* ����Ϊ�������״̬ */
		xNeedPoll = xMBMasterPortEventPost(EV_MASTER_FRAME_RECEIVED);
		break;

		/* An error occured while receiving the frame. */
        case STATE_M_RX_ERROR:
		vMBMasterSetErrorType(EV_ERROR_RECEIVE_DATA);
		xNeedPoll = xMBMasterPortEventPost( EV_MASTER_ERROR_PROCESS );
		break;

		/* Function called in an illegal state. */
        default:
		assert_param(
				(eRcvState ==STATE_M_RX_INIT) || (eRcvState == STATE_M_RX_RCV ) ||
				(eRcvState ==STATE_M_RX_ERROR) || (eRcvState == STATE_M_RX_IDLE ));
		break;
	}
    
    /* ֻҪ�M�����@�����r���������J����Ք������ */
	eRcvState = STATE_M_RX_IDLE;

	switch (eSndState)
	{
		/* A frame was send finish and convert delay or respond timeout expired.
		 * If the frame is broadcast,The master will idle,and if the frame is not
		 * broadcast.Notify the listener process error.*/
        case STATE_M_TX_XFWR:
		if ( xFrameIsBroadcast == FALSE ) 
        {
			vMBMasterSetErrorType(EV_ERROR_RESPOND_TIMEOUT);    /* ����Ϊ��Ӧ��ʱ */
			xNeedPoll = xMBMasterPortEventPost(EV_MASTER_ERROR_PROCESS);
		}
		break;
		/* Function called in an illegal state. */
        default:
		assert_param(
				( eSndState == STATE_M_TX_XFWR ) || ( eSndState == STATE_M_TX_IDLE ));
		break;
	}
	eSndState = STATE_M_TX_IDLE;

    /* ���Ք�����ɣ��P�]���r�� */
	vMBMasterPortTimersDisable( );

	/* If timer mode is convert delay, the master event then turns EV_MASTER_EXECUTE status. */
	if (eMasterCurTimerMode == MB_TMODE_CONVERT_DELAY) 
    {
		xNeedPoll = xMBMasterPortEventPost( EV_MASTER_EXECUTE );
	}

	return xNeedPoll;
}

/* Get Modbus Master send RTU's buffer address pointer.*/
void vMBMasterGetRTUSndBuf( UCHAR ** pucFrame )
{
	*pucFrame = (UCHAR *)ucMasterRTUSndBuf;
}

/* Get Modbus Master send PDU's buffer address pointer.*/
void vMBMasterGetPDUSndBuf( UCHAR ** pucFrame )
{
	*pucFrame = (UCHAR *)&ucMasterRTUSndBuf[MB_SER_PDU_PDU_OFF];
}

/* Set Modbus Master send PDU's buffer length.*/
void vMBMasterSetPDUSndLength( USHORT SendPDULength )
{
	usMasterSendPDULength = SendPDULength;
}

/* Get Modbus Master send PDU's buffer length.*/
USHORT usMBMasterGetPDUSndLength( void )
{
	return usMasterSendPDULength;
}

/* Set Modbus Master �ĳ�ʱģʽ.*/
void vMBMasterSetCurTimerMode( eMBMasterTimerMode eMBTimerMode )
{
	eMasterCurTimerMode = eMBTimerMode;
}

/* ���ص�ǰ�㲥״̬ */
BOOL xMBMasterRequestIsBroadcast( void )
{
	return xFrameIsBroadcast;
}
#endif

