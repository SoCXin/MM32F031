#include "tim1.h" 

/********************************************************************************************************
**函数信息 ：void Tim1_UPCount_test1(u16 Period,u16 Prescaler)                     
**功能描述 ：配置定时器1向上计数模式
**输入参数 ：Period 16位计数器重载值
Prescaler 时钟预分频值
**输出参数 ：无
********************************************************************************************************/
void Tim1_UPCount_test(u16 Prescaler,u16 Period)
{	
    /*使能TIM1时钟,默认时钟源为PCLK2(PCLK2未分频时不倍频,否则由PCLK2倍频输出),可选其它时钟源*/
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC->APB2ENR|=RCC_APB2ENR_TIM1EN;
    TIM1->ARR=Prescaler;  	//设定计数器自动重装值 
    TIM1->PSC=Period;  	//预分频器设置
    TIM1->CR1|=TIM_CR1_CEN;    //使能定时器3	
    
    
}
/********************************************************************************************************
**函数信息 ：Tim1_UPStatusOVCheck_test1(void)                       
**功能描述 ：等待定时器溢出
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void Tim1_UPStatusOVCheck_test(void)
{
    /*等待事件更新标志位*/
    while((TIM1->SR&TIM_SR_UIF)==RESET);
    /*清事件标志位*/
    
    TIM1->SR=~TIM_SR_UIF;
}


