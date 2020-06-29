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
**函数信息 ：int main (void)                          
**功能描述 ：使用杜邦线连接板子上的PA6(Ain4)与PB10(K3)                                                                                                                                                                                                                                                                                                                                          )
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
int main(void)
{
    
    delay_init();
    uart_initwBaudRate(115200);
    LED_Init();
    TIM2_PWM_Init(1000-1,SystemCoreClock/1000000-1);   //1KHZ周期
    TIM_SetCompare3(TIM2,600);		//设置占空比  pa2
    TIM3_PWMINPUT_INIT(0xffff,SystemCoreClock/1000000-1);   //pwm输入初始化以1M的频率捕捉
    printf("请连接MiniBoard上的PA6(Ain4)与PB10(K3)\r\n");
    while(1)
    {
        delay_ms(500);
        if(CollectFlag==1)
        {
            printf("period  = %dus\r\n",period);    //打印周期,大约是1MS
            CollectFlag = 0;
        }
    }	
}


