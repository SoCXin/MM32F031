#include "wkup.h"
#include "led.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������
//�������� ����	   
//////////////////////////////////////////////////////////////////////////////////

void Sys_Standby(void)
{  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR����ʱ��
    //	RCC->APB2RSTR|=0X01FC;//��λ����IO��
    PWR_WakeUpPinCmd(ENABLE);  //ʹ�ܻ��ѹܽŹ���
    PWR_EnterSTANDBYMode();	  //���������STANDBY��ģʽ 
    
}

//���WKUP�ŵ��ź�
//����ֵ1:��������
//      0:����Ĵ���	
u8 Check_WKUP(void) 
{
    u8 t=0;	//��¼���µ�ʱ��
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
            if(t>=100)		//���³���2����
            {
                LED1_ON();
                LED2_ON();
                LED3_ON();
                LED4_ON();
                return 1; 	//����3s������
            }
            
        }
        else 
        { 
            LED1_OFF();
            LED2_OFF();
            LED3_OFF();
            LED4_OFF();
            return 0; //���²���3��
        }
        
    }
    
}  


/********************************************************************************************************
**������Ϣ ��EXTI0_IRQHandler(void)                         
**�������� ���ⲿ�ж�0�����������жϱ�־λ
**������� ����
**������� ����
********************************************************************************************************/
void EXTI0_1_IRQHandler(void)
{ 		    		    				     		    
    EXTI_ClearITPendingBit(EXTI_Line0); // ���LINE10�ϵ��жϱ�־λ		  
} 

/********************************************************************************************************
**������Ϣ ��WKUP_Init(void)                         
**�������� ���ⲿ�жϻ��Ѵ�����ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void WKUP_Init(void)
{	
    GPIO_InitTypeDef GPIO_InitStructure;  		  
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��GPIOA�͸��ù���ʱ��
    
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;	 //PA.0
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��IO
    //ʹ���ⲿ�жϷ�ʽ
    SYSCFG_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	//�ж���0����GPIOA.0
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//���ð������е��ⲿ��·
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�����ⲿ�ж�ģʽ:EXTI��·Ϊ�ж�����
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //�����ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	// ��ʼ���ⲿ�ж�
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn; //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2; //�����ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
    
    if(Check_WKUP()==0) Sys_Standby();    //���ǿ���,�������ģʽ  
}
















