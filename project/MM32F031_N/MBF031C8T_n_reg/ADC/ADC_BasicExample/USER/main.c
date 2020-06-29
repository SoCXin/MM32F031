#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "adc.h"

/********************************************************************************************************
**函数信息 ：main(void)
**功能描述 ：
**输入参数 ：
**输出参数 ：
**    备注 ：
********************************************************************************************************/
int main(void)
{
    u16 ADCVAL;
    float fValue;
    
    delay_init();
    uart_initwBaudRate(115200);	 //串口初始化为115200
    
    /*配置ADC1为单次转换模式,通道1使能*/
    ADC1_SingleChannel();
    while(1)
    {
        /*读取当前转换数据*/
        ADCVAL=ADC1_SingleChannel_Get();  
        fValue = ((float)ADCVAL/4095)*3.3;
        printf("ADC1_CH_1=%fV\r\n",fValue);
        delay_ms(500);  
    }	
}
