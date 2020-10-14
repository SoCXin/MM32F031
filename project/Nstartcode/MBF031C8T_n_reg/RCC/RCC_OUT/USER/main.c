#include "delay.h"
#include "sys.h"
/********************************************************************************************************
**������Ϣ ��main(void)                     
**�������� ��
**������� ����
**������� ����
********************************************************************************************************/
extern u32 SystemCoreClock;

int main(void)    
{
    
    RCC->AHBENR|=0x1<<17;    //ʹ��GPIOAʱ��
    RCC->APB2ENR|=0x1;
    GPIOA->CRH&=0XFFFFFFF0; 
    GPIOA->CRH|=0xb;//PA8 �������   	 
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



