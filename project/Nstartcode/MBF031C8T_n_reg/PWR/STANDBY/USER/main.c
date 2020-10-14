#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "wkup.h"
#include "led.h"

int main(void)
{ 
    
    delay_init();
    LED_Init();		  		//初始化与LED连接的硬件接口
    uart_initwBaudRate(115200);	 	//串口初始化为115200
    printf("请按下wakeup按键3秒不放\r\n");
    WKUP_Init();			//初始化WK_UP按键，同时检测是否正常开机
    
    while(1)
    {
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        delay_ms(250);//延时250ms
        printf("已从standby模式唤醒\r\n");
    }
    
}

