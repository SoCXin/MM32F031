#include "uart.h"
#include "sys.h"
#include "delay.h"
#include "i2c.h"

int main(void)
{
    
    
    delay_init();
    uart_initwBaudRate(115200);
    printf("uart ok\r\n");
    I2CMasterTest();             //I2C收发数据
    while(1)                    //无限循环
    {
        
    }
}
