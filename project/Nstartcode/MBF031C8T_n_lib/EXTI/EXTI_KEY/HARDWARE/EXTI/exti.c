#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������
//�ⲿ�ж� ��������			   
////////////////////////////////////////////////////////////////////////////////// 	  


//�ⲿ�жϳ�ʼ������
void EXTIX_Init(void)
{
    
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��
    
    KEY_Init();//��ʼ��������Ӧioģʽ
    
    //GPIOC.13 �ж����Լ��жϳ�ʼ������
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource13);
    
    EXTI_InitStructure.EXTI_Line=EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
    
    //GPIOA.0	  �ж����Լ��жϳ�ʼ������
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
    
    EXTI_InitStructure.EXTI_Line=EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
    
    //GPIOB.10	  �ж����Լ��жϳ�ʼ������
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource10);
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line10;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�����ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
    
    //GPIOB.11	  �ж����Լ��жϳ�ʼ������
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource11);
    
    EXTI_InitStructure.EXTI_Line=EXTI_Line11;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�����ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
    
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02;					//�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;					//�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure); 
    
}

u16 EXTI_DELAY=0;

void EXTI0_1_IRQHandler(void)
{
    for(EXTI_DELAY=0;EXTI_DELAY<1000;EXTI_DELAY++);
    if(WK_UP==1)
    {	  
        LED2_TOGGLE();
    }
    EXTI_ClearFlag(EXTI_Line0);  //���EXTI0��·����λ
}


void EXTI4_15_IRQHandler(void)
{			
    for(EXTI_DELAY=0;EXTI_DELAY<1000;EXTI_DELAY++);
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
    EXTI_ClearFlag(EXTI_Line10|EXTI_Line11|EXTI_Line13);   //���EXTI10��11��13��·����λ
    
}


