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
#include "stdio.h"
#include "HAL_conf.h"

static __IO uint32_t TimingDelay;


#define LED1_Port             GPIOB
#define LED1_Pin              GPIO_Pin_5
#define LED2_Port             GPIOB
#define LED2_Pin              GPIO_Pin_4
#define LED3_Port             GPIOB
#define LED3_Pin              GPIO_Pin_3
#define LED4_Port             GPIOA
#define LED4_Pin              GPIO_Pin_15

#define LED1_ON()             GPIO_ResetBits(LED1_Port,LED1_Pin)
#define LED1_OFF()            GPIO_SetBits(LED1_Port,LED1_Pin)
#define LED1_TOGGLE()         (GPIO_ReadOutputDataBit(LED1_Port,LED1_Pin))?(GPIO_ResetBits(LED1_Port,LED1_Pin)):(GPIO_SetBits(LED1_Port,LED1_Pin))



#define LED2_ON()             GPIO_ResetBits(LED2_Port,LED2_Pin)
#define LED2_OFF()            GPIO_SetBits(LED2_Port,LED2_Pin)
#define LED2_TOGGLE()         (GPIO_ReadOutputDataBit(LED2_Port,LED2_Pin))?(GPIO_ResetBits(LED2_Port,LED2_Pin)):(GPIO_SetBits(LED2_Port,LED2_Pin))


#define LED3_ON()             GPIO_ResetBits(LED3_Port,LED3_Pin)
#define LED3_OFF()            GPIO_SetBits(LED3_Port,LED3_Pin)
#define LED3_TOGGLE()         (GPIO_ReadOutputDataBit(LED3_Port,LED3_Pin))?(GPIO_ResetBits(LED3_Port,LED3_Pin)):(GPIO_SetBits(LED3_Port,LED3_Pin))


#define LED4_ON()             GPIO_ResetBits(LED4_Port,LED4_Pin)
#define LED4_OFF()            GPIO_SetBits(LED4_Port,LED4_Pin)
#define LED4_TOGGLE()         (GPIO_ReadOutputDataBit(LED4_Port,LED4_Pin))?(GPIO_ResetBits(LED4_Port,LED4_Pin)):(GPIO_SetBits(LED4_Port,LED4_Pin))

#define SW2_GPIO_Port         GPIOA
#define SW2_Pin               GPIO_Pin_0


void GPIO_Clock_Set(GPIO_TypeDef* GPIOx, FunctionalState NewState);
void LED_Init(void);
void WKUP_STOP_Init(void);


static void deleyNop(u32 DlyTime);
void SystemInit(void);


/*******************************************************************************
* @name   : main
* @brief  : stop mode
* @param  : None
* @retval : void
*******************************************************************************/
int main(void)
{
    u16 i;
    deleyNop(10000);
    LED_Init();
    for(i = 0; i < 10; i++) {
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        deleyNop(3000);
    }
    WKUP_STOP_Init();
    /*
    important notes
    need to set those GPIO Pins as AIN exclude the wake up pin;
    need to disalbe those interrupts (Systick, Timer), exclude the wake interrupt
    */
    PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
    /*
    After enter STOP mode, the SYSCLK changed to HSI/6
    Need re-initialize the SYSCLK , set HSI/4+PLL to 48Mhz as SYSCLK
    */
    SystemInit();
    /*
    re-initialize those GPIO Pins as previous setting before the wake up pin;
    re-enalbe those interrupts (Systick, Timer) as previous setting before the wake up pin
    */
    while(1) 
		{
        LED2_TOGGLE();
        deleyNop(3000);
    }
}



static void deleyNop(u32 DlyTime)
{
    u32 i, j;
    for(i = 0; i < DlyTime; i++) {
        for(j = 0; j < 100; j++) {
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
        }
    }
}


/*******************************************************************************
* @name   : EXTI0_1_IRQHandler
* @brief  : EXTI0_1 interrupt request handler
* @param  : void
* @retval : void
*******************************************************************************/
void EXTI0_1_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line0);
}

/*******************************************************************************
* @name   : WKUP_STOP_Init
* @brief  : WKUP STOP Initialize
* @param  : void
* @retval : void
*******************************************************************************/
void WKUP_STOP_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    GPIO_Clock_Set(GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,  EXTI_PinSource0);


    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

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
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, NewState);
    }
    if(GPIOx == GPIOB) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, NewState);
    }
    if(GPIOx == GPIOC) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, NewState);
    }
    if(GPIOx == GPIOD) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, NewState);
    }
}
/*******************************************************************************
**函数信息 ：LED_Init(void)
**功能描述 ：LED初始化
**输入参数 ：无
**输出参数 ：无
*******************************************************************************/
void LED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_Clock_Set(GPIOA, ENABLE);                                              //开启GPIOA时钟
    GPIO_Clock_Set(GPIOB, ENABLE);                                              //开启GPIOB时钟
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

