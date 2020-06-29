#include "wkup.h"
#include "led.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������
//�������� ����	   
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

//���WKUP�ŵ��ź�
//����ֵ1:��������
//      0:����Ĵ���	
u8 Check_WKUP(void) 
{
    u16 t=0;	//��¼���µ�ʱ��
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
            if(t>=1000)		//���³���2����
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
    EXTI->PR = EXTI_PR_PR0;
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

/********************************************************************************************************
**������Ϣ ��WKUP_Init(void)                         
**�������� ���ⲿ�жϻ��Ѵ�����ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void WKUP_Init(void)
{			  
    RCC->AHBENR|=0x7<<17;     //��ʹ������IO PORTAʱ��    
    RCC->APB2ENR|=0x1;
    
    GPIOA->CRL&=0XFFFFFFF0;//PA0���ó�����	  
    GPIOA->CRL|=GPIO_CRL_CNF0_1;  
    //ʹ���ⲿ�жϷ�ʽ
    Ex_NVIC_Config(GPIO_A,0,RTIR); 		//�����ش���
    
    MY_NVIC_Init(2,2,EXTI0_1_IRQn,2);
    if(Check_WKUP()==0) Sys_Standby();    //���ǿ���,�������ģʽ  
}















