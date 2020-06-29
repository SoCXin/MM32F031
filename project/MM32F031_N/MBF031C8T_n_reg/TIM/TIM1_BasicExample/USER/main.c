#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "tim1.h"

int main(void)
{
    unsigned int uiCnt=0;
    
    delay_init();
    uart_initwBaudRate(115200);
    Tim1_UPCount_test(SystemCoreClock/10000-1,9999);
    
    while(1)              //����ѭ��
    {
        for(uiCnt=0; ;uiCnt++)
        {
            /* �ȴ���ʱ����� */
            Tim1_UPStatusOVCheck_test();
            
            /* ��ӡ��ǰ��ֵ */
            printf("0x%d\r\n",uiCnt);
        }
    }
}
