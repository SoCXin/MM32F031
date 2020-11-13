#include "delay.h"
#include "sys.h"
#include "led.h"
/********************************************************************************************************
**������Ϣ ��main(void)                     
**�������� ��
**������� ����
**������� ����
********************************************************************************************************/

int main(void)    
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   //mco  pa8
    GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ���⸴�����
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_PinAFConfig( GPIOA,GPIO_PinSource8,GPIO_AF_0);
    
    RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);  //ͨ��PA8 pin �۲�Ƶ��
    
    //PA8���ϵͳʱ�����/2
    while(1);
}



