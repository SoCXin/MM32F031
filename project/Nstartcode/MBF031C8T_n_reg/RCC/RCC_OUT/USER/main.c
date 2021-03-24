#include "delay.h"
#include "sys.h"
/********************************************************************************************************
**函数信息 ：main(void)                     
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
extern u32 SystemCoreClock;

int main(void)    
{
    
    RCC->AHBENR|=0x1<<17;    //使能GPIOA时钟
    RCC->APB2ENR|=0x1;
    GPIOA->CRH&=0XFFFFFFF0; 
    GPIOA->CRH|=0xb;//PA8 推挽输出   	 
    GPIOA->AFRH&=~0XF;
    if(SystemCoreClock<8000000){
        RCC->CFGR |= (uint32_t)RCC_CFGR_MCO_HSE ;//PLL/2
    }
    else if(SystemCoreClock<=48000000){
        RCC->CFGR |= (uint32_t)RCC_CFGR_MCO_SYSCLK ;//PLL/2
    }
    else{
        RCC->CFGR |= (uint32_t)RCC_CFGR_MCO_PLL ;//PLL/2
    }
    while(1);
}



