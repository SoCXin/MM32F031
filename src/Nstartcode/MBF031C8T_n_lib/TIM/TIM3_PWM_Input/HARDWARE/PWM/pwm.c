/**
******************************************************************************
* @file    wwdg_test.c  
* @brief   ���ö�ʱ��2,ʵ����·�ɵ�10KHz PWM����
*          PA6,PA7�ɹ۲��������
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "pwm.h" 
#include "uart.h"	  

u16 period = 0;
u16 duty  = 0;
u8 CollectFlag = 0;

void TIM2_PWM_Init(u16 arr,u16 psc)      //��T2��CH3   APB1
{  
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  //����GPIOBʱ��
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //TIM2_CH3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_2);  //PB10���ù��ܵ�AF2
    
    TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
    
    
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH3Ԥװ��ʹ��	 
    
    TIM_ARRPreloadConfig(TIM2, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
    
    TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2
    
    
}



/*�������ƣTTIM3_PWMINPUT_INIT(u16 arr,u16 psc)
����      PWM�����ʼ��*/
void TIM3_PWMINPUT_INIT(u16 arr,u16 psc)   //��T3��CH1 APB1  PA6    AF1
{
    
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ICInitTypeDef  TIM3_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);     //Open TIM3 clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //����GPIOAʱ��
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_1);  //PA6���ù��ܵ�AF1
    
    TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ  
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    
    
    /*�����ж����ȼ�*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;                     
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1;                   
    TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;       
    TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;   
    TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; 
    TIM3_ICInitStructure.TIM_ICFilter = 0x0; 
    
    TIM_PWMIConfig(TIM3, &TIM3_ICInitStructure);     //PWM��������           
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);     //ѡ����Ч�����       
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);  //����Ϊ���Ӹ�λģʽ
    
    TIM_ITConfig(TIM3, TIM_IT_CC1|TIM_IT_Update, ENABLE);          //�ж�����
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
    TIM_Cmd(TIM3, ENABLE);    
}

void TIM3_IRQHandler(void)
{
    
    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)//����1���������¼�
    {	
        period	=	TIM_GetCapture1(TIM3);     //�ɼ�����
        duty = TIM_GetCapture2(TIM3); 				//�ɼ�ռ�ձ�				  
        CollectFlag = 1;
    }		
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
}



