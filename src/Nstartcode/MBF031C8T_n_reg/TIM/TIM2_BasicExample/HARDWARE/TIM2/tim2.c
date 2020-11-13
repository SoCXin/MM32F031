#include "tim2.h" 
#include "sys.h"
/********************************************************************************************************
**函数信息 ：void TIM2_IRQHandler(void)                  
**功能描述 ：定时器2中断函数
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void TIM2_IRQHandler(void)
{
    //  /*必须手动清除中断标志位*/
    //	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    //	
    //	/*全局标志*/
    //	ucTim2Flag++;
    if(TIM2->SR&TIM_SR_UIF)//溢出中断
    {
        /*全局标志*/
        ucTim2Flag++;
    }
    TIM2->SR&=~(TIM_SR_UIF);//清除中断标志位 	    
    
}


/********************************************************************************************************
**函数信息 ：void Tim2_UPCount_test1(u16 Period,u16 Prescaler)                     
**功能描述 ：配置定时器2向上计数模式
**输入参数 ：Period 16位计数器重载值
Prescaler 时钟预分频值
**输出参数 ：无
********************************************************************************************************/
void Tim2_UPCount_test(u16 Prescaler,u16 Period)
{
    RCC->APB1ENR|=RCC_APB1ENR_TIM2EN;	//TIM3时钟使能    
    TIM2->ARR=Prescaler;  	//设定计数器自动重装值 
    TIM2->PSC=Period;  	//预分频器设置
    TIM2->DIER|=TIM_DIER_UIE;   //允许更新中断				
    TIM2->CR1|=TIM_CR1_CEN;    //使能定时器3
    TIM2->CR1|=TIM_CR1_CEN;    //使能定时器3
    MY_NVIC_Init(1,3,TIM2_IRQn,2);//抢占1，子优先级3，组2			
    
}


