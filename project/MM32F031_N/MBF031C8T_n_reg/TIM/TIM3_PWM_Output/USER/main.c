#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "pwm.h"
#include "led.h"
/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� ���ϵ�󣬲鿴LED�𽥱�����Ȼ���ֱ䰵����˷���
**������� ��
**������� ��
********************************************************************************************************/

int main(void)
{
    u8 dir=1;	
    u16 led0pwmval=0;    
    
    delay_init();	
    uart_initwBaudRate(115200);
    LED_Init();
    TIM3_PWM_Init(99,0);//����Ƶ
    while(1)                                                                                               //����ѭ��
    {
        delay_ms(4);	 
        if(dir)led0pwmval++;
        else led0pwmval--;	 
        if(led0pwmval>300)dir=0;
        if(led0pwmval==0)dir=1;	   					 
        TIM3->CCR1 = led0pwmval;
    }	
}


