/**
******************************************************************************
* @file    pwm.c  
* @brief   配置定时器3,实现两路可调10KHz PWM功能
*          PA6,PA7可观察输出波形
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "pwm.h" 

//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
    
    RCC->APB1ENR|=RCC_APB1ENR_TIM3EN; 	//TIM3时钟使能    
    RCC->AHBENR|=0x7<<17;   //RCC->APB2ENR|=1<<3;  //使能GPIOB时钟
    
    //SYSCFG->CFGR|=0x1<<11;
    GPIOB->AFRL=0x10000;
    GPIOB->CRL&=0XFFF0FFFF;	//PB4清除之前的设置
    GPIOB->CRL|=GPIO_CRL_MODE4|GPIO_CRL_CNF4_1;	//复用功能输出 
    
    TIM3->ARR=arr;
    TIM3->PSC=psc;
    
    TIM3->CCMR1|=TIM_CCMR1_IC1F_0|TIM_CCMR1_IC1F_1|TIM_CCMR1_IC1F_2;  	//CH1 PWM2模式		 
    TIM3->CCMR1|=TIM_CCMR1_OC1PE; 		//CH1预装载使能	 
    TIM3->CCER|=TIM_CCER_CC1E;   	//OC1 输出使能	   
    TIM3->CR1=TIM_CR1_ARPE;   	//ARPE使能 
    TIM3->CR1|=TIM_CR1_CEN;    	//使能定时器1 	
    
}



