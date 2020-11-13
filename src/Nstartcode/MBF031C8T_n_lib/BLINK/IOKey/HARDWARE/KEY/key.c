#include "key.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//开发板
//按键输入 驱动代码		   
//////////////////////////////////////////////////////////////////////////////////	 

//按键初始化函数 
//PC13、PA0、PB10和PB11 设置成输入
void KEY_Init(void)
{
    
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC,ENABLE);//使能GPIOA,GPIOB,GPIOC时钟
    
    
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;//PC13，K1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC13
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PA0,K2（WK_UP）
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成下拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA0
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11;//PB10,PB11,K3,K4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA0设置成上拉输入 
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.10,11
    
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






