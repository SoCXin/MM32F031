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

void LED_Init(void);

void Tim1_UPCount_test(u16 Prescaler, u16 Period);
void Tim1_UPStatusOVCheck_test(void);

extern u32 SystemCoreClock;

unsigned int uiCnt = 0;
/********************************************************************************************************
**������Ϣ ��main(void)
**�������� ��
**������� ����
**������� ����
********************************************************************************************************/
int main(void)
{

    LED_Init();
    Tim1_UPCount_test(SystemCoreClock/10000-1, 19999);
    while(1) {
        uiCnt++;
        /* �ȴ���ʱ����� */
        Tim1_UPStatusOVCheck_test();
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
    }
}


/********************************************************************************************************
**������Ϣ ��void Tim1_UPCount_test1(u16 Period,u16 Prescaler)
**�������� �����ö�ʱ��1���ϼ���ģʽ
**������� ��Period 16λ����������ֵ,Prescaler ʱ��Ԥ��Ƶֵ
**������� ����
********************************************************************************************************/
void Tim1_UPCount_test(u16 Prescaler, u16 Period)
{
    TIM_TimeBaseInitTypeDef TIM_StructInit;

    /*ʹ��TIM1ʱ��,Ĭ��ʱ��ԴΪPCLK2(PCLK2δ��Ƶʱ����Ƶ,������PCLK2��Ƶ���),��ѡ����ʱ��Դ*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_TimeBaseStructInit(&TIM_StructInit);
    TIM_StructInit.TIM_Period = Period;                                         //ARR�Ĵ���ֵ
    TIM_StructInit.TIM_Prescaler = Prescaler;                                   //Ԥ��Ƶֵ
    /*�����˲�������Ƶ��,��Ӱ�춨ʱ��ʱ��*/
    TIM_StructInit.TIM_ClockDivision = TIM_CKD_DIV1;                            //������Ƶֵ
    TIM_StructInit.TIM_CounterMode = TIM_CounterMode_Up;                        //����ģʽ
    TIM_StructInit.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM1, &TIM_StructInit);

    TIM_Cmd(TIM1, ENABLE);

    /*���¶�ʱ��ʱ���������ʱ��,�����־λ*/
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
}
/********************************************************************************************************
**������Ϣ ��Tim1_UPStatusOVCheck_test1(void)
**�������� ���ȴ���ʱ�����
**������� ����
**������� ����
********************************************************************************************************/
void Tim1_UPStatusOVCheck_test(void)
{
    /*�ȴ��¼����±�־λ*/
    while(TIM_GetFlagStatus(TIM1, TIM_FLAG_Update) == RESET);
    /*���¼���־λ*/
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
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
**������Ϣ ��LED_Init(void)
**�������� ��LED��ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void LED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_Clock_Set(GPIOA, ENABLE);  //����GPIOAʱ��
    GPIO_Clock_Set(GPIOB, ENABLE);  //����GPIOBʱ��
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

