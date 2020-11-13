#include "sys.h"

/********************************************************************************************************
**������Ϣ ��System_Clock_Init(u8 PLL)                     
**�������� ���ⲿʱ�ӱ�Ƶ����Ϊϵͳʱ��
**������� ��PLL����Ƶ����
**������� ����
********************************************************************************************************/
void System_Clock_Init(u8 PLL)
{
    unsigned char temp=0;   
    RCC->CR|=RCC_CR_HSEON;  //�ⲿ����ʱ��ʹ��HSEON
    while(!(RCC->CR&RCC_CR_HSERDY));//�ȴ��ⲿʱ�Ӿ���
    RCC->CFGR=RCC_CFGR_PPRE1_2; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
    
    RCC->CFGR|=RCC_CFGR_PLLSRC;	  //PLLSRC ON 
    RCC->CR &=~(RCC_CR_PLLON);		//��PLL//	RCC->CR &=~(7<<20);		//��PLL
    
    RCC->CR &=~(0x1f<<26);	
    RCC->CR|=(PLL - 1) << 26;   //����PLLֵ 2~16
    
    FLASH->ACR|=FLASH_ACR_LATENCY_1|FLASH_ACR_PRFTBE|FLASH_ACR_PRFTBS;	  //FLASH 2����ʱ����
    
    RCC->CR|=RCC_CR_PLLON;  //PLLON
    while(!(RCC->CR&RCC_CR_PLLRDY));//�ȴ�PLL����
    RCC->CFGR|=RCC_CFGR_SW_PLL;//PLL��Ϊϵͳʱ��	 
    while(temp!=0x02)     //�ȴ�PLL��Ϊϵͳʱ�����óɹ�
    {    
        temp=RCC->CFGR>>2;
        temp&=0x03;
    }    
}	





void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
    u32 temp,temp1;	  
    temp1=(~NVIC_Group)&0x07;//ȡ����λ
    temp1<<=8;
    temp=SCB->AIRCR;  //��ȡ��ǰ������
    temp&=0X0000F8FF; //�����ǰ����
    temp|=0X05FA0000; //д��Կ��
    temp|=temp1;	   
    SCB->AIRCR=temp;  //���÷���	    	  				   
}
//����NVIC 
//NVIC_PreemptionPriority:��ռ���ȼ�
//NVIC_SubPriority       :��Ӧ���ȼ�
//NVIC_Channel           :�жϱ��
//NVIC_Group             :�жϷ��� 0~4
//ע�����ȼ����ܳ����趨����ķ�Χ!����������벻���Ĵ���
//�黮��:
//��0:0λ��ռ���ȼ�,4λ��Ӧ���ȼ�
//��1:1λ��ռ���ȼ�,3λ��Ӧ���ȼ�
//��2:2λ��ռ���ȼ�,2λ��Ӧ���ȼ�
//��3:3λ��ռ���ȼ�,1λ��Ӧ���ȼ�
//��4:4λ��ռ���ȼ�,0λ��Ӧ���ȼ�
//NVIC_SubPriority��NVIC_PreemptionPriority��ԭ����,��ֵԽС,Խ����	   
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{ 
    u32 temp;	
    MY_NVIC_PriorityGroupConfig(NVIC_Group);//���÷���
    temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
    temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
    temp&=0xf;								//ȡ����λ  
    NVIC->ISER[NVIC_Channel/32]|=(1<<NVIC_Channel%32);//ʹ���ж�λ(Ҫ����Ļ�,�෴������OK) 
    NVIC->IP[NVIC_Channel]|=temp<<4;		//������Ӧ���ȼ����������ȼ�   	    	  				   
} 

