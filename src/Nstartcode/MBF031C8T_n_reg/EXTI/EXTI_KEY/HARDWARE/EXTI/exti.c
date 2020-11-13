#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "uart.h"
#include "sys.h"

extern void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	;	  
extern void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM); 
extern void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);
//�ⲿ�жϳ�ʼ������
void EXTIX_Init(void)
{
    KEY_Init();
    RCC->APB2ENR|=0x00000001;         //ʹ��AFIOʱ��	   
    
    Ex_NVIC_Config(GPIO_A,0,RTIR); 		//�����ش���
    
    
    Ex_NVIC_Config(GPIO_C,13,FTIR);		//�½��ش���
    
    Ex_NVIC_Config(GPIO_B,10,FTIR);		//�½��ش���
    Ex_NVIC_Config(GPIO_B,11,FTIR);		//�½��ش���
    
    MY_NVIC_Init(2,2,EXTI0_1_IRQn,2);    	//��ռ2�������ȼ�2����2
    MY_NVIC_Init(2,0,EXTI4_15_IRQn,2);	//��ռ2�������ȼ�0����2	   
    
}

u16 EXTI_DELAY=0;
void EXTI0_1_IRQHandler(void)
{
    for(EXTI_DELAY=0;EXTI_DELAY<10000;EXTI_DELAY++);
    if(WK_UP==1)
    {	  
        LED2_TOGGLE();
    }
    EXTI->PR=1<<0;  //���LINE0�ϵ��жϱ�־λ  
    
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
    EXTI->PR=1<<10;  //���LINE10�ϵ��жϱ�־λ  
    EXTI->PR=1<<11;  //���LINE11�ϵ��жϱ�־λ  
    EXTI->PR=1<<13;  //���LINE13�ϵ��жϱ�־λ  
}


void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM) 
{
    u8 EXTADDR;
    u8 EXTOFFSET;
    EXTADDR=BITx/4;//�õ��жϼĴ�����ı��
    EXTOFFSET=(BITx%4)*4; 
    RCC->APB2ENR|=0x01;//ʹ��io����ʱ��			 
    SYSCFG->EXTICR[EXTADDR]&=~(0x000F<<EXTOFFSET);//���ԭ�����ã�����
    SYSCFG->EXTICR[EXTADDR]|=GPIOx<<EXTOFFSET;//EXTI.BITxӳ�䵽GPIOx.BITx 
    //�Զ�����
    EXTI->IMR|=1<<BITx;//  ����line BITx�ϵ��ж�
    //EXTI->EMR|=1<<BITx;//������line BITx�ϵ��¼� (������������,��Ӳ�����ǿ��Ե�,��������������ʱ���޷������ж�!)
    if(TRIM&0x01)EXTI->FTSR|=1<<BITx;//line BITx���¼��½��ش���
    if(TRIM&0x02)EXTI->RTSR|=1<<BITx;//line BITx���¼��������ش���
} 	  


