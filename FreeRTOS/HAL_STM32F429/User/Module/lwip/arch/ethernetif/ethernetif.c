/**
  ******************************************************************************
  * File Name          : ethernetif.c
  * Description        : This file provides code for the configuration
  *                      of the ethernetif.c MiddleWare.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/timeouts.h"
#include "netif/ethernet.h"
#include "netif/etharp.h"
#include "lwip/ethip6.h"
#include "ethernetif.h"
#include <string.h>

/* Within 'USER CODE' section, code will be kept by default at each generation */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* Private define ------------------------------------------------------------*/

/* Network interface name */
#define IFNAME0 'R'
#define IFNAME1 'Y'


struct ethernetif 
{
	struct eth_addr *ethaddr;
/* Add whatever per-interface state that is needed here. */
};



/* Global Ethernet handle */
extern ETH_HandleTypeDef heth;

/* 以太网接收中断信号量 */
xSemaphoreHandle s_xSemaphore = NULL;


sys_sem_t tx_sem = NULL;
sys_mbox_t eth_tx_mb = NULL;

//static struct netif *s_pxNetIf = NULL;

static void arp_timer(void *arg);




/*******************************************************************************
                       LL Driver Interface ( LwIP stack --> ETH) 
*******************************************************************************/
/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 * @note  网卡初始化函数
 *        1.硬件初始化
 *        2.操作系统线程创建，数据结构创建
 *        3.网络参数配置
 */
static void low_level_init(struct netif *netif)
{ 
    HAL_StatusTypeDef hal_eth_init_status;
    
    //初始化bsp—eth,注意多个网卡时的处理。
    hal_eth_init_status = Bsp_Eth_Init();

    if (hal_eth_init_status == HAL_OK)
    {
        /* 设置网卡与ETH外设连接完成标志位 */  
        netif->flags |= NETIF_FLAG_LINK_UP;
    }

#if LWIP_ARP || LWIP_ETHERNET 

    /* set MAC hardware address length */
    netif->hwaddr_len = ETH_HWADDR_LEN;
    
    /* 配置MAC地址，此例程还没正确配置MAC地址 */
    netif->hwaddr[0] =  heth.Init.MACAddr[0];
    netif->hwaddr[1] =  heth.Init.MACAddr[1];
    netif->hwaddr[2] =  heth.Init.MACAddr[2];
    netif->hwaddr[3] =  heth.Init.MACAddr[3];
    netif->hwaddr[4] =  heth.Init.MACAddr[4];
    netif->hwaddr[5] =  heth.Init.MACAddr[5];
    
    /* maximum transfer unit */
    netif->mtu = NETIF_MTU;
    
    /* Accept broadcast address and ARP traffic */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    #if LWIP_ARP
        netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
    #else 
        netif->flags |= NETIF_FLAG_BROADCAST;
    #endif /* LWIP_ARP */

    /* 创建以太网数据接收信号量 */ 
    s_xSemaphore = xSemaphoreCreateCounting(40,0);
    
    if(sys_sem_new(&tx_sem , 0) == ERR_OK)
    {
        printf("sys_sem_new ok\n");
    }
    
    if(sys_mbox_new(&eth_tx_mb , 50) == ERR_OK)
    {
        printf("sys_mbox_new ok\n");
    }

    /* 创建以太网接收的线程 */
	sys_thread_new("ETHIN",
                  ethernetif_input,         /* 任务入口函数 */
                  netif,        	        /* 任务入口函数参数 */
                  NETIF_IN_TASK_STACK_SIZE, /* 任务栈大小 */
                  NETIF_IN_TASK_PRIORITY);  /* 任务的优先级 */
                  
#endif /* LWIP_ARP || LWIP_ETHERNET */
  
    HAL_NVIC_SetPriority(ETH_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(ETH_IRQn);
    __HAL_RCC_ETH_CLK_ENABLE();
    HAL_ETH_Start(&heth);
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 *       以太网发送函数，将lwip 数据结构体pbuf数据通过以太网函数HAL_ETH_TransmitFrame()发送出去
 *      此时pbuf的数据已经有了
 */

static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
	static sys_sem_t ousem = NULL;

	if(ousem == NULL)
    {
        sys_sem_new(&ousem,0);
        sys_sem_signal(&ousem);
    }

    err_t errval;
    struct pbuf *q;                 /* 定义一个lwip pbuf数据结构体指针 */

    uint8_t *buffer = (uint8_t *)(heth.TxDesc->Buffer1Addr);
    __IO ETH_DMADescTypeDef *DmaTxDesc; /* 以太网数据发送的数据结构 */
    uint32_t framelength = 0;
    uint32_t bufferoffset = 0;
    uint32_t byteslefttocopy = 0;
    uint32_t payloadoffset = 0;
    DmaTxDesc = heth.TxDesc;        /* 以太网发送指针赋值 */   
    bufferoffset = 0;

    /* 以太网发送缓冲区是否可用？ */
    if((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET)
    {
        errval = ERR_USE;
        goto error;
    }
    sys_sem_wait(&ousem);
  
    /* copy frame from pbufs to driver buffers */
    for(q = p; q != NULL; q = q->next)
    {
        /* 获取pbuf数据长度 */
        byteslefttocopy = q->len;
        payloadoffset = 0;
        
        /* 检查要拷贝的数据长度是否大于以太网控制器的最大发送字节，是的话要分批次拷贝 */
        while( (byteslefttocopy + bufferoffset) > ETH_TX_BUF_SIZE )
        {
            /* 将payload拷贝到缓冲区 */
            memcpy( (uint8_t*)((uint8_t*)buffer + bufferoffset), (uint8_t*)((uint8_t*)q->payload + payloadoffset), (ETH_TX_BUF_SIZE - bufferoffset) );
        
            /* Point to next descriptor */
            DmaTxDesc = (ETH_DMADescTypeDef *)(DmaTxDesc->Buffer2NextDescAddr);
        
            /* 查看指向的缓冲区是否可用 */
            if((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET)
            {
                errval = ERR_USE;
                goto error;
            }
      
            buffer = (uint8_t *)(DmaTxDesc->Buffer1Addr);
        
            byteslefttocopy = byteslefttocopy - (ETH_TX_BUF_SIZE - bufferoffset);
            payloadoffset = payloadoffset + (ETH_TX_BUF_SIZE - bufferoffset);
            framelength = framelength + (ETH_TX_BUF_SIZE - bufferoffset);
            bufferoffset = 0;
        }
    
        /* Copy the remaining bytes */
        memcpy( (uint8_t*)((uint8_t*)buffer + bufferoffset), (uint8_t*)((uint8_t*)q->payload + payloadoffset), byteslefttocopy );
        bufferoffset = bufferoffset + byteslefttocopy;
        framelength = framelength + byteslefttocopy;
    }
  
    /* 以太网DMA控制器启动发送 */ 
    HAL_ETH_TransmitFrame(&heth, framelength);
    
    errval = ERR_OK;

error:
  
    /* When Transmit Underflow flag is set, clear it and issue a Transmit Poll Demand to resume transmission */
    if ((heth.Instance->DMASR & ETH_DMASR_TUS) != (uint32_t)RESET)
    {
        /* Clear TUS ETHERNET DMA flag */
        heth.Instance->DMASR = ETH_DMASR_TUS;

        /* Resume DMA transmission*/
        heth.Instance->DMATPDR = 0;
    }
    
    sys_sem_signal(&ousem);
    
    return errval;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 * @note    以太网接收函数
   */
static struct pbuf *low_level_input(struct netif *netif)
{
    struct pbuf *p = NULL;
    struct pbuf *q = NULL;
    uint16_t len = 0;
    uint8_t *buffer;
    __IO ETH_DMADescTypeDef *dmarxdesc;
    uint32_t bufferoffset = 0;
    uint32_t payloadoffset = 0;
    uint32_t byteslefttocopy = 0;
    uint32_t i=0;
  
  
    /* 以太网控制器是否接收到数据帧？ */
    if (HAL_ETH_GetReceivedFrame(&heth) != HAL_OK)
    {
        return NULL;
    }

    /* Obtain the size of the packet and put it into the "len" variable. */
    len = heth.RxFrameInfos.length;                 /* 获取接收数据的长度 */
    buffer = (uint8_t *)heth.RxFrameInfos.buffer;   /* 获取接收的数据包指针 */
  
    
    if (len > 0)
    {
        /* 内存池中分配pbuf来存放以太网控制器接收到的以太网数据 */
        p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
    }
    
    if (p != NULL)
    {
        dmarxdesc = heth.RxFrameInfos.FSRxDesc; /* 赋值以太网接收数据的指针 */
        bufferoffset = 0;

        for(q = p; q != NULL; q = q->next)
        {
            byteslefttocopy = q->len;
            payloadoffset = 0;
            
            /* Check if the length of bytes to copy in current pbuf is bigger than Rx buffer size */
            while( (byteslefttocopy + bufferoffset) > ETH_RX_BUF_SIZE )
            {
                /* Copy data to pbuf */
                memcpy( (uint8_t*)((uint8_t*)q->payload + payloadoffset), (uint8_t*)((uint8_t*)buffer + bufferoffset), (ETH_RX_BUF_SIZE - bufferoffset));
                
                /* Point to next descriptor */
                dmarxdesc = (ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
                buffer = (uint8_t *)(dmarxdesc->Buffer1Addr);
                
                byteslefttocopy = byteslefttocopy - (ETH_RX_BUF_SIZE - bufferoffset);
                payloadoffset = payloadoffset + (ETH_RX_BUF_SIZE - bufferoffset);
                bufferoffset = 0;
            }

            /* Copy remaining data in pbuf */
            memcpy( (uint8_t*)((uint8_t*)q->payload + payloadoffset), (uint8_t*)((uint8_t*)buffer + bufferoffset), byteslefttocopy);
            bufferoffset = bufferoffset + byteslefttocopy;
        }
    }  
  
    /* Release descriptors to DMA */
    /* Point to first descriptor */
    dmarxdesc = heth.RxFrameInfos.FSRxDesc;
    /* Set Own bit in Rx descriptors: gives the buffers back to DMA */
    for (i=0; i< heth.RxFrameInfos.SegCount; i++)
    {  
        dmarxdesc->Status |= ETH_DMARXDESC_OWN;
        dmarxdesc = (ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
    }
    
    /* Clear Segment_Count */
    heth.RxFrameInfos.SegCount =0;  
  
    /* When Rx Buffer unavailable flag is set: clear it and resume reception */
    if ((heth.Instance->DMASR & ETH_DMASR_RBUS) != (uint32_t)RESET)  
    {
        /* Clear RBUS ETHERNET DMA flag */
        heth.Instance->DMASR = ETH_DMASR_RBUS;
        /* Resume DMA reception */
        heth.Instance->DMARPDR = 0;
    }
    
    return p;
}


//static err_t ethernetif_linkoutput(struct netif *netif, struct pbuf *p)
//{
//  netif = NULL;
//  void *msg;
//  
//  msg = p;
//  
//  if(sys_mbox_trypost(&eth_tx_mb,&msg) == ERR_OK)
//  {
//    PRINT_DEBUG("sys_mbox_trypost ok\n");
////    xSemaphoreTake(tx_sem,portMAX_DELAY);
//  }
//  
//  
//  return ERR_OK; 
//}

//void ethernetif_output(void *pParams) 
//{
//  void *msg;
//  struct pbuf *p;
//  struct netif *netif;  
//  netif = (struct netif*) pParams;
//  while(1)
//  {
//    if(sys_arch_mbox_fetch(&eth_tx_mb,&msg,0)==ERR_OK)
//    {
//      PRINT_DEBUG("sys_arch_mbox_fetch ok\n");
//      p = (struct pbuf *)msg;
//      
//      if(p!=NULL)
//      {
//        if(low_level_output(netif,p) == ERR_OK)
//        {
//          PRINT_DEBUG("low_level_output ok\n");
//          LED3_TOGGLE;
//        }
//      }
//      
//      /* send ACK */
////      sys_sem_signal(&tx_sem);
//    }
////    else
////      PRINT_DEBUG("not ok\n");
//  }
//  
//}


/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @note    网卡数据包解析函数，解析为ARP还是IP包
 */
void ethernetif_input(void *pParams) 
{
	struct netif *netif;
	struct pbuf *p = NULL;
	netif = (struct netif*) pParams;
    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
  
	while(1) 
    {
        /* 判断网卡是否接收到数据 */
        if(xSemaphoreTake( s_xSemaphore, portMAX_DELAY ) == pdTRUE)
        {
            /* move received packet into a new pbuf */
            taskENTER_CRITICAL();
            p = low_level_input(netif); /* 获取网卡数据的指针 */
            taskEXIT_CRITICAL();

            /* points to packet payload, which starts with an Ethernet header */
            if(p != NULL)
            {
                taskENTER_CRITICAL();

                /* full packet send to tcpip_thread to process */
                if (netif->input(p, netif) != ERR_OK)
                {
                    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
                    pbuf_free(p);
                    p = NULL;
                }

                taskEXIT_CRITICAL();
            }
        }
	}
}


#if !LWIP_ARP
/**
 * This function has to be completed by user in case of ARP OFF.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if ...
 */
static err_t low_level_output_arp_off(struct netif *netif, struct pbuf *q, const ip4_addr_t *ipaddr)
{  
  err_t errval;
  errval = ERR_OK;
    
/* USER CODE BEGIN 5 */ 
    
/* USER CODE END 5 */  
    
  return errval;
  
}
#endif /* LWIP_ARP */ 

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 * @note    lwip协议网卡管理函数，以太网初始化函数
 */
err_t ethernetif_init(struct netif *netif)
{
 	struct ethernetif *ethernetif;

	ethernetif = mem_malloc(sizeof(struct ethernetif));

	if (ethernetif == NULL) 
    {
		printf("ethernetif_init: out of memory\n");
		return ERR_MEM;
	}
  
    LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */


    netif->state = ethernetif;  /* 配置网卡状态 */
    netif->name[0] = IFNAME0;   /* 配置网络名字 */
    netif->name[1] = IFNAME1;

#if LWIP_IPV4
    #if LWIP_ARP || LWIP_ETHERNET
        #if LWIP_ARP
        netif->output = etharp_output;  /* 配置ARP协议处理接口 */
        #else
        /* The user should write ist own code in low_level_output_arp_off function */
        netif->output = low_level_output_arp_off;
        #endif /* LWIP_ARP */
    #endif /* LWIP_ARP || LWIP_ETHERNET */
#endif /* LWIP_IPV4 */
 
#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */

    /* 配置以太网发送函数 */
    netif->linkoutput = low_level_output;   
    
    /* 进行网卡初始化 */
    low_level_init(netif);
    ethernetif->ethaddr = (struct eth_addr *) &(netif->hwaddr[0]);
    //  etharp_init();
    //  sys_timeout(ARP_TMR_INTERVAL, arp_timer, NULL);
    return ERR_OK;
}
/* ARP定时器，1秒执行一次 */
static void arp_timer(void *arg)
{
    etharp_tmr();
    sys_timeout(ARP_TMR_INTERVAL, arp_timer, NULL);
}

/* USER CODE BEGIN 6 */

/**
* @brief  Returns the current time in milliseconds
*         when LWIP_TIMERS == 1 and NO_SYS == 1
* @param  None
* @retval Time
*/
//u32_t sys_jiffies(void)
//{
//  return HAL_GetTick();
//}

/**
* @brief  Returns the current time in milliseconds
*         when LWIP_TIMERS == 1 and NO_SYS == 1
* @param  None
* @retval Time
*/
//u32_t sys_now(void)
//{
//  return HAL_GetTick();
//}

/* USER CODE END 6 */

/* USER CODE BEGIN 7 */

/* USER CODE END 7 */

#if LWIP_NETIF_LINK_CALLBACK
/**
  * @brief  Link callback function, this function is called on change of link status
  *         to update low level driver configuration.
* @param  netif: The network interface
  * @retval None
  */
void ethernetif_update_config(struct netif *netif)
{
    __IO uint32_t tickstart = 0;
    uint32_t regvalue = 0;
    
    if(netif_is_link_up(netif))
    { 
        /* Restart the auto-negotiation */
        if(heth.Init.AutoNegotiation != ETH_AUTONEGOTIATION_DISABLE)
        {
        /* Enable Auto-Negotiation */
        HAL_ETH_WritePHYRegister(&heth, PHY_BCR, PHY_AUTONEGOTIATION);
        
        /* Get tick */
        tickstart = HAL_GetTick();
        
        /* Wait until the auto-negotiation will be completed */
        do
        {
            HAL_ETH_ReadPHYRegister(&heth, PHY_BSR, &regvalue);
            
            /* Check for the Timeout ( 1s ) */
            if((HAL_GetTick() - tickstart ) > 1000)
            {     
            /* In case of timeout */ 
            goto error;
            }   
        } while (((regvalue & PHY_AUTONEGO_COMPLETE) != PHY_AUTONEGO_COMPLETE));
        
        /* Read the result of the auto-negotiation */
        HAL_ETH_ReadPHYRegister(&heth, PHY_SR, &regvalue);
        
        /* Configure the MAC with the Duplex Mode fixed by the auto-negotiation process */
        if((regvalue & PHY_DUPLEX_STATUS) != (uint32_t)RESET)
        {
            /* Set Ethernet duplex mode to Full-duplex following the auto-negotiation */
            heth.Init.DuplexMode = ETH_MODE_FULLDUPLEX;  
        }
        else
        {
            /* Set Ethernet duplex mode to Half-duplex following the auto-negotiation */
            heth.Init.DuplexMode = ETH_MODE_HALFDUPLEX;           
        }
        /* Configure the MAC with the speed fixed by the auto-negotiation process */
        if(regvalue & PHY_SPEED_STATUS)
        {  
            /* Set Ethernet speed to 10M following the auto-negotiation */
            heth.Init.Speed = ETH_SPEED_10M; 
        }
        else
        {   
            /* Set Ethernet speed to 100M following the auto-negotiation */ 
            heth.Init.Speed = ETH_SPEED_100M;
        }
        }
        else /* AutoNegotiation Disable */
        {
        error :
        /* Check parameters */
        assert_param(IS_ETH_SPEED(heth.Init.Speed));
        assert_param(IS_ETH_DUPLEX_MODE(heth.Init.DuplexMode));
        
        /* Set MAC Speed and Duplex Mode to PHY */
        HAL_ETH_WritePHYRegister(&heth, PHY_BCR, ((uint16_t)(heth.Init.DuplexMode >> 3) |
                                                        (uint16_t)(heth.Init.Speed >> 1))); 
        }

        /* ETHERNET MAC Re-Configuration */
        HAL_ETH_ConfigMAC(&heth, (ETH_MACInitTypeDef *) NULL);

        /* Restart MAC interface */
        HAL_ETH_Start(&heth);   
    }
    else
    {
        /* Stop MAC interface */
        HAL_ETH_Stop(&heth);
    }

    ethernetif_notify_conn_changed(netif);
}

/* USER CODE BEGIN 8 */
/**
  * @brief  This function notify user about link status changement.
  * @param  netif: the network interface
  * @retval None
  */
__weak void ethernetif_notify_conn_changed(struct netif *netif)
{
  /* NOTE : This is function could be implemented in user file 
            when the callback is needed,
  */

}
/* USER CODE END 8 */ 
#endif /* LWIP_NETIF_LINK_CALLBACK */

/* USER CODE BEGIN 9 */

/* USER CODE END 9 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

