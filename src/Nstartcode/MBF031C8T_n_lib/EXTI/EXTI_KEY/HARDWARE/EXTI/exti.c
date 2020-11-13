#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//开发板
//外部中断 驱动代码			   
////////////////////////////////////////////////////////////////////////////////// 	  


//外部中断初始化函数
void EXTIX_Init(void)
{
    
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);//外部中断，需要使能AFIO时钟
    
    KEY_Init();//初始化按键对应io模式
    
    //GPIOC.13 中断线以及中断初始化配置
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource13);
    
    EXTI_InitStructure.EXTI_Line=EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
    
    //GPIOA.0	  中断线以及中断初始化配置
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
    
    EXTI_InitStructure.EXTI_Line=EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
    
    //GPIOB.10	  中断线以及中断初始化配置
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource10);
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line10;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//上升沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
    
    //GPIOB.11	  中断线以及中断初始化配置
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource11);
    
    EXTI_InitStructure.EXTI_Line=EXTI_Line11;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//上升沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
    
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;			//使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02;					//子优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;			//使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;					//子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure); 
    
}

u16 EXTI_DELAY=0;

void EXTI0_1_IRQHandler(void)
{
    for(EXTI_DELAY=0;EXTI_DELAY<1000;EXTI_DELAY++);
    if(WK_UP==1)
    {	  
        LED2_TOGGLE();
    }
    EXTI_ClearFlag(EXTI_Line0);  //清除EXTI0线路挂起位
}


void EXTI4_15_IRQHandler(void)
{			
    for(EXTI_DELAY=0;EXTI_DELAY<1000;EXTI_DELAY++);
    if(KEY1==0)
    {	  
        LED1_TOGGLE();
    }
    else if(KEY3==0)
    {	  
        LED3_TOGGLE();
    }	
    else if(KEY4==0)
    {	  
        LED4_TOGGLE();
    }
    EXTI_ClearFlag(EXTI_Line10|EXTI_Line11|EXTI_Line13);   //清除EXTI10，11，13线路挂起位
    
}


