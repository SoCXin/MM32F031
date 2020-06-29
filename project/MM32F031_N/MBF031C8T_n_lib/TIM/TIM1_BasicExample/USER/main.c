#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "tim1.h"
extern u32 SystemCoreClock;

int main(void)
{
    unsigned int uiCnt=0;
    
    delay_init();
    uart_initwBaudRate(115200);
    Tim1_UPCount_test(SystemCoreClock/10000-1,9999);
    while(1)              //无限循环
    {
        for(uiCnt=0; ;uiCnt++)
        {
            /* 等待定时器溢出 */
            Tim1_UPStatusOVCheck_test();
            
            /* 打印当前数值 */
            printf("0x%d\r\n",uiCnt);
        }
    }
}
