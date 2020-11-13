#include "key.h"
#include "delay.h"


//按键初始化函数 
//PA0 PC13 PB10 PB11设置成输入
void KEY_Init(void)
{
    
    
    RCC->AHBENR|=0xf<<17;   //RCC->APB2ENR|=1<<2;  //使能GPIOA时钟	   	    
    
    GPIOA->CRL&=0XFFFFFFF0;	//PA0设置成输入	,K2(WK_UP)
    GPIOA->CRL|=GPIO_CRL_CNF0_1;   
    
    GPIOC->CRH&=0XFF0FFFFF;	//PC13设置成输入,K1	  
    GPIOC->CRH|=GPIO_CRH_CNF13_1;   
    GPIOC->ODR|=GPIO_ODR_ODR13;	   	//PC13上拉 
    
    GPIOB->CRH&=0XFFFF00FF;	//PB10,PB11设置成输入,K3,K4	  
    GPIOB->CRH|=GPIO_CRH_CNF10_1|GPIO_CRH_CNF11_1;   
    GPIOB->ODR|=GPIO_ODR_ODR10|GPIO_ODR_ODR11;	   	//PB10,PB11上拉 
    
} 
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//KEY1_PRES，KEY1按下
//WKUP_PRES，WK_UP按下 
//KEY3_PRES，KEY3按下
//KEY4_PRES，KEY4按下
u8 KEY_Scan(u8 mode)
{	 
    static u8 key_up=1;//按键按松开标志
    if(mode)key_up=1;  //支持连按		  
    if(key_up&&(KEY1==0||WK_UP==1||KEY3==0||KEY4==0))
    {
        delay_ms(10);//去抖动 
        key_up=0;
        if(KEY1==0)return KEY1_PRES;
        else if(WK_UP==1)return WKUP_PRES; 
        else if(KEY3==0)return KEY3_PRES;
        else if(KEY4==0)return KEY4_PRES;
    }else if(KEY1==1&&KEY3==1&&KEY4==1&&WK_UP==0)key_up=1; 	     
    return 0;// 无按键按下
}



















