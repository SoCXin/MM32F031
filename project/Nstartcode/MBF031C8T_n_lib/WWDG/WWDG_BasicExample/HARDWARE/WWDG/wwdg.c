/**
******************************************************************************
* @file    wwdg.c  
* @brief   ���ô��ڿ��Ź�
*          
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "HAL_device.h"
#include "HAL_conf.h"
/********************************************************************************************************
**������Ϣ ��void WWDG_IRQHandler (void)                   
**�������� �����ڿ��Ź��жϺ���
**������� ����
**������� ����
**    ��ע �����ݼ�����������0x40ʱ�����ж�
������������С��0x7f����0x40,���ӽ���0x40���ܻ�Ƶ��������ж�           
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
**������Ϣ ��Wwdg_reset_test()                     
**�������� ��ʹ�ô��ڿ��Ź�
**������� ��unsigned char ucTcn ���Ź��ݼ�������ֵ
unsigned char ucWcnt����ֵ
**������� ����
**    ��ע ��ϵͳ����ʱ���Ź�Ϊʧ��״̬,���Ź�ʧ�ܺ����ֶ��ر�,��λ���Ź�ʧ�� 
WWDG_SetCounter(0x7e);ι������
WWDG_Enable(0x7e);���Ź�ʹ�ܲ���ֵ������
���ڿ��Ź���λʱ��Ϊ΢�뵽���뼶,���ö�ʱ��ȴ���λ
WWDG counter clock = (PCLK1/4096)/x 
********************************************************************************************************/
void Wwdg_reset_ON(unsigned char ucTcnt,unsigned char ucWcnt)
{
    /*ʧ�ܴ��ڿ��Ź�ʱ��*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
    
    /*WWDG_Prescaler_x,xΪ1,2,4,8,WWDG counter clock = (PCLK1/4096)/x */
    WWDG_SetPrescaler(WWDG_Prescaler_8);
    WWDG_SetWindowValue(ucWcnt);
    
    /*Ϊ���Ź��Լ���������ֵ,Ϊ7λ��������ֵΪ0x40~0x7f֮��*/
    WWDG_Enable(ucTcnt);
}

/********************************************************************************************************
**������Ϣ ��void Wwdg_ExterslReloadReset_test(void)                   
**�������� ��������λ
**������� ����
**������� ����
**    ��ע �����Կ����˳�����ú����������λ,����Ҫһ����ʱ�ĸ�λ�ɿ��Ǵ˺���,
��Ͽ��Ź��ж�ʹ�ñ���ϵͳ������λ,�����ж�ʱ���������,���ѡ
�����κ�����������.
********************************************************************************************************/
void Wwdg_ExterslReloadReset_ON(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);                                                   //������
    WWDG_SetPrescaler(WWDG_Prescaler_8);                                                                   //������
    /*�˴�ָС������ֵ,�ҷ�������Χ,��С��0x80*/
    WWDG_SetWindowValue(0x70);	                                                                           //������
    /*�˴�ָ��������ֵ,�ҷ�������Χ,��С��0x7f����0x40*/
    WWDG_Enable(0x75);
}

/********************************************************************************************************
**������Ϣ ��Wwdg_irq_test()                      
**�������� �����ڿ��Ź��ж�����
**������� ����
**������� ����
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
    /*�˴�Ϊ����,����ֵ����С�ڴ���ֵ,��������������λ*/
    WWDG_Enable(0x66);		
}

