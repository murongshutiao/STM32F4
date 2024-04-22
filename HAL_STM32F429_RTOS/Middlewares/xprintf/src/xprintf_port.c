
#include "xprintf.h"

int xprintf_out(int ch)
{

uint8_t c=ch;

uart_write(&c,1);

return ch;

}

int xprintf_in(void)
{

int len=0;

uint8_t ch=0;

do

{

len = uart_read(&ch, 1);

}while(len == 0);

return ch;

}



xdev_out(xprintf_out);

xdev_in(xprintf_in);