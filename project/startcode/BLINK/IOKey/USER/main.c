/**
******************************************************************************
* @file     main.c
* @author   Qitas
* @version  V1.1
* @date     
* @brief
******************************************************************************
*/

#include "HAL_device.h"
#include "HAL_conf.h"
#include "stdio.h"

extern u32 SystemCoreClock;
void delay_init(void);
void delay_ms(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
void LED_Init(void);
void KEY_Init(void);
u8 KEY_Scan(u8 mode);

static __IO uint32_t TimingDelay;


#define LED4_PORT               (GPIOB)
#define LED4_PIN                (GPIO_Pin_7)
#define LED3_PORT               (GPIOB)
#define LED3_PIN                (GPIO_Pin_6)
#define LED2_PORT               (GPIOB)
#define LED2_PIN                (GPIO_Pin_3)
#define LED1_PORT               (GPIOA)
#define LED1_PIN                (GPIO_Pin_15)

#define LED4_ON()  GPIO_ResetBits(LED4_PORT,LED4_PIN)	// PA15
#define LED4_OFF()  GPIO_SetBits(LED4_PORT,LED4_PIN)	// PA15
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(LED4_PORT,LED4_PIN))?(GPIO_ResetBits(LED4_PORT,LED4_PIN)):(GPIO_SetBits(LED4_PORT,LED4_PIN))	// PA15

#define LED3_ON()  GPIO_ResetBits(LED3_PORT,LED3_PIN)	// PB3
#define LED3_OFF()  GPIO_SetBits(LED3_PORT,LED3_PIN)	// PB3
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(LED3_PORT,LED3_PIN))?(GPIO_ResetBits(LED3_PORT,LED3_PIN)):(GPIO_SetBits(LED3_PORT,LED3_PIN))	// PB3

#define LED2_ON()  GPIO_ResetBits(LED2_PORT,LED2_PIN)	// PB4
#define LED2_OFF()  GPIO_SetBits(LED2_PORT,LED2_PIN)	// PB4
#define LED2_TOGGLE()  (GPIO_ReadOutputDataBit(LED2_PORT,LED2_PIN))?(GPIO_ResetBits(LED2_PORT,LED2_PIN)):(GPIO_SetBits(LED2_PORT,LED2_PIN))	// PB4

#define LED1_ON()  GPIO_ResetBits(LED1_PORT,LED1_PIN)	// PB5
#define LED1_OFF()  GPIO_SetBits(LED1_PORT,LED1_PIN)	// PB5
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(LED1_PORT,LED1_PIN))?(GPIO_ResetBits(LED1_PORT,LED1_PIN)):(GPIO_SetBits(LED1_PORT,LED1_PIN))	// PB5


#define KEY1_PORT               (GPIOB)
#define KEY1_PIN                (GPIO_Pin_5)
#define V_KEY1                  GPIO_ReadInputDataBit(KEY1_PORT,KEY1_PIN)		//读取按键1

#define KEY1  GPIO_ReadInputDataBit(KEY1_PORT,KEY1_PIN)//读取按键1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键2 
#define KEY3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)//读取按键3
#define KEY4  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)//读取按键4

#define KEY1_PRES	1		//KEY1 
#define WKUP_PRES	2		//WK_UP  
#define KEY3_PRES	3		//KEY3 
#define KEY4_PRES	4		//KEY4 

/********************************************************************************************************
**函数信息 ：main(void)
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
int main(void)
{
    u8 t = 0;

    delay_init();
    LED_Init();                                                                 //初始化与LED连接的硬件接口
    KEY_Init();                                                                 //初始化与按键连接的硬件接口
    while(1) {
        t = KEY_Scan(0);                                                        //得到键值
        if(KEY1_PRES == t) {
            LED1_TOGGLE();
            LED2_TOGGLE();
            LED3_TOGGLE();
            LED4_TOGGLE();
        } else if(WKUP_PRES == t) {
            LED2_TOGGLE();
        } else if(KEY3_PRES == t) {
            LED3_TOGGLE();
        } else if(KEY4_PRES == t) {
            LED4_TOGGLE();
        } else {
            delay_ms(10);
        }
    }
}

/********************************************************************************************************
**函数信息 ：delay_init(void)
**功能描述 ：systick延时函数初始化
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void delay_init(void)
{
    if (SysTick_Config(SystemCoreClock / 1000)) {
        /* Capture error */
        while (1);
    }
    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x0);                                        //SysTick中断优先级设置
}

/********************************************************************************************************
**函数信息 ：SysTick_Handler(void)
**功能描述 ：进入该中断函数后，Systick进行递减
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SysTick_Handler(void)
{
    TimingDelay_Decrement();
}

/********************************************************************************************************
**函数信息 ：TimingDelay_Decrement(void)
**功能描述 ：以1ms的速度递减
**输入参数 ：pclk2，例如系统时钟为8MHz，参数应传入8
**输出参数 ：无
********************************************************************************************************/
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00) {
        TimingDelay--;
    }
}

/********************************************************************************************************
**函数信息 ：delay_ms(__IO uint32_t nTime)
**功能描述 ：程序应用调用延时，使用systick
**输入参数 ：nTime：延时
**输出参数 ：无
********************************************************************************************************/
void delay_ms(__IO uint32_t nTime)
{
    TimingDelay = nTime;

    while(TimingDelay != 0);
}

/*******************************************************************************
* @name   : GPIO_Clock_Set
* @brief  : RCC clock set
* @param  : Portx , State
* @retval : void
*******************************************************************************/
void GPIO_Clock_Set(GPIO_TypeDef* GPIOx, FunctionalState NewState)
{

    if(GPIOx == GPIOA) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, NewState);                         //GPIO clock starts
    }
    if(GPIOx == GPIOB) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, NewState);                         //GPIO clock starts
    }
    if(GPIOx == GPIOC) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, NewState);                         //GPIO clock starts
    }
    if(GPIOx == GPIOD) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, NewState);                         //GPIO clock starts
    }
}
/********************************************************************************************************
**函数信息 ：LED_Init(void)
**功能描述 ：LED初始化
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void LED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_Clock_Set(LED1_PORT, ENABLE);  
    GPIO_Clock_Set(LED2_PORT, ENABLE);  
    GPIO_Clock_Set(LED4_PORT, ENABLE);
    GPIO_Clock_Set(LED3_PORT, ENABLE);  
    GPIO_InitStructure.GPIO_Pin  =  LED1_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED1_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  =  LED2_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED2_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  =  LED3_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED3_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  =  LED4_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED4_PORT, &GPIO_InitStructure);
    // GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    // GPIO_Init(GPIOB, &GPIO_InitStructure);
    LED1_ON();
    LED2_ON();
    LED3_ON();
    LED4_ON();
}


/********************************************************************************************************
**函数信息 ：void KEY_Init(void)
**功能描述 ：按键初始化函数
**输入参数 ：
**输出参数 ：
**    备注 ：
********************************************************************************************************/
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;


    GPIO_Clock_Set(GPIOA, ENABLE);  //开启GPIOA时钟
    GPIO_Clock_Set(KEY1_PORT, ENABLE);  //开启GPIOB时钟
    GPIO_Clock_Set(GPIOC, ENABLE);  //开启GPIOC时钟
    GPIO_InitStructure.GPIO_Pin  = KEY1_PIN;                                 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //设置成上拉输入
    GPIO_Init(KEY1_PORT, &GPIO_InitStructure);                                      //初始化GPIOC13

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;                                  //PA0,K2（WK_UP）
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;                               //设置成下拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //初始化GPIOA0

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11;                   //PB10,PB11,K3,K4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //设置成上拉输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);                                      //初始化GPIOB.10,11
}

/********************************************************************************************************
**函数信息 ：u8 KEY_Scan(u8 mode)
**功能描述 ：按键处理函数
**输入参数 ：mode  mode:0,不支持连续按;1,支持连续按;
**输出参数 ：0,没有按键按下;KEY1_PRES,KEY1按下;WKUP_PRES,WK_UP按下;KEY3_PRES,KEY3按下;KEY4_PRES，KEY4按下
**    备注 ：
********************************************************************************************************/
u8 KEY_Scan(u8 mode)
{
    static u8 key_up = 1;                                                       //按键按松开标志
    if(mode)key_up = 1;                                                         //支持连按
    if(key_up && (KEY1 == 0 || WK_UP == 1 || KEY3 == 0 || KEY4 == 0)) {
        delay_ms(10);                                                           //去抖动
        key_up = 0;
        if(KEY1 == 0)return KEY1_PRES;
        else if(WK_UP == 1)return WKUP_PRES;
        else if(KEY3 == 0)return KEY3_PRES;
        else if(KEY4 == 0)return KEY4_PRES;
    } else if(KEY1 == 1 && KEY3 == 1 && KEY4 == 1 && WK_UP == 0)key_up = 1;
    return 0;                                                                   //无按键按下
}


/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/*-------------------------(C) COPYRIGHT 2019 MindMotion ----------------------*/

