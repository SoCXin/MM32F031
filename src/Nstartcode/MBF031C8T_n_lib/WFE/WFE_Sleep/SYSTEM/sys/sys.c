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



