#include "wkup.h"
#include "led.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//开发板
//待机唤醒 代码	   
//////////////////////////////////////////////////////////////////////////////////

void Sys_Standby(void)
{  
    RCC->APB1ENR|=RCC_APB1ENR_PWREN;  
    
    PWR->CSR |= 0x00000100; 
    
    /* Clear Wake-up flag */
    PWR->CR |= 0x4;
    /* Select STANDBY mode */
    PWR->CR |= 0x2;
    /* Set SLEEPDEEP bit of Cortex System Control Register */
    
    SCB->SCR |= 0x4;
    /* Request Wait For Interrupt */
    __WFI();
}

//检测WKUP脚的信号
//返回值1:连续按下
//      0:错误的触发	
u8 Check_WKUP(void) 
{
    u16 t=0;	//记录按下的时间
    LED1_ON();
    LED2_ON();
    LED3_ON();
    LED4_ON();
    
    while(1)
    {
        if((GPIOA->IDR&0x1)==1)
        {
            t++;
            delay_ms(2);      
            if(t>=1000)		//按下超过2秒钟
            {
                LED1_ON();
                LED2_ON();
                LED3_ON();
                LED4_ON();
                return 1; 	//按下3s以上了
            }
        }
        else 
        { 
            LED1_OFF();
            LED2_OFF();
            LED3_OFF();
            LED4_OFF();
            return 0; //按下不足3秒
        }
        
    }
    
}  


/********************************************************************************************************
**函数信息 ：EXTI0_IRQHandler(void)                         
**功能描述 ：外部中断0，进入后清除中断标志位
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void EXTI0_1_IRQHandler(void)
{ 		    		    				     		     
    EXTI->PR = EXTI_PR_PR0;
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

/********************************************************************************************************
**函数信息 ：WKUP_Init(void)                         
**功能描述 ：外部中断唤醒待机初始化
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void WKUP_Init(void)
{			  
    RCC->AHBENR|=0x7<<17;     //先使能外设IO PORTA时钟    
    RCC->APB2ENR|=0x1;
    
    GPIOA->CRL&=0XFFFFFFF0;//PA0设置成输入	  
    GPIOA->CRL|=GPIO_CRL_CNF0_1;  
    //使用外部中断方式
    Ex_NVIC_Config(GPIO_A,0,RTIR); 		//上升沿触发
    
    MY_NVIC_Init(2,2,EXTI0_1_IRQn,2);
    if(Check_WKUP()==0) Sys_Standby();    //不是开机,进入待机模式  
}















