#include "./Module/letter_shell/port/shell_port.h"


/* 1.创建Shell对象，初始化Shell缓冲区 */
Shell shell;
char shell_buffer[512];

/* 2.编写接口函数 */
void User_Shell_Write(const char ch)
{
    HAL_UART_Transmit(&DebugUartHandler,(uint8_t *)&ch, 1, 0xFFFF);
}

/* 初始化letter-shell */
void User_Shell_Init(void)
{
    shell.write = User_Shell_Write;

    shellInit(&shell, shell_buffer,512);
}







