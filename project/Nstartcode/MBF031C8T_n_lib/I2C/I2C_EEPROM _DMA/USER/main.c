#include "uart.h"
#include "sys.h"
#include "delay.h"
#include "i2c.h"

int main(void)
{
    
    delay_init();//�����ⲿ����
    uart_initwBaudRate(115200);
    printf("uart ok\r\n");
    I2C1MasterDMATest();//I2C�շ�����
    
    while(1)              //����ѭ��
    {
        
    }
}

