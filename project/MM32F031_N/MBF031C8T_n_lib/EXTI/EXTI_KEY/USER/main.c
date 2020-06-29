#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "exti.h"
#include "key.h"
#include "led.h"


int main(void)
{
    //GPIO口测试，闪烁LED灯
    
    LED_Init();		  	 	//初始化与LED连接的硬件接口
    KEY_Init();          	//初始化与按键连接的硬件接口
    EXTIX_Init();
    
    while(1)
    {
    }
}


