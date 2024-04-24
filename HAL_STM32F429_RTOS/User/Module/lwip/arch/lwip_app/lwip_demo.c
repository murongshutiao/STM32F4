/**
 ****************************************************************************************************
 * @file        lwip_demo
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-8-01
 * @brief       lwIP WebServer 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 阿波罗 F429开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */
 
#include "lwip_demo.h"
#include "httpd.h"
#include "httpd_cgi_ssi.h"

/**
 * @brief       lwip_demo 测试
 * @param       无
 * @retval      无
 */
void lwip_demo(void)
{
    /* Httpd Init */
    httpd_init();   /* */

    /* 配置SSI处理程序 */
    httpd_ssi_init();

    /* 配置CGI处理器 */
    httpd_cgi_init();
}
