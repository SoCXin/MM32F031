#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "tim2.h"

unsigned int ucTim2Flag=0;
int main(void)
{
    unsigned int uiCnt=0;
    
    delay_init();
    uart_initwBaudRate(115200);
    Tim2_UPCount_test(SystemCoreClock/100000-1,99);
    while(1)              //����ѭ��
    {
        if(ucTim2Flag >999)
        {
            ucTim2Flag=0;
            
            /*��ӡ��ǰuiCntֵ */
            printf("Time=0x%ds\r\n",uiCnt);
            uiCnt++;
        }
    }
}
