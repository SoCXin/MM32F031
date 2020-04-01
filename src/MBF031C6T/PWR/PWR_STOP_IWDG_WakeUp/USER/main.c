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


#define LED4_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)    // PA15
#define LED4_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15) // PA15
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15)) // PA15

#define LED3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_3) // PB3
#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_3)  // PB3
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3))?(GPIO_ResetBits(GPIOB,GPIO_Pin_3)):(GPIO_SetBits(GPIOB,GPIO_Pin_3))   // PB3

#define LED2_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_4) // PB4
#define LED2_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)  // PB4
#define LED2_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?(GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))   // PB4

#define LED1_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_5) // PB5
#define LED1_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)  // PB5
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?(GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))   // PB5


void LED_Init(void);
void RCC_ConfigInit(void);


void GPIO_ConfigInit(void);
void Iwdg_Interrupt_Config(void);
void WKUP_Init(void);
void Write_Iwdg_ON(u16 IWDG_Prescaler, u16 Reload);
void Write_Iwdg_RL(void);
void HSI_SYSCLK(void);
static void delayms(u32 uldelay);

int iwdg_flag = 0;


/*******************************************************************************
* @name   : main
* @brief  : stop mode
* @param  : None
* @retval : void
*******************************************************************************/
int main(void)
{
    u32 ulA = 0;
    delayms(2000);
    LED_Init();
    while (1) 
		{
        ulA++;
        delayms(1000);
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        if (ulA == 5) 
				{
            ulA = 0;
            break;
        }
    }
    RCC_ConfigInit();
    GPIO_ConfigInit();
    WKUP_Init();
    Iwdg_Interrupt_Config();
    Write_Iwdg_ON(IWDG_Prescaler_256, 1000);//256*1000/40000=6.4S
    delayms(60);
    PWR_EnterSTOPMode(0, PWR_STOPEntry_WFI);
    delayms(20);
    while (1) {
        if (iwdg_flag) {
            iwdg_flag = 0;
            ulA = 10;
            SystemInit();//set freq from HSI/6 to HSI

            LED_Init();
            while (ulA--) {
                LED1_TOGGLE();
                delayms(600);
            }
            Write_Iwdg_ON(IWDG_Prescaler_256, 1000);
            delayms(20);
            RCC_ConfigInit();
            GPIO_ConfigInit();
            delayms(20);
            PWR_EnterSTOPMode(0, PWR_STOPEntry_WFI);
            delayms(20);
        }

    }
}

void HSI_SYSCLK(void)
{
    RCC_HSICmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
    RCC->CFGR &= ~ 0xf;
    while ((RCC->CFGR & 0xf) != 0x0);
}

static void delayms(u32 uldelay)
{
    u32 i, j;
    for (i = 0; i < uldelay; i++) {
        for (j = 0; j < 3000; j++) {

        }
    }
}
void WWDG_IRQHandler()
{
    iwdg_flag = 1;
    EXTI_ClearITPendingBit(EXTI_Line24);//EXTI->PR |= 0x1 << 24;
    IWDG_ClearIT();
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

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);   //开启GPIOA,GPIOB时钟

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

/*******************************************************************************
* @name   : EXTI2_3_IRQHandler
* @brief  : EXTI2_3 interrupt request handler
* @param  : void
* @retval : void
*******************************************************************************/
void EXTI4_15_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line4);                                         // Çå³ýLINE10ÉÏµÄÖÐ¶Ï±êÖ¾Î»
}

/*******************************************************************************
* @name   : WKUP_Init
* @brief  : WKUP Initialize
* @param  : void
* @retval : void
*******************************************************************************/
void WKUP_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,  EXTI_PinSource4);

    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                         //config interrupt line
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* @name   : RCC_ConfigInit
* @brief  : RCC config
* @param  : None
* @retval : void
*******************************************************************************/
void RCC_ConfigInit(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD, ENABLE);
}

/*******************************************************************************
* @name   : GPIO_ConfigInit
* @brief  : GPIO config
* @param  : None
* @retval : void
*******************************************************************************/
void GPIO_ConfigInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin  =  0xFFEF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_All;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_All;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_All;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void Iwdg_Interrupt_Config()
{
    NVIC_InitTypeDef NVIC_InitSturcture;
    EXTI_InitTypeDef EXTI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //Íâ²¿ÖÐ¶Ï£¬ÐèÒªÊ¹ÄÜAFIOÊ±ÖÓ

    EXTI_InitStructure.EXTI_Line = EXTI_Line24;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitSturcture.NVIC_IRQChannel = WWDG_IRQn;
    NVIC_InitSturcture.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitSturcture.NVIC_IRQChannelPriority = 0;
    NVIC_Init(&NVIC_InitSturcture);
}





/*******************************************************************************
* @name   : PVU_CheckStatus
* @brief  : Check Status
* @param  : None
* @retval : void
*******************************************************************************/
void PVU_CheckStatus(void)
{
    while (IWDG_GetFlagStatus(IWDG_FLAG_PVU));
}

/*******************************************************************************
* @name   : RVU_CheckStatus
* @brief  : Check Status
* @param  : None
* @retval : void
*******************************************************************************/
void RVU_CheckStatus(void)
{
    while (IWDG_GetFlagStatus(IWDG_FLAG_RVU));
}

/*******************************************************************************
* @name   : Write_Iwdg_ON
* @brief  : Write_Iwdg_ON
* @param  : None
* @retval : void
*******************************************************************************/
void Write_Iwdg_ON(u16 IWDG_Prescaler, u16 Reload)
{

    RCC_LSICmd(ENABLE);                                                         //enable LSI
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    PVU_CheckStatus();                                                          //get IWDG status
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetPrescaler(IWDG_Prescaler);

    RVU_CheckStatus();                                                          //get IWDG status
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetReload(Reload & 0xfff);

    IWDG_EnableIT();                                                            //Enable Interrupt mode IWDG->CTRL = 0x01;

    IWDG_ReloadCounter();                                                       //load and enable IWDG
    IWDG_Enable();

}

/*******************************************************************************
* @name   : Write_Iwdg_RL
* @brief  : Write_Iwdg_RL
* @param  : None
* @retval : void
*******************************************************************************/
void Write_Iwdg_RL(void)
{
    IWDG_ReloadCounter();
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

