/**
******************************************************************************
* @file    wwdg.c  
* @brief   配置窗口看门狗
*          
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "HAL_device.h"
#include "HAL_conf.h"
/********************************************************************************************************
**函数信息 ：void WWDG_IRQHandler (void)                   
**功能描述 ：窗口看门狗中断函数
**输入参数 ：无
**输出参数 ：无
**    备注 ：当递减计数器等于0x40时进入中断
计数器参数须小于0x7f大于0x40,若接近于0x40可能会频繁进入此中断           
********************************************************************************************************/
void WWDG_IRQHandler (void)
{	
    if(WWDG_GetFlagStatus())
    {
        WWDG_ClearFlag();
        WWDG_SetCounter(0x66);                                                                               
    }
}

/********************************************************************************************************
**函数信息 ：Wwdg_reset_test()                     
**功能描述 ：使用窗口看门狗
**输入参数 ：unsigned char ucTcn 看门狗递减计数器值
unsigned char ucWcnt窗口值
**输出参数 ：无
**    备注 ：系统启动时看门狗为失能状态,看门狗失能后不能手动关闭,复位后看门狗失能 
WWDG_SetCounter(0x7e);喂狗函数
WWDG_Enable(0x7e);看门狗使能并赋值计数器
窗口看门狗复位时间为微秒到毫秒级,适用短时间等待复位
WWDG counter clock = (PCLK1/4096)/x 
********************************************************************************************************/
void Wwdg_reset_ON(unsigned char ucTcnt,unsigned char ucWcnt)
{
    /*失能窗口看门狗时钟*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
    
    /*WWDG_Prescaler_x,x为1,2,4,8,WWDG counter clock = (PCLK1/4096)/x */
    WWDG_SetPrescaler(WWDG_Prescaler_8);
    WWDG_SetWindowValue(ucWcnt);
    
    /*为看门狗自减计数器赋值,为7位数据且数值为0x40~0x7f之间*/
    WWDG_Enable(ucTcnt);
}

/********************************************************************************************************
**函数信息 ：void Wwdg_ExterslReloadReset_test(void)                   
**功能描述 ：立即复位
**输入参数 ：无
**输出参数 ：无
**    备注 ：可以看到此程序调用后程序立即复位,若需要一个即时的复位可考虑此函数,
配合看门狗中断使用避免系统持续复位,若打开中断时已完成设置,则可选
择屏蔽函数内配置项.
********************************************************************************************************/
void Wwdg_ExterslReloadReset_ON(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);                                                   //配置项
    WWDG_SetPrescaler(WWDG_Prescaler_8);                                                                   //配置项
    /*此处指小于下面值,且符合自身范围,即小于0x80*/
    WWDG_SetWindowValue(0x70);	                                                                           //配置项
    /*此处指大于上面值,且符合自身范围,即小于0x7f大于0x40*/
    WWDG_Enable(0x75);
}

/********************************************************************************************************
**函数信息 ：Wwdg_irq_test()                      
**功能描述 ：窗口看门狗中断配置
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void Wwdg_irq_ON(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
    WWDG_SetWindowValue(0x70);
    WWDG_EnableIT();
    /*此处为配置,参数值必须小于窗口值,否则立即产生复位*/
    WWDG_Enable(0x66);		
}

