#include "adc.h"

/********************************************************************************************************
**函数信息 ：void ADC1_SingleChannel(uint8_t ADC_Channel_x)      
**功能描述 ：配置ADC1单次转换模式
**输入参数 ：ADC_Channel_x , x为0~8
**输出参数 ：无
**    备注 ：
********************************************************************************************************/
void ADC1_SingleChannel(void)
{
    RCC->AHBENR |=1<<17;//使能GPIOA时钟
    RCC->APB2ENR |=RCC_APB2ENR_ADC1EN;//使能ADC1时钟
    GPIOA->CRL&=0XFFFFFF0F;//PA1 anolog输入
    
    RCC->APB2RSTR|=RCC_APB2RSTR_ADC1RST;   //ADC1复位
    RCC->APB2RSTR&=~(RCC_APB2RSTR_ADC1RST);//复位结束	   
    
    /*将ADC配置成单次软件触发转换模式*/
    ADC1->ADCFG |= ADCFG_ADCPRE_8|ADCFG_ADCPRE_10;//8分频
    ADC1->ADCR &= ~(ADCR_ADMD_PERIOD|ADCR_ADMD_CONTINUE|ADCR_ALIGN_LEFT);//单次非周期模式,数据右对齐
    ADC1->ADCR |=ADCR_ADMD_PERIOD;
    ADC1->ADCHS =ADCHS_CHEN1;//使能通道1
    
    ADC1->ADCFG |= ADCFG_ADEN;//ADC1使能
    ADC1->ADCR |= ADCR_ADST;//开始转换
}
/********************************************************************************************************
**函数信息 ：ADC1_SingleChannel_Get()       
**功能描述 ：获取ADC1转换数据
**输入参数 ：ADC_Channel_x , x为0~8
*puiADData ,ADC1实际转换数据
**输出参数 ：ucStatus ,0 表示数据获取失败,1 表示成功
********************************************************************************************************/
u16 ADC1_SingleChannel_Get(void)
{ 
    u16 puiADData;
    /*ADCR寄存器的ADST位使能，软件启动转换*/
    ADC1->ADCR |=0x00000100;
    while(((ADC1->ADSTA )&0x01)==0);
    
    ADC1->ADSTA |= 0x01;
    puiADData=ADC1->ADDATA&0xfff;
    return puiADData;
}

