/* 
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2006-2018 Christian Walter <cwalter@embedded-solutions.at>
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
 * File: $Id: mbconfig.h,v 1.14 2006/12/07 22:10:34 wolti Exp $
 *       $Id: mbconfig.h,v 1.60 2013/08/13 21:19:55 Armink Add Master Functions $
 */

#ifndef _MB_CONFIG_H
#define _MB_CONFIG_H

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif
 
#include "stm32f429xx.h"
#include "stm32f4xx_hal_conf.h"

/* 主机ASCII,RTU,TCP模式使能 */
#define MB_MASTER_ASCII_ENABLED                 (  0 )
#define MB_MASTER_RTU_ENABLED                   (  1 )
#define MB_MASTER_TCP_ENABLED                   (  0 )


/*! 从机ASCII,RTU,TCP模式使能 */
#define MB_SLAVE_ASCII_ENABLED                  (  0 )
#define MB_SLAVE_RTU_ENABLED                    (  1 )
#define MB_SLAVE_TCP_ENABLED                    (  0 )

/*! \brief The character timeout value for Modbus ASCII.
 *
 * The character timeout value is not fixed for Modbus ASCII and is therefore
 * a configuration option. It should be set to the maximum expected delay
 * time of the network.
 */
#define MB_ASCII_TIMEOUT_SEC                    (  1 )

/*! 支持的最大功能数(功能码值) */
#define MB_FUNC_HANDLERS_MAX                    ( 16 )  

/*! 支持的最大从机地址 */
#define MB_FUNC_OTHER_REP_SLAVEID_BUF           ( 32 )

/*! \brief If the <em>Report Slave ID</em> function should be enabled. */
#define MB_FUNC_OTHER_REP_SLAVEID_ENABLED       (  1 )

/*! 读输入寄存器(0x04)使能 */
#define MB_FUNC_READ_INPUT_ENABLED              (  1 )

/*! 读保持寄存器(0x03)使能 */
#define MB_FUNC_READ_HOLDING_ENABLED            (  1 )

/*! 写保持寄存器(0x06)使能 */
#define MB_FUNC_WRITE_HOLDING_ENABLED           (  1 )

/*! 写多个保持寄存器(0x10)使能 */
#define MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED  (  1 )

/*! 读线圈(0x01)使能 */
#define MB_FUNC_READ_COILS_ENABLED              (  1 )

/*! 写线圈(0x05)使能 */
#define MB_FUNC_WRITE_COIL_ENABLED              (  1 )

/*! 写多个线圈(0x0F)使能 */
#define MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED    (  1 )

/*! \brief If the <em>Read Discrete Inputs</em> function should be enabled. */
#define MB_FUNC_READ_DISCRETE_INPUTS_ENABLED    (  1 )

/*! \brief If the <em>Read/Write Multiple Registers</em> function should be enabled. */
#define MB_FUNC_READWRITE_HOLDING_ENABLED       (  1 )

#ifdef __cplusplus
    PR_END_EXTERN_C
#endif

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0

/*! \brief If master send a broadcast frame,the master will wait time of convert to delay,
 * then master can send other frame */
#define MB_MASTER_DELAY_MS_CONVERT              (200 )

/*! \brief If master send a frame which is not broadcast,the master will wait sometime for slave.
 * And if slave is not respond in this time,the master will process this timeout error.
 * Then master can send other frame */
#define MB_MASTER_TIMEOUT_MS_RESPOND            (100 )
#define MB_MASTER_TOTAL_SLAVE_NUM               ( 16 )  /*! 最大节点数量 */
#endif

#endif
