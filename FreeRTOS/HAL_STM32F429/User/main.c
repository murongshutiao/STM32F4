#include "main.h"
#include "board.h"

/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"



static TaskHandle_t AppTaskCreate_Handle = NULL;/* ���������� */
static TaskHandle_t Test1_Task_Handle = NULL;/* LED������ */
static TaskHandle_t Test2_Task_Handle = NULL;/* KEY������ */



static void AppTaskCreate(void);/* ���ڴ������� */

static void Test1_Task(void* pvParameters);/* Test1_Task����ʵ�� */
static void Test2_Task(void* pvParameters);/* Test2_Task����ʵ�� */

extern void TCPIP_Init(void);


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
    BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
    
    /* ������Ӳ����ʼ�� */
    BSP_Init();
  
//  tcpecho_init();
  
    /* ����AppTaskCreate���� */
    xReturn = xTaskCreate   ((TaskFunction_t)AppTaskCreate,  /* ������ں��� */
                            (const char*    )"AppTaskCreate",/* �������� */
                            (uint16_t       )512,  /* ����ջ��С */
                            (void*          )NULL,/* ������ں������� */
                            (UBaseType_t    )1, /* ��������ȼ� */
                            (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 
    /* ����������� */           
    if(pdPASS == xReturn)
    {
        vTaskStartScheduler();   /* �������񣬿������� */
    }
    else
    {
        return -1;  
    }
  
    while(1);   /* ��������ִ�е����� */ 
}


/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
    BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
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
    httpd_init();       /* httpd��������ʼ�� */
    httpd_ssi_init();
    httpd_cgi_init();
    
    taskENTER_CRITICAL();           //�����ٽ���

    /* ����Test1_Task���� */
    xReturn = xTaskCreate((TaskFunction_t )Test1_Task, /* ������ں��� */
                            (const char*    )"Test1_Task",/* �������� */
                            (uint16_t       )512,   /* ����ջ��С */
                            (void*          )NULL,	/* ������ں������� */
                            (UBaseType_t    )1,	    /* ��������ȼ� */
                            (TaskHandle_t*  )&Test1_Task_Handle);/* ������ƿ�ָ�� */
    if(pdPASS == xReturn)
    {
        printf("Create Test1_Task sucess...\r\n");
    }
  
    /* ����Test2_Task���� */
    xReturn = xTaskCreate((TaskFunction_t )Test2_Task,  /* ������ں��� */
                            (const char*    )"Test2_Task",/* �������� */
                            (uint16_t       )512,  /* ����ջ��С */
                            (void*          )NULL,/* ������ں������� */
                            (UBaseType_t    )2, /* ��������ȼ� */
                            (TaskHandle_t*  )&Test2_Task_Handle);/* ������ƿ�ָ�� */ 
    if(pdPASS == xReturn)
    {
        printf("Create Test2_Task sucess...\n\n");
    }
    
    vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
    
    taskEXIT_CRITICAL();            //�˳��ٽ���
}



/**********************************************************************
  * @ ������  �� Test1_Task
  * @ ����˵���� Test1_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void Test1_Task(void* parameter)
{	
    while (1)
    {
        LED1_TOGGLE;
    //    PRINT_DEBUG("LED1_TOGGLE\n");
        vTaskDelay(1000);/* ��ʱ1000��tick */
    }
}

/**********************************************************************
  * @ ������  �� Test2_Task
  * @ ����˵���� Test2_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void Test2_Task(void* parameter)
{	 
    while (1)
    {
        LED2_TOGGLE;
    //    PRINT_DEBUG("LED2_TOGGLE\n");
        vTaskDelay(2000);/* ��ʱ2000��tick */
    }
}





/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
