#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"
/********************************************************************************************************
**函数信息 ：main(void)                       
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
int main(void)
{
    u8 t=0;
    
    delay_init();	
    LED_Init();		  	 	//初始化与LED连接的硬件接口
    KEY_Init();          	//初始化与按键连接的硬件接口
    
    while(1)
    {
        t=KEY_Scan(0);		//得到键值
        switch(t)
        {				 
        case KEY1_PRES://K1默认不连PC13,默认连接reset复位按键，所以按下K1会复位
            LED1_TOGGLE();
            break;
        case WKUP_PRES:
            LED2_TOGGLE();
            break;
        case KEY3_PRES:				
            LED3_TOGGLE();
            break;
        case KEY4_PRES:				
            LED4_TOGGLE();
            break;
        default:
            delay_ms(10);	
        } 
    }
}



