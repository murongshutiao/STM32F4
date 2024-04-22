#include "./Module/letter_shell/port/shell_port.h"


/* 1.����Shell���󣬳�ʼ��Shell������ */
Shell shell;
char shell_buffer[512];

/* 2.��д�ӿں��� */
void User_Shell_Write(const char ch)
{
    HAL_UART_Transmit(&DebugUartHandler,(uint8_t *)&ch, 1, 0xFFFF);
}

/* ��ʼ��letter-shell */
void User_Shell_Init(void)
{
    shell.write = User_Shell_Write;

    shellInit(&shell, shell_buffer,512);
}







