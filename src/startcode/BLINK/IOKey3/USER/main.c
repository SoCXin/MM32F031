/**
******************************************************************************
* @file     main.c
* @author   AE team
* @version  V1.1.1
* @date     15/05/2019
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


static __IO uint32_t TimingDelay;



#define LED4_PORT               (GPIOB)
#define LED4_PIN                (GPIO_Pin_5)
#define LED3_PORT               (GPIOB)
#define LED3_PIN                (GPIO_Pin_4)
#define LED2_PORT               (GPIOB)
#define LED2_PIN                (GPIO_Pin_3)
#define LED1_PORT               (GPIOA)
#define LED1_PIN                (GPIO_Pin_15)


#define LED4_ON()            GPIO_ResetBits(LED4_PORT,LED4_PIN)                  // PA15
#define LED4_OFF()           GPIO_SetBits(LED4_PORT,LED4_PIN)                    // PA15
#define LED4_TOGGLE()       (GPIO_ReadOutputDataBit(LED4_PORT,LED4_PIN))? \
    (GPIO_ResetBits(LED4_PORT,LED4_PIN)):         \
    (GPIO_SetBits(LED4_PORT,LED4_PIN))                   // PA15

#define LED3_ON()            GPIO_ResetBits(LED3_PORT,LED3_PIN)                   // PB3
#define LED3_OFF()           GPIO_SetBits(LED3_PORT,LED3_PIN)                     // PB3
#define LED3_TOGGLE()       (GPIO_ReadOutputDataBit(LED3_PORT,LED3_PIN))?  \
    (GPIO_ResetBits(LED3_PORT,LED3_PIN)):          \
    (GPIO_SetBits(LED3_PORT,LED3_PIN))                    // PB3

#define LED2_ON()            GPIO_ResetBits(LED2_PORT,LED2_PIN)                   // PB4
#define LED2_OFF()           GPIO_SetBits(LED2_PORT,LED2_PIN)                     // PB4
#define LED2_TOGGLE()       (GPIO_ReadOutputDataBit(LED2_PORT,LED2_PIN))?  \
    (GPIO_ResetBits(LED2_PORT,LED2_PIN)):          \
    (GPIO_SetBits(LED2_PORT,LED2_PIN))                    // PB4

#define LED1_ON()            GPIO_ResetBits(LED1_PORT,LED1_PIN)                   // PB5
#define LED1_OFF()           GPIO_SetBits(LED1_PORT,LED1_PIN)                     // PB5
#define LED1_TOGGLE()       (GPIO_ReadOutputDataBit(LED1_PORT,LED1_PIN))?  \
    (GPIO_ResetBits(LED1_PORT,LED1_PIN)):          \
    (GPIO_SetBits(LED1_PORT,LED1_PIN))                    // PB5


#define SW1_GPIO_Port         GPIOC
#define SW1_Pin               GPIO_Pin_13
#define SW2_GPIO_Port         GPIOA
#define SW2_Pin               GPIO_Pin_0
#define SW3_GPIO_Port         GPIOB
#define SW3_Pin               GPIO_Pin_10
#define SW4_GPIO_Port         GPIOB
#define SW4_Pin               GPIO_Pin_11

uint8_t KEY1State, KEY2State, KEY3State, KEY4State;

void keystatusInit(void)
{
    KEY1State = 0; KEY2State = 0; KEY3State = 0; KEY4State = 0;
}

GPIO_TypeDef* GPIO_PORT[] = {LED1_PORT,
                             LED2_PORT,
                             LED3_PORT,
                             LED4_PORT
                            };

const uint16_t GPIO_PIN[] = {LED1_PIN,
                             LED2_PIN,
                             LED3_PIN,
                             LED4_PIN
                            };
typedef enum {
    LED1,
    LED2,
    LED3,
    LED4
} Led_TypeDef;

void LEDx_TOGGLE(GPIO_TypeDef* GPIOx, uint16_t GPIO_pin)
{
    (GPIO_ReadOutputDataBit(GPIOx, GPIO_pin)) ? (GPIO_ResetBits(GPIOx, GPIO_pin)) : (GPIO_SetBits(GPIOx, GPIO_pin));
}

void LED_Toggle(Led_TypeDef Led)
{
    (GPIO_ReadOutputDataBit(GPIO_PORT[Led], GPIO_PIN[Led])) ? (GPIO_ResetBits(GPIO_PORT[Led], GPIO_PIN[Led])) : (GPIO_SetBits(GPIO_PORT[Led], GPIO_PIN[Led]));
}
static u8 KEY1DOWNRELEASE = 0;
u8 getKEY1downRelease(void)
{
    u8 result = 0;
    if(KEY1DOWNRELEASE == 0) {
        if  ( KEY1State == 1 ) {
            KEY1DOWNRELEASE = 1;
        }
    } else if(KEY1DOWNRELEASE == 1) {
        if  ( KEY1State == 0 ) {
            KEY1DOWNRELEASE = 2;
        }
    }
    if(KEY1DOWNRELEASE == 2) {
        result = 1;
        KEY1DOWNRELEASE = 0;
    }
    return result;
}
static u8 KEY2DOWNRELEASE = 0;
u8 getKEY2downRelease(void)
{
    u8 result = 0;
    if(KEY2DOWNRELEASE == 0) {
        if  ( KEY2State == 1 ) {
            KEY2DOWNRELEASE = 1;
        }
    } else if(KEY2DOWNRELEASE == 1) {
        if  ( KEY2State == 0 ) {
            KEY2DOWNRELEASE = 2;
        }
    }
    if(KEY2DOWNRELEASE == 2) {
        result = 1;
        KEY2DOWNRELEASE = 0;
    }
    return result;
}
static u8 KEY3DOWNRELEASE = 0;
u8 getKEY3downRelease(void)
{
    u8 result = 0;
    if(KEY3DOWNRELEASE == 0) {
        if  ( KEY3State == 1 ) {
            KEY3DOWNRELEASE = 1;
        }
    } else if(KEY3DOWNRELEASE == 1) {
        if  ( KEY3State == 0 ) {
            KEY3DOWNRELEASE = 2;
        }
    }
    if(KEY3DOWNRELEASE == 2) {
        result = 1;
        KEY3DOWNRELEASE = 0;
    }
    return result;
}
static u8 KEY4DOWNRELEASE = 0;
u8 getKEY4downRelease(void)
{
    u8 result = 0;
    if(KEY4DOWNRELEASE == 0) {
        if  ( KEY4State == 1 ) {
            KEY4DOWNRELEASE = 1;
        }
    } else if(KEY4DOWNRELEASE == 1) {
        if  ( KEY4State == 0 ) {
            KEY4DOWNRELEASE = 2;
        }
    }
    if(KEY4DOWNRELEASE == 2) {
        result = 1;
        KEY4DOWNRELEASE = 0;
    }
    return result;
}
void test(void)
{
    while (1) {
        if  ( getKEY1downRelease() == 1 ) {
            LED_Toggle(LED1);

        }
        if  ( getKEY2downRelease() == 1 ) {
            LED_Toggle(LED2);
        }
        if  ( getKEY3downRelease() == 1 ) {
            LED_Toggle(LED3);
        }
        if  ( getKEY4downRelease() == 1 ) {
            LED_Toggle(LED4);
        }
    }
}
static u16 KEY1BitBuf = 0x0;
static u16 KEY2BitBuf = 0x0;
static u16 KEY3BitBuf = 0x0;
static u16 KEY4BitBuf = 0x0;

void Key_Scan(void)
{
    /*检测SW1是否按下 */
    KEY1BitBuf =  ( KEY1BitBuf << 1 ) ;
    if ( GPIO_ReadInputDataBit(SW1_GPIO_Port, SW1_Pin) == Bit_RESET ) {
        KEY1BitBuf = KEY1BitBuf | 0x01; //缓存区左移1位，并将当前值移入最低位
        if ( (0xFFFF) == KEY1BitBuf ) { //连续16次扫描,有按键输入，即16毫秒内都检测到按下状态，即认为按键按下
            //1MS中断一次，sw1Count大于等于32，即按键已按下32ms
            KEY1State = 1;                                                   //设置按键标志
        }
    } else {
        if ( 0x0 == KEY1BitBuf ) { //按键弹起
            KEY1State = 0;
        }
    }
    /*检测SW2是否按下 */
    KEY2BitBuf =  ( KEY2BitBuf << 1 ) ;
    if ( GPIO_ReadInputDataBit(SW2_GPIO_Port, SW2_Pin) ==  Bit_SET ) {
        KEY2BitBuf = KEY2BitBuf | 0x01; //缓存区左移1位，并将当前值移入最低位
        if ( (0xFFFF) == KEY2BitBuf ) { //连续16次扫描,有按键输入，即16毫秒内都检测到按下状态，即认为按键按下
            //1MS中断一次，sw1Count大于等于32，即按键已按下32ms
            KEY2State = 1;                                                   //设置按键标志
        }
    } else {
        if ( 0x0 == KEY2BitBuf ) { //按键弹起
            KEY2State = 0;
        }
    }
    /*检测SW3是否按下 */
    KEY3BitBuf =  ( KEY3BitBuf << 1 ) ;
    if ( GPIO_ReadInputDataBit(SW3_GPIO_Port, SW3_Pin) == Bit_RESET ) {
        KEY3BitBuf = KEY3BitBuf | 0x01; //缓存区左移1位，并将当前值移入最低位
        if ( (0xFFFF) == KEY3BitBuf ) { //连续16次扫描,有按键输入，即16毫秒内都检测到按下状态，即认为按键按下
            //1MS中断一次，sw1Count大于等于32，即按键已按下32ms
            KEY3State = 1;                                                   //设置按键标志
        }
    } else {
        if ( 0x0 == KEY3BitBuf ) { //按键弹起
            KEY3State = 0;
        }
    }
    /*检测SW4是否按下 */
    KEY4BitBuf =  ( KEY4BitBuf << 1 ) ;
    if ( GPIO_ReadInputDataBit(SW4_GPIO_Port, SW4_Pin) == Bit_RESET ) {
        KEY4BitBuf = KEY4BitBuf | 0x01; //缓存区左移1位，并将当前值移入最低位
        if ( (0xFFFF) == KEY4BitBuf ) { //连续16次扫描,有按键输入，即16毫秒内都检测到按下状态，即认为按键按下
            //1MS中断一次，sw1Count大于等于32，即按键已按下32ms
            KEY4State = 1;                                                   //设置按键标志
        }
    } else {
        if ( 0x0 == KEY4BitBuf ) { //按键弹起
            KEY4State = 0;
        }
    }
}
/* USER CODE END 0 */
/********************************************************************************************************
**函数信息 ：main(void)
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
int main(void)
{
    delay_init();
    LED_Init();                                                                 //初始化与LED连接的硬件接口
    KEY_Init();                                                                 //初始化与按键连接的硬件接口
    keystatusInit();
    test();
    while(1) {
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
    Key_Scan();
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
    //LED控制使能
    GPIO_Clock_Set(LED4_PORT, ENABLE);  
    GPIO_InitStructure.GPIO_Pin  =  LED4_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED4_PORT, &GPIO_InitStructure);
    GPIO_Clock_Set(LED3_PORT, ENABLE);  
    GPIO_InitStructure.GPIO_Pin  =  LED3_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED3_PORT, &GPIO_InitStructure);
    GPIO_Clock_Set(LED2_PORT, ENABLE);  
    GPIO_InitStructure.GPIO_Pin  =  LED2_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED2_PORT, &GPIO_InitStructure);
    GPIO_Clock_Set(LED1_PORT, ENABLE);  
    GPIO_InitStructure.GPIO_Pin  =  LED1_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED1_PORT, &GPIO_InitStructure);
    LED1_ON();
    LED2_ON();
    LED3_ON();
    LED4_ON();
}

/*******************************************************************************
**函数信息 ：void KEY_Init(void)
**功能描述 ：按键初始化函数
**输入参数 ：
**输出参数 ：
**    备注 ：
*******************************************************************************/
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

