/**--------------File Info---------------------------------------------------------------------------------
** File name:		  	sleep.c
** Descriptions:		    main ��������
**
*********************************************************************************************************/
#include "sleep.h"
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


void GPIO_Configuration(void)
{
    RCC->AHBENR|=0xf<<17;
    RCC->APB2ENR|=0x00000001;
    
    /*����PA0λ��������*/
    GPIOA->CRL=0x8;
    
    /*ѡ��PA0λ�ⲿ�¼���������*/
    Ex_NVIC_Config(GPIO_A,0,RTIR); 
    //�����ش���
    /*ʹ���ⲿ�ж�0,����Ϊ�¼�ģʽ,�½��ش���*/
    EXTI->EMR=0x01;
    EXTI->RTSR=0x01;
    EXTI->PR=0x01;
    
    /*�����ÿ�Ƕ���ж�����,ֻ�������¼�,��Ӧλû�й���,������������¼�λ*/
}
/********************************************************************************************************
**������Ϣ ��SysInit(void)                        
**�������� ��˯��ָ��
**������� ����
**������� ����
**    ��ע ��
********************************************************************************************************/
void sleep_test(void)
{
    GPIO_Configuration();
    __WFE();
}
