#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "led.h"
#include "sleep.h"

int main(void)
{
    //请不要在这里使用systick方式延时,systick中断也会唤醒wfe
    
    uart_initwBaudRate(115200);	 //串口初始化为115200
    LED_Init();
    printf("sleep mode ! \r\n");
    sleep_test();   
    
    //按下K2，被唤醒
    while(1)//无限循环
    {
        printf("Wake Up ! \r\n");
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        delay_ms(1000);
    }	
}



