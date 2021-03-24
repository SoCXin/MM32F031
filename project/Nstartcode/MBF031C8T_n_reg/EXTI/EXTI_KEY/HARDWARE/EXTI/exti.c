#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "uart.h"
#include "sys.h"

extern void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	;	  
extern void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM); 
extern void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);
//外部中断初始化程序
void EXTIX_Init(void)
{
    KEY_Init();
    RCC->APB2ENR|=0x00000001;         //使能AFIO时钟	   
    
    Ex_NVIC_Config(GPIO_A,0,RTIR); 		//上升沿触发
    
    
    Ex_NVIC_Config(GPIO_C,13,FTIR);		//下降沿触发
    
    Ex_NVIC_Config(GPIO_B,10,FTIR);		//下降沿触发
    Ex_NVIC_Config(GPIO_B,11,FTIR);		//下降沿触发
    
    MY_NVIC_Init(2,2,EXTI0_1_IRQn,2);    	//抢占2，子优先级2，组2
    MY_NVIC_Init(2,0,EXTI4_15_IRQn,2);	//抢占2，子优先级0，组2	   
    
}

u16 EXTI_DELAY=0;
void EXTI0_1_IRQHandler(void)
{
    for(EXTI_DELAY=0;EXTI_DELAY<10000;EXTI_DELAY++);
    if(WK_UP==1)
    {	  
        LED2_TOGGLE();
    }
    EXTI->PR=1<<0;  //清除LINE0上的中断标志位  
    
}


void EXTI4_15_IRQHandler (void)
{			
    for(EXTI_DELAY=0;EXTI_DELAY<10000;EXTI_DELAY++);
    
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
    EXTI->PR=1<<10;  //清除LINE10上的中断标志位  
    EXTI->PR=1<<11;  //清除LINE11上的中断标志位  
    EXTI->PR=1<<13;  //清除LINE13上的中断标志位  
}


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


