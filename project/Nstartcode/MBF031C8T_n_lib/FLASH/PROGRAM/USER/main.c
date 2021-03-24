#include "delay.h"
#include "sys.h"
#include "led.h"
#include "uart.h"
#include "flash.h"

/********************************************************************************************************
**������Ϣ ��main(void)                     
**�������� ��
**������� ����
**������� ����
********************************************************************************************************/
int main(void)    
{ 
    //���س���ǰҪѡ��ȫ����
    u8 t; 
    delay_init();
    LED_Init();
    uart_initwBaudRate(115200);
    t=FLASH_Program();
    
    if(t==0)
        while(1)              //����ѭ��
        {
            printf("program ok\r\n");
            LED1_TOGGLE();
            LED2_TOGGLE();
            LED3_TOGGLE();
            LED4_TOGGLE();
            delay_ms(1000);
        }
    else
        while(1)
        {
            printf("program error\r\n");
            delay_ms(1000);
        }
}



