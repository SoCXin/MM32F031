/**
******************************************************************************
* @file     main.c
* @author   AE team
* @version  V1.1.1
* @date     15/05/2019
* @brief
******************************************************************************
* @copy
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2019 MindMotion</center></h2>
*/
#include "HAL_device.h"
#include "HAL_conf.h"

extern u32 SystemCoreClock;
void delay_init(void);
void delay_ms(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
void LED_Init(void);
void KEY_Init(void);
u8 KEY_Scan(u8 mode);

static __IO uint32_t TimingDelay;

#define LED4_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)	// PA15
#define LED4_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15)	// PA15
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15))	// PA15

#define LED3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_3)	// PB3
#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_3)	// PB3
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3))?(GPIO_ResetBits(GPIOB,GPIO_Pin_3)):(GPIO_SetBits(GPIOB,GPIO_Pin_3))	// PB3

#define LED2_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_4)	// PB4
#define LED2_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)	// PB4
#define LED2_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?(GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))	// PB4

#define LED1_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)	// PB5
#define LED1_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)	// PB5
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?(GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))	// PB5

#define KEY1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//读取按键1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键2 
#define KEY3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)//读取按键3
#define KEY4  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)//读取按键4

#define KEY1_PRES	1		//KEY1 
#define WKUP_PRES	2		//WK_UP  
#define KEY3_PRES	3		//KEY3 
#define KEY4_PRES	4		//KEY4 
void setalarmtime_ms(uint32_t nTime);
void setLEDtoggle(u32 freq);

uint32_t SysTickCnt = 0;
uint32_t systickLEDfreq;

void Sys_Standby(void);
/*
slow fast blink then slow 1s blink,
and wait key3(wkup key) to enter standby
if get Key3 enter standby
if get Other key , slow other freq led toggle
enter standby, MCU current is about 0.5uA
if get wkup key , then wake up from Standby as reset;
*/
/********************************************************************************************************
**函数信息 ：main(void)
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
int main(void)
{
    u8 t = 0;
    int i;
    setLEDtoggle(1000);
    delay_init();
    LED_Init();                                                                 //初始化与LED连接的硬件接口
    KEY_Init();                                                                 //初始化与按键连接的硬件接口
    //已从standby模式唤醒或刚上电

    //进入6s 100ms一下的LED快
    setLEDtoggle(100);
    delay_ms(6000);
    //进入1000ms一下的LED闪烁
    setLEDtoggle(1000);
    //
    //请按下KEY3按键进入Standby，按其他按键切换其他频率闪烁LED

    while(1) {
        t = KEY_Scan(0);                                                        //得到键值
        if(KEY1_PRES == t) {
            setLEDtoggle(300);
        } else if(WKUP_PRES == t) {
            setLEDtoggle(600);
        } else if(KEY3_PRES == t) {
            //请按下KEY3按键进入Standby
            Sys_Standby();
            //进入Standby后，只有按下WKUP（PA0）按键唤醒MCU，复位重启MCU
        } else if(KEY4_PRES == t) {
            setLEDtoggle(2000);
        } else {
            delay_ms(10);
        }
    }
}

/********************************************************************************************************
**函数信息 ：Sys_Standby(void)
**功能描述 ：进入待机模式
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void Sys_Standby(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);                         //使能PWR外设时钟
    //	RCC->APB2RSTR|=0X01FC;//复位所有IO口
    PWR_WakeUpPinCmd(ENABLE);                                                   //使能唤醒管脚功能
    PWR_EnterSTANDBYMode();                                                     //进入待命（STANDBY）模式
}
void setLEDtoggle(u32 freq)
{
    if((freq > 10000) || (freq == 0))
        freq = 10000;
    systickLEDfreq = freq;
}
static u32 abc = 0;
void ledtogglefreq(void)
{


    if(abc < systickLEDfreq)
        abc++;
    else {
        abc = 0;
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
    }
}


void setalarmtime_ms(uint32_t nTime)
{
    TimingDelay = nTime;
}
void checkalarmtime_ms(uint32_t *pnTime)
{
    *pnTime = TimingDelay;
}
static u8 key_up = 1;                                                           //按键按松开标志

u8 Systick_KEY_Scan(u8 mode)
{
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
    SysTickCnt++;
    ledtogglefreq();
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
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, NewState);                   //GPIO clock starts
    }
    if(GPIOx == GPIOB) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, NewState);                   //GPIO clock starts
    }
    if(GPIOx == GPIOC) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, NewState);                   //GPIO clock starts
    }
    if(GPIOx == GPIOD) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, NewState);                   //GPIO clock starts
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

    GPIO_Clock_Set(GPIOA, ENABLE);  //开启GPIOA时钟
    GPIO_Clock_Set(GPIOB, ENABLE);  //开启GPIOB时钟
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

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
    GPIO_Clock_Set(GPIOB, ENABLE);  //开启GPIOB时钟
    GPIO_Clock_Set(GPIOC, ENABLE);  //开启GPIOC时钟
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;                                 //PC13，K1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //设置成上拉输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);                                      //初始化GPIOC13

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

