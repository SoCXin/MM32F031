#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"
/********************************************************************************************************
**������Ϣ ��main(void)                       
**�������� ��
**������� ����
**������� ����
********************************************************************************************************/
int main(void)
{
    u8 t=0;
    
    delay_init();	
    LED_Init();		  	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
    KEY_Init();          	//��ʼ���밴�����ӵ�Ӳ���ӿ�
    
    while(1)
    {
        t=KEY_Scan(0);		//�õ���ֵ
        switch(t)
        {				 
        case KEY1_PRES://K1Ĭ�ϲ���PC13,Ĭ������reset��λ���������԰���K1�Ḵλ
            LED1_TOGGLE();
            break;
        case WKUP_PRES:
            LED2_TOGGLE();
            break;
        case KEY3_PRES:				
            LED3_TOGGLE();
            break;
        case KEY4_PRES:				
            LED4_TOGGLE();
            break;
        default:
            delay_ms(10);	
        } 
    }
}



