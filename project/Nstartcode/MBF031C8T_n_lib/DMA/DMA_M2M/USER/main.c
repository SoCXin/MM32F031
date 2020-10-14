#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "dma.h"
#include "led.h"
/********************************************************************************************************
**函数信息 ：int main (void)                          
**功能描述 ：开机后，ARMLED闪动
**输入参数 ：
**输出参数 ：
********************************************************************************************************/


int main(void)
{
    //DMA的测试
    
    delay_init();
    uart_initwBaudRate(115200); //UART1的发送，可以通过串口软件打印UART OK
    LED_Init();
    DMA_m8tom8_test();
    // 	DMA_m8tom16_test();
    // 	DMA_m8tom32_test();
    //  DMA_m16tom8_test();
    //  DMA_m16tom16_test();
    //	DMA_m16tom32_test();
    
    
    while(1)              //无限循环
    {
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        delay_ms(1000);
    }
}

