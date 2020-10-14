#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "wkup.h"
#include "led.h"

int main(void)
{ 
    
    delay_init();
    LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
    uart_initwBaudRate(115200);	 	//���ڳ�ʼ��Ϊ115200
    printf("�밴��wakeup����3�벻��\r\n");
    WKUP_Init();			//��ʼ��WK_UP������ͬʱ����Ƿ���������
    
    while(1)
    {
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        delay_ms(250);//��ʱ250ms
        printf("�Ѵ�standbyģʽ����\r\n");
    }
    
}

