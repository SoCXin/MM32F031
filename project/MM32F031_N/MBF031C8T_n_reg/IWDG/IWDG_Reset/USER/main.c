#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "iwdg.h"

int main(void)
{
    
    delay_init();
    uart_initwBaudRate(115200);	 //串口初始化为115200
    
    printf("uart ok!\r\n");
    /*配置为LSI32分频,计数器初始值为0x7ff,复位时长约为1.6s*/
    Write_Iwdg_ON(0x03,0xf);
    while(1)//无限循环
    {
        //无复位程序进入死循环,反正系统一直打印串口数据
        //		Write_Iwdg_RL();     
    }	
    
    
}



