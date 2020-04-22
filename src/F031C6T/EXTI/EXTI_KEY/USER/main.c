/**
******************************************************************************
* @file     main.c
* @author   AE team
* @version  V1.1.1
* @date     15/05/2019
* @brief
******************************************************************************
*/

#include "HAL_device.h"
#include "HAL_conf.h"
#include "stdio.h"

void LED_Init(void);
void KEY_Init(void);
void EXTIX_Init(void);

#define LED1_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)	// PA15
#define LED1_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15)	    // PA15
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15)) // PA15

#define LED2_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_3)	    // PB3
#define LED2_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_3)	    // PB3
#define LED2_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3))?(GPIO_ResetBits(GPIOB,GPIO_Pin_3)):(GPIO_SetBits(GPIOB,GPIO_Pin_3))	// PB3

#define LED3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_4)	    // PB4
#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)	    // PB4
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?(GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))	// PB4

#define LED4_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)	    // PB5
#define LED4_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)	    // PB5
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?(GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))	// PB5


#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	    //读取按键2 

#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)		//读取按键1
#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)		//读取按键1
#define KEY3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)	    //读取按键3
#define KEY4  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)	    //读取按键4

#define KEY1_PRES	1		//KEY1 
#define KEY2_PRES	2		  
#define KEY3_PRES	3		//KEY3 
#define KEY4_PRES	4		//KEY4 

/********************************************************************************************************
**函数信息 ：main(void)
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
int main(void)
{
    //GPIO口测试，闪烁LED灯

    LED_Init();                                                                 //初始化与LED连接的硬件接口
    KEY_Init();                                                                 //初始化与按键连接的硬件接口
    EXTIX_Init();

    while(1) 
    {
        if(WK_UP == 0) 
        {
            LED1_ON();
        }
        else
        {
            LED1_OFF();
        }
        
    }
}

/********************************************************************************************************
**函数信息 ：LED_Init(void)
**功能描述 ：LED初始化
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void LED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);   //开启GPIOA,GPIOB时钟

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    LED1_ON();
    LED2_ON();
    LED3_ON();
    LED4_ON();
}

/********************************************************************************************************
**函数信息 ：void KEY_Init(void)
**功能描述 ：按键初始化函数
**输入参数 ：
**输出参数 ：
**    备注 ：
********************************************************************************************************/
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA |RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC, ENABLE); //使能GPIOA,GPIOB,GPIOC时钟

//    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;                                 //PC13，K1
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //设置成上拉输入
//    GPIO_Init(GPIOC, &GPIO_InitStructure);                                      //初始化GPIOC13

   GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;                                  //PA0,K2（WK_UP）
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;                               //设置成下拉输入
   GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //初始化GPIOA0

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_10 | GPIO_Pin_11;        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //设置成上拉输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);                                      //初始化GPIOB.10,11
}

/********************************************************************************************************
**函数信息 ：void EXTIX_Init(void)
**功能描述 ：外部中断初始化函数
**输入参数 ：
**输出参数 ：
**常用函数 ：
********************************************************************************************************/
void EXTIX_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);                      //外部中断，需要使能AFIO时钟

    KEY_Init();                                                                 //初始化按键对应io模式

    //GPIOC.13 中断线以及中断初始化配置
//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);

//    EXTI_InitStructure.EXTI_Line = EXTI_Line13;
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                     //下降沿触发
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//    EXTI_Init(&EXTI_InitStructure);                                             //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

//    //GPIOA.0	  中断线以及中断初始化配置
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;                      //上升沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);                                             //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		
    //GPIOB.1	  中断线以及中断初始化配置
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);

    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;                  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);                                             //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;                          //使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;                          //子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                             //使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);
		
    //GPIOB.2	  中断线以及中断初始化配置
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;               
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);                                             //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_3_IRQn;                          //使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;                          //子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                             //使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);

    //GPIOB.10	  中断线以及中断初始化配置
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource10);
    EXTI_InitStructure.EXTI_Line = EXTI_Line10;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;                   
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);                                             //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
    //GPIOB.11	  中断线以及中断初始化配置
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource11);
    EXTI_InitStructure.EXTI_Line = EXTI_Line11;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;                   
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);                                             //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;                         //使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;                          //子优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                             //使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);  	                                        //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器


}

/********************************************************************************************************
**函数信息 ：void EXTI0_1_IRQHandler(void)
**功能描述 ：外部中断函数
**输入参数 ：
**输出参数 ：
**常用函数 ：
********************************************************************************************************/
u16 EXTI_DELAY = 0;

void EXTI0_1_IRQHandler(void)
{
    if(EXTI_GetFlagStatus(EXTI_Line0) != RESET) 
    {
        for(EXTI_DELAY = 0; EXTI_DELAY < 100; EXTI_DELAY++);
        if(KEY1 == 0) 
        {
            LED1_TOGGLE();
        }
        EXTI_ClearFlag(EXTI_Line0);   
    } 
    if(EXTI_GetFlagStatus(EXTI_Line1) != RESET) 
    {
        for(EXTI_DELAY = 0; EXTI_DELAY < 100; EXTI_DELAY++);
        if(KEY1 == 1) 
        {
            LED1_TOGGLE();
        }
        EXTI_ClearFlag(EXTI_Line1);   
    }                                      
}

/********************************************************************************************************
**函数信息 ：void EXTI4_15_IRQHandler(void)
**功能描述 ：外部中断函数
**输入参数 ：
**输出参数 ：
**常用函数 ：
********************************************************************************************************/
void EXTI2_3_IRQHandler(void)
{
    if(EXTI_GetFlagStatus(EXTI_Line2) != RESET) 
    {
        for(EXTI_DELAY = 0; EXTI_DELAY < 100; EXTI_DELAY++);
        if(KEY2 == 1) 
        {
            LED2_TOGGLE();
        }
        EXTI_ClearFlag(EXTI_Line2);      
    }                                         
}

/********************************************************************************************************
**函数信息 ：void EXTI4_15_IRQHandler(void)
**功能描述 ：外部中断函数
**输入参数 ：
**输出参数 ：
**常用函数 ：
********************************************************************************************************/
void EXTI4_15_IRQHandler(void)
{
    if(EXTI_GetFlagStatus(EXTI_Line10) != RESET) 
    {
        for(EXTI_DELAY = 0; EXTI_DELAY < 100; EXTI_DELAY++);
        if(KEY3 == 1) 
        {
            LED3_TOGGLE();
        }
        EXTI_ClearFlag(EXTI_Line10);   
    } 
    if(EXTI_GetFlagStatus(EXTI_Line11) != RESET) 
    {
        for(EXTI_DELAY = 0; EXTI_DELAY < 100; EXTI_DELAY++);
        if(KEY4 == 1) 
        {
            LED4_TOGGLE();
        }
        EXTI_ClearFlag(EXTI_Line11);                    //清除EXTI10，11，13线路挂起位
    }
}

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/*-------------------------(C) COPYRIGHT 2019 MindMotion ----------------------*/

