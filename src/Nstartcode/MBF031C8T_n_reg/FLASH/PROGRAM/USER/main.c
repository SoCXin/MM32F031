#include "delay.h"
#include "sys.h"
#include "led.h"
#include "uart.h"
#include "flash.h"

/********************************************************************************************************
**函数信息 ：main(void)                     
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
int main(void)    
{
    u8 t;
    
    delay_init();	
    uart_initwBaudRate(115200);
    LED_Init();
    t=FLASH_Program();
    
    if(t==0)
        while(1)              //无限循环
        {
            printf("program ok\r\n");
            LED1_TOGGLE();
            LED2_TOGGLE();
            LED3_TOGGLE();
            LED4_TOGGLE();
            delay_ms(1000);
        }
    else
        while(1)
        {
            printf("program error\r\n");
            delay_ms(1000);
        }
}



