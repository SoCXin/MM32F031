/**--------------File Info---------------------------------------------------------------------------------
** File name:		  	sleep.c
** Descriptions:		    main 函数调用
**
*********************************************************************************************************/
#include "sleep.h"
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM) 
{
    u8 EXTADDR;
    u8 EXTOFFSET;
    EXTADDR=BITx/4;//得到中断寄存器组的编号
    EXTOFFSET=(BITx%4)*4; 
    RCC->APB2ENR|=0x01;//使能io复用时钟			 
    SYSCFG->EXTICR[EXTADDR]&=~(0x000F<<EXTOFFSET);//清除原来设置！！！
    SYSCFG->EXTICR[EXTADDR]|=GPIOx<<EXTOFFSET;//EXTI.BITx映射到GPIOx.BITx 
    //自动设置
    EXTI->IMR|=1<<BITx;//  开启line BITx上的中断
    //EXTI->EMR|=1<<BITx;//不屏蔽line BITx上的事件 (如果不屏蔽这句,在硬件上是可以的,但是在软件仿真的时候无法进入中断!)
    if(TRIM&0x01)EXTI->FTSR|=1<<BITx;//line BITx上事件下降沿触发
    if(TRIM&0x02)EXTI->RTSR|=1<<BITx;//line BITx上事件上升降沿触发
} 	  


void GPIO_Configuration(void)
{
    RCC->AHBENR|=0xf<<17;
    RCC->APB2ENR|=0x00000001;
    
    /*配置PA0位下拉输入*/
    GPIOA->CRL=0x8;
    
    /*选择PA0位外部事件触发输入*/
    Ex_NVIC_Config(GPIO_A,0,RTIR); 
    //上升沿触发
    /*使能外部中断0,设置为事件模式,下降沿触发*/
    EXTI->EMR=0x01;
    EXTI->RTSR=0x01;
    EXTI->PR=0x01;
    
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
