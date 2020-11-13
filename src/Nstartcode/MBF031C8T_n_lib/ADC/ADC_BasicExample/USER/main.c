#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "adc.h"
#include "led.h"

/********************************************************************************************************
**������Ϣ ��main(void)
**�������� ��
**������� ��
**������� ��
**    ��ע ��
********************************************************************************************************/
int main(void)
{
    u16 ADCVAL;
    float fValue;
    delay_init();
    LED_Init();
    uart_initwBaudRate(115200);	 //���ڳ�ʼ��Ϊ115200
    
    /*����ADC1Ϊ����ת��ģʽ,ͨ��1ʹ��*/
    ADC1_SingleChannel( ADC_Channel_1);
    while(1)
    {
        ADCVAL=Get_Adc_Average(ADC_Channel_1,5);
        fValue = ((float)ADCVAL/4095)*3.3;
        printf("ADC1_CH_1=%fV\r\n",fValue);
        delay_ms(200);  
    }
    
}
