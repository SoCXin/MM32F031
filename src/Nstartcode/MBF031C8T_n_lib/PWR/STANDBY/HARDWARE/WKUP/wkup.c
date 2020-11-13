#include "wkup.h"
#include "led.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//开发板
//待机唤醒 代码	   
//////////////////////////////////////////////////////////////////////////////////

void Sys_Standby(void)
{  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//使能PWR外设时钟
    //	RCC->APB2RSTR|=0X01FC;//复位所有IO口
    PWR_WakeUpPinCmd(ENABLE);  //使能唤醒管脚功能
    PWR_EnterSTANDBYMode();	  //进入待命（STANDBY）模式 
    
}

//检测WKUP脚的信号
//返回值1:连续按下
//      0:错误的触发	
u8 Check_WKUP(void) 
{
    u8 t=0;	//记录按下的时间
    LED1_ON();
    LED2_ON();
    LED3_ON();
    LED4_ON();
    
    while(1)
    {
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
        {
            t++;
            delay_ms(20);      
            if(t>=100)		//按下超过2秒钟
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
    EXTI_ClearITPendingBit(EXTI_Line0); // 清除LINE10上的中断标志位		  
} 

/********************************************************************************************************
**函数信息 ：WKUP_Init(void)                         
**功能描述 ：外部中断唤醒待机初始化
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void WKUP_Init(void)
{	
    GPIO_InitTypeDef GPIO_InitStructure;  		  
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能GPIOA和复用功能时钟
    
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;	 //PA.0
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;//上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化IO
    //使用外部中断方式
    SYSCFG_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	//中断线0连接GPIOA.0
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//设置按键所有的外部线路
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//设外外部中断模式:EXTI线路为中断请求
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //上升沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	// 初始化外部中断
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn; //使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2; //从优先级2级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
    NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
    
    if(Check_WKUP()==0) Sys_Standby();    //不是开机,进入待机模式  
}
















