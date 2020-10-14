#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "adc.h"

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
    uart_initwBaudRate(115200);	 //���ڳ�ʼ��Ϊ115200
    
    /*����ADC1Ϊ����ת��ģʽ,ͨ��1ʹ��*/
    ADC1_SingleChannel();
    while(1)
    {
        /*��ȡ��ǰת������*/
        ADCVAL=ADC1_SingleChannel_Get();  
        fValue = ((float)ADCVAL/4095)*3.3;
        printf("ADC1_CH_1=%fV\r\n",fValue);
        delay_ms(500);  
    }	
}
