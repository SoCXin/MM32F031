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
    TIM_TimeBaseInitTypeDef TIM_StructInit;
    
    /*使能TIM1时钟,默认时钟源为PCLK2(PCLK2未分频时不倍频,否则由PCLK2倍频输出),可选其它时钟源*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
    
    TIM_StructInit.TIM_Period=Period;                                                                      //ARR寄存器值
    TIM_StructInit.TIM_Prescaler=Prescaler;                                                                //预分频值
    /*数字滤波器采样频率,不影响定时器时钟*/
    TIM_StructInit.TIM_ClockDivision=TIM_CKD_DIV1;                                                         //采样分频值
    TIM_StructInit.TIM_CounterMode=TIM_CounterMode_Up;                                                     //计数模式
    TIM_StructInit.TIM_RepetitionCounter=0;
    
    TIM_TimeBaseInit(TIM1, &TIM_StructInit);
    
    TIM_Cmd(TIM1, ENABLE);	
    
    /*更新定时器时会产生更新时间,清除标志位*/
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
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
    while(TIM_GetFlagStatus(TIM1,TIM_FLAG_Update)==RESET);
    /*清事件标志位*/
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
}

