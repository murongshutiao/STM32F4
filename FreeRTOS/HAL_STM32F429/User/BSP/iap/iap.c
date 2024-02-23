#include "./iap/iap.h"


void NVIC_SetVectorTable(uint32_t Addr,uint32_t offset)
{
    SCB->VTOR = Addr | (offset & (uint32_t)0xFFFFFE00);
}

void Jump_Dfu_BootLoader(void)
{
    __IO uint32_t BootAddr = 0x1FFF0000;
    void (*SysMemBootJump)(void);

    __set_PRIMASK(1);   /* �ر�ȫ���ж� */

    /* �رյδ�ʱ�� */
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;

    /* ��������ʱ�ӵ�Ĭ��״̬��ʹ��HSIʱ�� */
    HAL_RCC_DeInit();

    /* �ر������жϣ���������жϹ����־ */
    for(uint8_t i=0;i<8;i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF;
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }

    __set_PRIMASK(0);

    /* ������ӳ�䵽ϵͳFlash */
    __HAL_SYSCFG_REMAPMEMORY_SYSTEMFLASH();

    /* ��ת��ϵͳBootloader,�׵�ַ��MSP,��ַ+4�Ǹ�λ�жϷ�������ַ */
    SysMemBootJump = (void(*)(void))(*((uint32_t *)(BootAddr + 4)));//STM32F4_Bootloader_RstAddr;

    __set_MSP(*(uint32_t *)BootAddr);
    __set_CONTROL(0);

    SysMemBootJump();

    while(1)
    {
        /* ��תʧ�� */
        LED2_ON;
    }

}

uint32_t g_iapbuf[IAP_WriteBufSize];     /* 2k�ֽڻ��� */



void IAP_WriteAppToFlash(uint32_t appxaddr,uint8_t *appbuf,uint32_t appsize)
{
    uint16_t i = 0;
    uint32_t fwaddr = appxaddr;
    uint8_t *dfu = appbuf;

    InternalFlashEraseSector(APP_ADDR1,APP_ADDR1 + (appsize / 4));

    for(uint32_t t=0; t<appsize; t+=4)
    {

        g_iapbuf[i++] = pU8ToU32(dfu);
        dfu += 4;

        if(i == IAP_WriteBufSize)
        {
            i = 0;
            InternalFlash_WriteData(fwaddr,g_iapbuf,512);
            fwaddr += (IAP_WriteBufSize * 4);
        }
    }

    if(i)
    {
        InternalFlash_WriteData(fwaddr,g_iapbuf,i);
    }
}

void IAP_Load_App(uint32_t AppAddr)
{
    pFunction JumpToApplication;
    uint32_t JumpAddress;

    /* Test if user code is programmed starting from address "APPLICATION_ADDRESS" */
    if (((*(__IO uint32_t*)AppAddr) & 0x2FF00000 ) == 0x20000000)
    {
        JumpAddress = *(__IO uint32_t*) (AppAddr + 4);
        JumpToApplication = (pFunction) JumpAddress;
        
        __set_MSP(*(__IO uint32_t*) AppAddr);
        JumpToApplication();
    }
}

void IAP_Proc(void)
{
    if(DebugUartReveiveData.semaphore == 1)
    {
        DebugUartReveiveData.semaphore = 0;

        IAP_WriteAppToFlash(APP_ADDR1,DebugUartReceiveBuffer,DebugUartReveiveData.len);
        DebugUartReveiveData.len = 0;
        IAP_Load_App(APP_ADDR1);
    }
}












