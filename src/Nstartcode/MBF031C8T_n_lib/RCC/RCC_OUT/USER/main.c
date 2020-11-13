#include "delay.h"
#include "sys.h"
#include "led.h"
/********************************************************************************************************
**函数信息 ：main(void)                     
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/

int main(void)    
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   //mco  pa8
    GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推免复用输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_PinAFConfig( GPIOA,GPIO_PinSource8,GPIO_AF_0);
    
    RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);  //通过PA8 pin 观察频率
    
    //PA8输出系统时钟输出/2
    while(1);
}



