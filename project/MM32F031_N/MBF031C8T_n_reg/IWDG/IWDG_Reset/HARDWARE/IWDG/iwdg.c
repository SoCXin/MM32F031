#include "iwdg.h" 

/********************************************************************************************************
**������Ϣ ��Write_Iwdg_PR(void)             
**�������� �������������Ź�
**������� ��IWDG_Prescaler ��ѡIWDG_Prescaler_X, XΪ4,8,16,32,64,128,256,��Ӧ��Ƶֵ��Xȡֵ��ͬ
Reload<=0xfff,Ϊ����������ֵ
**������� ����
**    ��ע ����λʱ��������LSI 40KHzΪ�ο�
Tiwdg=(X/LSI)*Reload
********************************************************************************************************/
void Write_Iwdg_ON(unsigned short int IWDG_Prescaler,unsigned short int Reload)
{
    /*�����ڲ�����ʱ��,�ȴ�ʱ�Ӿ���*/
    RCC->CSR|=RCC_CSR_LSION;
    
    /*����ʱ��Ԥ��Ƶ*/	
    IWDG->KR=0X5555;
    IWDG->PR=IWDG_Prescaler;
    
    /*�������ؼĴ���ֵ*/	
    //RVU_CheckStatus();
    IWDG->KR=0X5555;
    IWDG->RLR = (Reload&0xfff);
    /*װ�ز�ʹ�ܼ�����*/	
    IWDG->KR=0XAAAA;
    IWDG->KR=0XCCCC;
}

//ι�������Ź�
void Write_Iwdg_RL(void)
{
    IWDG->KR=0XAAAA;//reload											   
}
