#include "key.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������
//�������� ��������		   
//////////////////////////////////////////////////////////////////////////////////	 

//������ʼ������ 
//PC13��PA0��PB10��PB11 ���ó�����
void KEY_Init(void)
{
    
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC,ENABLE);//ʹ��GPIOA,GPIOB,GPIOCʱ��
    
    
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;//PC13��K1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
    GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC13
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0,K2��WK_UP��
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA0
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11;//PB10,PB11,K3,K4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA0���ó��������� 
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.10,11
    
} 
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//����ֵ��
//0��û���κΰ�������
//KEY1_PRES��KEY1����
//WKUP_PRES��WK_UP���� 
//KEY3_PRES��KEY3����
//KEY4_PRES��KEY4����
u8 KEY_Scan(u8 mode)
{	 
    static u8 key_up=1;//�������ɿ���־
    if(mode)key_up=1;  //֧������		  
    if(key_up&&(KEY1==0||WK_UP==1||KEY3==0||KEY4==0))
    {
        delay_ms(10);//ȥ���� 
        key_up=0;
        if(KEY1==0)return KEY1_PRES;
        else if(WK_UP==1)return WKUP_PRES; 
        else if(KEY3==0)return KEY3_PRES;
        else if(KEY4==0)return KEY4_PRES;
    }else if(KEY1==1&&KEY3==1&&KEY4==1&&WK_UP==0)key_up=1; 	     
    return 0;// �ް�������
}






