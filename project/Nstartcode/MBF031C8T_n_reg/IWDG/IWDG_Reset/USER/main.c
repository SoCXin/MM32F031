#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "iwdg.h"

int main(void)
{
    
    delay_init();
    uart_initwBaudRate(115200);	 //���ڳ�ʼ��Ϊ115200
    
    printf("uart ok!\r\n");
    /*����ΪLSI32��Ƶ,��������ʼֵΪ0x7ff,��λʱ��ԼΪ1.6s*/
    Write_Iwdg_ON(0x03,0xf);
    while(1)//����ѭ��
    {
        //�޸�λ���������ѭ��,����ϵͳһֱ��ӡ��������
        //		Write_Iwdg_RL();     
    }	
    
    
}



