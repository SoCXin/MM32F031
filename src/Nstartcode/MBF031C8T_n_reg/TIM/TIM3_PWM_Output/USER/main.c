#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "pwm.h"
#include "led.h"
/********************************************************************************************************
**函数信息 ：int main (void)                          
**功能描述 ：上电后，查看LED逐渐变亮，然后又变暗，如此反复
**输入参数 ：
**输出参数 ：
********************************************************************************************************/

int main(void)
{
    u8 dir=1;	
    u16 led0pwmval=0;    
    
    delay_init();	
    uart_initwBaudRate(115200);
    LED_Init();
    TIM3_PWM_Init(99,0);//不分频
    while(1)                                                                                               //无限循环
    {
        delay_ms(4);	 
        if(dir)led0pwmval++;
        else led0pwmval--;	 
        if(led0pwmval>300)dir=0;
        if(led0pwmval==0)dir=1;	   					 
        TIM3->CCR1 = led0pwmval;
    }	
}


