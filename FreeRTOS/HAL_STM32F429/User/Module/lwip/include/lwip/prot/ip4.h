/**
 * @file
 * IPv4 protocol definitions
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifndef LWIP_HDR_PROT_IP4_H
#define LWIP_HDR_PROT_IP4_H

#include "lwip/arch.h"
#include "lwip/ip4_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

/** This is the packed version of ip4_addr_t,
    used in network headers that are itself packed */
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
struct ip4_addr_packed {
  PACK_STRUCT_FIELD(u32_t addr);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif

typedef struct ip4_addr_packed ip4_addr_p_t;

/* Size of the IPv4 header. Same as 'sizeof(struct ip_hdr)'. */
#define IP_HLEN 20
/* Maximum size of the IPv4 header with options. */
#define IP_HLEN_MAX 60

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/bpstruct.h"
#endif
PACK_STRUCT_BEGIN
/* The IPv4 header */
struct ip_hdr /* IPV4头部 */
{
    /* version / header length */
    PACK_STRUCT_FLD_8(u8_t _v_hl);  /* 版本和首部长度 */
    /* type of service */
    PACK_STRUCT_FLD_8(u8_t _tos);   /* 服务类型 */
    /* total length */
    PACK_STRUCT_FIELD(u16_t _len);  /* 数据报长度 */
    /* identification */
    PACK_STRUCT_FIELD(u16_t _id);   /* 每一条ip报文的id,对于ip分片的每一条是一样的。 */
    /* fragment offset field */
    PACK_STRUCT_FIELD(u16_t _offset);   /* 分片标志与分片偏移 */
/* 关于分片标志的相关宏定义 */
#define IP_RF 0x8000U        /* 保留的分片标志位 */
#define IP_DF 0x4000U        /* 不分片标志位 */
#define IP_MF 0x2000U        /* 更多分片标志位 */
#define IP_OFFMASK 0x1fffU   /* mask for fragmenting bits */
    /* time to live */
    PACK_STRUCT_FLD_8(u8_t _ttl);   /* 生存时间 */
    /* protocol*/
    PACK_STRUCT_FLD_8(u8_t _proto); /* 上层协议 */
    /* checksum */
    PACK_STRUCT_FIELD(u16_t _chksum);   /* 校验和 */
    /* source and destination IP addresses */
    PACK_STRUCT_FLD_S(ip4_addr_p_t src);
    PACK_STRUCT_FLD_S(ip4_addr_p_t dest);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END
#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch/epstruct.h"
#endif

/* Macros to get struct ip_hdr fields: */
#define IPH_V(hdr)  ((hdr)->_v_hl >> 4)     /* 获取协议版本 */
#define IPH_HL(hdr) ((hdr)->_v_hl & 0x0f)   /* 获取头部长度 */
#define IPH_HL_BYTES(hdr) ((u8_t)(IPH_HL(hdr) * 4)) /* 获取IP首部长度字节 */
#define IPH_TOS(hdr) ((hdr)->_tos)                  /* 获取服务类型 */
#define IPH_LEN(hdr) ((hdr)->_len)                  /* IP报文总长度 */
#define IPH_ID(hdr) ((hdr)->_id)                    
#define IPH_OFFSET(hdr) ((hdr)->_offset)
#define IPH_OFFSET_BYTES(hdr) ((u16_t)((lwip_ntohs(IPH_OFFSET(hdr)) & IP_OFFMASK) * 8U))
#define IPH_TTL(hdr) ((hdr)->_ttl)
#define IPH_PROTO(hdr) ((hdr)->_proto)
#define IPH_CHKSUM(hdr) ((hdr)->_chksum)

/* Macros to set struct ip_hdr fields: */
#define IPH_VHL_SET(hdr, v, hl) (hdr)->_v_hl = (u8_t)((((v) << 4) | (hl)))
#define IPH_TOS_SET(hdr, tos) (hdr)->_tos = (tos)
#define IPH_LEN_SET(hdr, len) (hdr)->_len = (len)
#define IPH_ID_SET(hdr, id) (hdr)->_id = (id)
#define IPH_OFFSET_SET(hdr, off) (hdr)->_offset = (off)
#define IPH_TTL_SET(hdr, ttl) (hdr)->_ttl = (u8_t)(ttl)
#define IPH_PROTO_SET(hdr, proto) (hdr)->_proto = (u8_t)(proto)
#define IPH_CHKSUM_SET(hdr, chksum) (hdr)->_chksum = (chksum)


#ifdef __cplusplus
}
#endif

#endif /* LWIP_HDR_PROT_IP4_H */
