#include "adc.h"

/********************************************************************************************************
**������Ϣ ��void ADC1_SingleChannel(uint8_t ADC_Channel_x)      
**�������� ������ADC1����ת��ģʽ
**������� ��ADC_Channel_x , xΪ0~8
**������� ����
**    ��ע ��
********************************************************************************************************/
void ADC1_SingleChannel(void)
{
    RCC->AHBENR |=1<<17;//ʹ��GPIOAʱ��
    RCC->APB2ENR |=RCC_APB2ENR_ADC1EN;//ʹ��ADC1ʱ��
    GPIOA->CRL&=0XFFFFFF0F;//PA1 anolog����
    
    RCC->APB2RSTR|=RCC_APB2RSTR_ADC1RST;   //ADC1��λ
    RCC->APB2RSTR&=~(RCC_APB2RSTR_ADC1RST);//��λ����	   
    
    /*��ADC���óɵ����������ת��ģʽ*/
    ADC1->ADCFG |= ADCFG_ADCPRE_8|ADCFG_ADCPRE_10;//8��Ƶ
    ADC1->ADCR &= ~(ADCR_ADMD_PERIOD|ADCR_ADMD_CONTINUE|ADCR_ALIGN_LEFT);//���η�����ģʽ,�����Ҷ���
    ADC1->ADCR |=ADCR_ADMD_PERIOD;
    ADC1->ADCHS =ADCHS_CHEN1;//ʹ��ͨ��1
    
    ADC1->ADCFG |= ADCFG_ADEN;//ADC1ʹ��
    ADC1->ADCR |= ADCR_ADST;//��ʼת��
}
/********************************************************************************************************
**������Ϣ ��ADC1_SingleChannel_Get()       
**�������� ����ȡADC1ת������
**������� ��ADC_Channel_x , xΪ0~8
*puiADData ,ADC1ʵ��ת������
**������� ��ucStatus ,0 ��ʾ���ݻ�ȡʧ��,1 ��ʾ�ɹ�
********************************************************************************************************/
u16 ADC1_SingleChannel_Get(void)
{ 
    u16 puiADData;
    /*ADCR�Ĵ�����ADSTλʹ�ܣ��������ת��*/
    ADC1->ADCR |=0x00000100;
    while(((ADC1->ADSTA )&0x01)==0);
    
    ADC1->ADSTA |= 0x01;
    puiADData=ADC1->ADDATA&0xfff;
    return puiADData;
}

