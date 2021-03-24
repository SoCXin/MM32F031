#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "led.h"
/********************************************************************************************************
**函数信息 ：int main (void)                          
**功能描述 ：开机后，ARMLED闪动
**输入参数 ：
**输出参数 ：
********************************************************************************************************/

int main(void)
{
    
    //GPIO口测试，闪烁LED灯
    delay_init();
    LED_Init();
    while(1)              //无限循环
    {
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        delay_ms(1000);
    }
}




