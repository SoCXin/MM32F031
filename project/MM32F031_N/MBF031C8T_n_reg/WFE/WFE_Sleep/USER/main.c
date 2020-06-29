#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "led.h"
#include "sleep.h"

int main(void)
{
    //�벻Ҫ������ʹ��systick��ʽ��ʱ,systick�ж�Ҳ�ỽ��wfe
    
    uart_initwBaudRate(115200);	 //���ڳ�ʼ��Ϊ115200
    LED_Init();
    printf("sleep mode ! \r\n");
    sleep_test();   
    
    //����K2��������
    while(1)//����ѭ��
    {
        printf("Wake Up ! \r\n");
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        delay_ms(1000);
    }	
}



