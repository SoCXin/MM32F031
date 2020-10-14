/**--------------File Info---------------------------------------------------------------------------------
** File name:		  	sleep.c
** Descriptions:		    main 函数调用
**
*********************************************************************************************************/
#include "sleep.h"
#include "HAL_conf.h"
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //开启GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    /*配置PA0位上拉输入*/
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init( GPIOA, &GPIO_InitStruct);
    
    /*选择PA0位外部事件触发输入*/
    SYSCFG_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    
    /*使能外部中断0,设置为事件模式,下降沿触发*/
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
    /*不配置可嵌入中断向量,只做唤醒事件,对应位没有挂起,所以无需清除事件位*/
}
/********************************************************************************************************
**函数信息 ：SysInit(void)                        
**功能描述 ：睡眠指令
**输入参数 ：无
**输出参数 ：无
**    备注 ：
********************************************************************************************************/
void sleep_test(void)
{
    GPIO_Configuration();
    __WFE();
}
