#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "dma.h"
#include "led.h"
/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� ��������ARMLED����
**������� ��
**������� ��
********************************************************************************************************/


int main(void)
{
    //DMA�Ĳ���
    
    delay_init();
    uart_initwBaudRate(115200); //UART1�ķ��ͣ�����ͨ�����������ӡUART OK
    LED_Init();
    DMA_m8tom8_test();
    // 	DMA_m8tom16_test();
    // 	DMA_m8tom32_test();
    //  DMA_m16tom8_test();
    //  DMA_m16tom16_test();
    //	DMA_m16tom32_test();
    
    
    while(1)              //����ѭ��
    {
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        delay_ms(1000);
    }
}

