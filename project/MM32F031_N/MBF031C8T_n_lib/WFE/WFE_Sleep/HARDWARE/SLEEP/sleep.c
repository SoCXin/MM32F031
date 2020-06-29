/**--------------File Info---------------------------------------------------------------------------------
** File name:		  	sleep.c
** Descriptions:		    main ��������
**
*********************************************************************************************************/
#include "sleep.h"
#include "HAL_conf.h"
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //����GPIOAʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    /*����PA0λ��������*/
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init( GPIOA, &GPIO_InitStruct);
    
    /*ѡ��PA0λ�ⲿ�¼���������*/
    SYSCFG_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    
    /*ʹ���ⲿ�ж�0,����Ϊ�¼�ģʽ,�½��ش���*/
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
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
