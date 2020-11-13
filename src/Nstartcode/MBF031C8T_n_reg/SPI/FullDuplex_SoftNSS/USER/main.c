#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "spi.h"

int main(void)
{
    
    delay_init();
    uart_initwBaudRate(115200);	 		//串口初始化为115200
    SPIM_Test();		
    while(1)              		//无限循环
    {
        
    }
}


