#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "exti.h"
#include "key.h"
#include "led.h"


int main(void)
{
    //GPIO�ڲ��ԣ���˸LED��
    
    LED_Init();		  	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
    KEY_Init();          	//��ʼ���밴�����ӵ�Ӳ���ӿ�
    EXTIX_Init();
    
    while(1)
    {
    }
}


