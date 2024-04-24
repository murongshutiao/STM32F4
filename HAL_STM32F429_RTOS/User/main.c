#include "main.h"
#include "board.h"

/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



static TaskHandle_t AppTaskCreate_Handle = NULL;/* 创建任务句柄 */
static TaskHandle_t Test1_Task_Handle = NULL;/* LED任务句柄 */
static TaskHandle_t Test2_Task_Handle = NULL;/* KEY任务句柄 */



static void AppTaskCreate(void);/* 用于创建任务 */

static void Test1_Task(void* pvParameters);/* Test1_Task任务实现 */
static void Test2_Task(void* pvParameters);/* Test2_Task任务实现 */

extern void TCPIP_Init(void);


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
    
    /* 开发板硬件初始化 */
    BSP_Init();
  
    /* 创建AppTaskCreate任务 */
    xReturn = xTaskCreate   ((TaskFunction_t)AppTaskCreate,         /* 任务入口函数 */
                            (const char*    )"AppTaskCreate",       /* 任务名字 */
                            (uint16_t       )512,                   /* 任务栈大小 */
                            (void*          )NULL,                  /* 任务入口函数参数 */
                            (UBaseType_t    )1,                     /* 任务的优先级 */
                            (TaskHandle_t*  )&AppTaskCreate_Handle);/* 任务控制块指针 */ 
    /* 启动任务调度 */           
    if(pdPASS == xReturn)
    {
        vTaskStartScheduler();   /* 启动任务，开启调度 */
    }
    else
    {
        return -1;  
    }
  
    while(1);   /* 正常不会执行到这里 */ 
}


/***********************************************************************
  * @ 函数名  ： AppTaskCreate
  * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void AppTaskCreate(void)
{
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
    TCPIP_Init();
    // Netconn_TCP_Client_Init();
    // Netconn_TCP_Server_Init();
    // Netconn_UDP_Server_Init();

    //Socket_TCP_Client_Init();

    //Htts_Client_init();
     //http_server_netconn_init();
    // Http_Server_Socket_Init();
    // http_server_socket_init();
    // Web_Cgi_Init(void);
    httpd_init();       /* httpd服务器初始化 */
    httpd_ssi_init();
    httpd_cgi_init();
    
    taskENTER_CRITICAL();           //进入临界区

    /* 创建Test1_Task任务 */
    xReturn = xTaskCreate((TaskFunction_t )Test1_Task, /* 任务入口函数 */
                            (const char*    )"Test1_Task",/* 任务名字 */
                            (uint16_t       )512,   /* 任务栈大小 */
                            (void*          )NULL,	/* 任务入口函数参数 */
                            (UBaseType_t    )1,	    /* 任务的优先级 */
                            (TaskHandle_t*  )&Test1_Task_Handle);/* 任务控制块指针 */
    if(pdPASS == xReturn)
    {
        printf("Create Test1_Task sucess...\r\n");
    }
  
    /* 创建Test2_Task任务 */
    xReturn = xTaskCreate((TaskFunction_t )Test2_Task,  /* 任务入口函数 */
                            (const char*    )"Test2_Task",/* 任务名字 */
                            (uint16_t       )512,  /* 任务栈大小 */
                            (void*          )NULL,/* 任务入口函数参数 */
                            (UBaseType_t    )2, /* 任务的优先级 */
                            (TaskHandle_t*  )&Test2_Task_Handle);/* 任务控制块指针 */ 
    if(pdPASS == xReturn)
    {
        printf("Create Test2_Task sucess...\n\n");
    }
    
    vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务
    
    taskEXIT_CRITICAL();            //退出临界区
}



/**********************************************************************
  * @ 函数名  ： Test1_Task
  * @ 功能说明： Test1_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void Test1_Task(void* parameter)
{	
    while (1)
    {
        LED1_TOGGLE;
    //    PRINT_DEBUG("LED1_TOGGLE\n");
        vTaskDelay(1000);/* 延时1000个tick */
    }
}

/**********************************************************************
  * @ 函数名  ： Test2_Task
  * @ 功能说明： Test2_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void Test2_Task(void* parameter)
{	 
    while (1)
    {
        LED2_TOGGLE;
    //    PRINT_DEBUG("LED2_TOGGLE\n");
        vTaskDelay(2000);/* 延时2000个tick */
    }
}





/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
