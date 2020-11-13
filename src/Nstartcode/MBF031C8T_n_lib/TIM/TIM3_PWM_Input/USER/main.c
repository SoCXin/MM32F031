#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "pwm.h"
#include "led.h"

extern u16 period;
extern u16 duty ;
extern u8 CollectFlag ;
extern u32 SystemCoreClock;

/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� ��ʹ�öŰ������Ӱ����ϵ�PA6(Ain4)��PB10(K3)                                                                                                                                                                                                                                                                                                                                          )
**������� ��
**������� ��
********************************************************************************************************/
int main(void)
{
    
    delay_init();
    uart_initwBaudRate(115200);
    LED_Init();
    TIM2_PWM_Init(1000-1,SystemCoreClock/1000000-1);   //1KHZ����
    TIM_SetCompare3(TIM2,600);		//����ռ�ձ�  pa2
    TIM3_PWMINPUT_INIT(0xffff,SystemCoreClock/1000000-1);   //pwm�����ʼ����1M��Ƶ�ʲ�׽
    printf("������MiniBoard�ϵ�PA6(Ain4)��PB10(K3)\r\n");
    while(1)
    {
        delay_ms(500);
        if(CollectFlag==1)
        {
            printf("period  = %dus\r\n",period);    //��ӡ����,��Լ��1MS
            CollectFlag = 0;
        }
    }	
}


