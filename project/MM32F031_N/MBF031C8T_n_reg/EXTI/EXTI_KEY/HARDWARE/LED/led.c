#include "led.h"

/********************************************************************************************************
**������Ϣ ��LED_Init(void)                        
**�������� ��LED��ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void LED_Init(void)
{
    
    RCC->AHBENR|=0x7<<17;   //ʹ��GPIOAʱ��	   	 
    //JTAG_Set(SWD_ENABLE);	//�ر�JTAG,����SWD
    
    GPIOA->CRH|=GPIO_CRH_MODE15;           //PA15 �������   	 
    GPIOA->ODR|=GPIO_ODR_ODR15;            // GPIOA->ODR|=1<<15;   //PA15 �����
    
    GPIOB->CRL|=GPIO_CRL_MODE3|GPIO_CRL_MODE4|GPIO_CRL_MODE5;      ////PB3,PB4,PB5�������
    GPIOB->ODR|=GPIO_ODR_ODR3|GPIO_ODR_ODR4|GPIO_ODR_ODR5;          ////PB.3,PB4,PB5����� 
    
    LED1_OFF();
    LED2_OFF();
    LED3_OFF();
    LED4_OFF();
}

