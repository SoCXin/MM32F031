#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "spi.h"

int main(void)
{
    
    delay_init();
    uart_initwBaudRate(115200);	 		//���ڳ�ʼ��Ϊ115200
    SPIM_Test();		
    while(1)              		//����ѭ��
    {
        
    }
}


