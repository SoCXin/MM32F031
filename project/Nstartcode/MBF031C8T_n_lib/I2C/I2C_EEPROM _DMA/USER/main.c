#include "uart.h"
#include "sys.h"
#include "delay.h"
#include "i2c.h"

int main(void)
{
    
    delay_init();//启动外部晶振
    uart_initwBaudRate(115200);
    printf("uart ok\r\n");
    I2C1MasterDMATest();//I2C收发数据
    
    while(1)              //无限循环
    {
        
    }
}

