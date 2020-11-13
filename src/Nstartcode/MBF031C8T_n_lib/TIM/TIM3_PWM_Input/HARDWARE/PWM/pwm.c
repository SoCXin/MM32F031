/**
******************************************************************************
* @file    wwdg_test.c  
* @brief   配置定时器2,实现两路可调10KHz PWM功能
*          PA6,PA7可观察输出波形
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "pwm.h" 
#include "uart.h"	  

u16 period = 0;
u16 duty  = 0;
u8 CollectFlag = 0;

void TIM2_PWM_Init(u16 arr,u16 psc)      //用T2的CH3   APB1
{  
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  //开启GPIOB时钟
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG , ENABLE);  //使能GPIO外设时钟使能
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //TIM2_CH3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_2);  //PB10复用功能的AF2
    
    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
    
    
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH3预装载使能	 
    
    TIM_ARRPreloadConfig(TIM2, ENABLE); //使能TIMx在ARR上的预装载寄存器
    
    TIM_Cmd(TIM2, ENABLE);  //使能TIM2
    
    
}



/*功能名称TIM3_PWMINPUT_INIT(u16 arr,u16 psc)
描述      PWM输入初始化*/
void TIM3_PWMINPUT_INIT(u16 arr,u16 psc)   //用T3的CH1 APB1  PA6    AF1
{
    
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ICInitTypeDef  TIM3_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);     //Open TIM3 clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //开启GPIOA时钟
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_1);  //PA6复用功能的AF1
    
    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值  
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    
    
    /*配置中断优先级*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;                     
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1;                   
    TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;       
    TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;   
    TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; 
    TIM3_ICInitStructure.TIM_ICFilter = 0x0; 
    
    TIM_PWMIConfig(TIM3, &TIM3_ICInitStructure);     //PWM输入配置           
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);     //选择有效输入端       
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);  //配置为主从复位模式
    
    TIM_ITConfig(TIM3, TIM_IT_CC1|TIM_IT_Update, ENABLE);          //中断配置
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
    TIM_Cmd(TIM3, ENABLE);    
}

void TIM3_IRQHandler(void)
{
    
    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
    {	
        period	=	TIM_GetCapture1(TIM3);     //采集周期
        duty = TIM_GetCapture2(TIM3); 				//采集占空比				  
        CollectFlag = 1;
    }		
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
}



