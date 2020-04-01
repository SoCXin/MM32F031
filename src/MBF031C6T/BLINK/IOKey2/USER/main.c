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
#include "stdio.h"

extern u32 SystemCoreClock;
void delay_init(void);
void delay_ms(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
void LED_Init(void);
void KEY_Init(void);
u8 KEY_Scan(u8 mode);

static __IO uint32_t TimingDelay;


#define LED1_Port  GPIOB
#define LED1_Pin   GPIO_Pin_5
#define LED2_Port  GPIOB
#define LED2_Pin   GPIO_Pin_4
#define LED3_Port  GPIOB
#define LED3_Pin   GPIO_Pin_3
#define LED4_Port  GPIOA
#define LED4_Pin   GPIO_Pin_15

#define LED1_ON()  GPIO_ResetBits(LED1_Port,LED1_Pin)
#define LED1_OFF()  GPIO_SetBits(LED1_Port,LED1_Pin)
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(LED1_Port,LED1_Pin))?(GPIO_ResetBits(LED1_Port,LED1_Pin)):(GPIO_SetBits(LED1_Port,LED1_Pin))



#define LED2_ON()  GPIO_ResetBits(LED2_Port,LED2_Pin)
#define LED2_OFF()  GPIO_SetBits(LED2_Port,LED2_Pin)
#define LED2_TOGGLE()  (GPIO_ReadOutputDataBit(LED2_Port,LED2_Pin))?(GPIO_ResetBits(LED2_Port,LED2_Pin)):(GPIO_SetBits(LED2_Port,LED2_Pin))


#define LED3_ON()  GPIO_ResetBits(LED3_Port,LED3_Pin)
#define LED3_OFF()  GPIO_SetBits(LED3_Port,LED3_Pin)
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(LED3_Port,LED3_Pin))?(GPIO_ResetBits(LED3_Port,LED3_Pin)):(GPIO_SetBits(LED3_Port,LED3_Pin))


#define LED4_ON()  GPIO_ResetBits(LED4_Port,LED4_Pin)
#define LED4_OFF()  GPIO_SetBits(LED4_Port,LED4_Pin)
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(LED4_Port,LED4_Pin))?(GPIO_ResetBits(LED4_Port,LED4_Pin)):(GPIO_SetBits(LED4_Port,LED4_Pin))



//#define LED1_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)	// PB5
//#define LED1_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)	// PB5
//#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?(GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))	// PB5



//#define LED2_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_4)	// PB4
//#define LED2_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)	// PB4
//#define LED2_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?(GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))	// PB4


//#define LED3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_3)	// PB3
//#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_3)	// PB3
//#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3))?(GPIO_ResetBits(GPIOB,GPIO_Pin_3)):(GPIO_SetBits(GPIOB,GPIO_Pin_3))	// PB3


//#define LED4_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)	// PA15
//#define LED4_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15)	// PA15
//#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15))	// PA15

#define SW1_GPIO_Port  GPIOC
#define SW1_Pin   GPIO_Pin_13
#define SW2_GPIO_Port  GPIOA
#define SW2_Pin   GPIO_Pin_0
#define SW3_GPIO_Port  GPIOB
#define SW3_Pin   GPIO_Pin_10
#define SW4_GPIO_Port  GPIOB
#define SW4_Pin   GPIO_Pin_11



/* USER CODE BEGIN 0 */
uint8_t sw1Count, sw2Count, sw3Count, sw4Count;
uint8_t pushFlag1, pushFlag2, pushFlag3, pushFlag4;
uint8_t swState1, swState2, swState3, swState4;

void keystatusInit(void)
{
    sw1Count = 0; sw2Count = 0; sw3Count = 0; sw4Count = 0;
    pushFlag1 = 0; pushFlag2 = 0; pushFlag3 = 0; pushFlag4 = 0;
    swState1 = 0; swState2 = 0; swState3 = 0; swState4 = 0;
}
/* USER CODE END 2 */
/* USER CODE BEGIN 1 */
GPIO_TypeDef* GPIO_PORT[] = {LED1_Port,
                             LED2_Port,
                             LED3_Port,
                             LED4_Port
                            };

const uint16_t GPIO_PIN[] = {LED1_Pin,
                             LED2_Pin,
                             LED3_Pin,
                             LED4_Pin
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

/* USER CODE BEGIN 2 */
void LED_Toggle(Led_TypeDef Led)
{
    (GPIO_ReadOutputDataBit(GPIO_PORT[Led], GPIO_PIN[Led])) ? (GPIO_ResetBits(GPIO_PORT[Led], GPIO_PIN[Led])) : (GPIO_SetBits(GPIO_PORT[Led], GPIO_PIN[Led]));
}


void test(void)
{
    /* USER CODE BEGIN WHILE */
    while (1) {
        if  ( swState1 == 1 ) {
            LED_Toggle(LED1);
//		  LEDx_TOGGLE(LED1_GPIO_Port,LED1_Pin);
            delay_ms(1);
        }
        if  ( swState2 == 1 ) {
            LED_Toggle(LED2);
//		  LEDx_TOGGLE(LED2_GPIO_Port,LED2_Pin);
            delay_ms(1);
        }
        if  ( swState3 == 1 ) {
            LED_Toggle(LED3);
//		  LEDx_TOGGLE(LED3_GPIO_Port,LED3_Pin);
            delay_ms(1);
        }
        if  ( swState4 == 1 ) {
            LED_Toggle(LED4);
//		  LEDx_TOGGLE(LED4_GPIO_Port,LED4_Pin);
            delay_ms(1);
        }
        /* USER CODE END WHILE */
    }
}



/* USER CODE END 1 */


void Key_Scan(void)
{
    /*���SW1�Ƿ��� */
    if(   GPIO_ReadInputDataBit(SW1_GPIO_Port, SW1_Pin) == Bit_RESET ) {
        sw1Count++;                                                             //SW1�����£�����sw1Count��1
        if(sw1Count >= 32) {                                                    //1MS�ж�һ�Σ�sw1Count���ڵ���32���������Ѱ���32ms
            if(pushFlag1 == 0) {                                                //�ж���û���ذ�����1Ϊ�У�0Ϊû��
                swState1 = 1;                                                   //���ð�����־
                sw1Count = 0;
                pushFlag1 = 1;                                                  //�����ذ�����־
            } else
                sw1Count = 0;                                                   //SW1���Ѿ����£�����sw1Count����
        } else
            swState1 = 0;

    } else {                                                                    //�ް�������
        sw1Count = 0;                                                           //����sw1Count
        swState1 = 0;                                                           //���������־
        pushFlag1 = 0;                                                          //����ذ�����־
    }

    /*���SW2�Ƿ��� */
    if(   GPIO_ReadInputDataBit(SW2_GPIO_Port, SW2_Pin) ==  Bit_SET) {//Bit_RESET
        sw2Count++;                         //SW2�����£�����sw2Count��1
        if(sw2Count >= 32) {                //1MS�ж�һ�Σ�sw2Count���ڵ���32���������Ѱ���32ms
            if(pushFlag2 == 0) {            //�ж���û���ذ�����1Ϊ�У�0Ϊû��
                swState2 = 1;                     //���ð�����־
                sw2Count = 0;
                pushFlag2 = 1;                   //�����ذ�����־
            } else
                sw2Count = 0;
        } else
            swState2 = 0;

    } else {                                        //�ް�������
        sw2Count = 0;                         //����sw2Count
        swState2 = 0;                          //���������־
        pushFlag2 = 0;                        //����ذ�����־
    }

    /*���SW3�Ƿ��� */
    if(   GPIO_ReadInputDataBit(SW3_GPIO_Port, SW3_Pin) == Bit_RESET ) {
        sw3Count++;                         //SW3�����£�����sw3Count��1
        if(sw3Count >= 32) {                //1MS�ж�һ�Σ�sw3Count���ڵ���32���������Ѱ���32ms
            if(pushFlag3 == 0) {            //�ж���û���ذ�����1Ϊ�У�0Ϊû��
                swState3 = 1;                     //���ð�����־
                sw3Count = 0;
                pushFlag3 = 1;                   //�����ذ�����־
            } else
                sw3Count = 0;
        } else
            swState3 = 0;

    } else {                                        //�ް�������
        sw3Count = 0;                         //����sw3Count
        swState3 = 0;                          //���������־
        pushFlag3 = 0;                        //����ذ�����־
    }

    /*���SW4�Ƿ��� */
    if(   GPIO_ReadInputDataBit(SW4_GPIO_Port, SW4_Pin) == Bit_RESET ) {
        sw4Count++;                         //SW4�����£�����sw4Count��1
        if(sw4Count >= 32) {                //1MS�ж�һ�Σ�sw4Count���ڵ���32���������Ѱ���32ms
            if(pushFlag4 == 0) {            //�ж���û���ذ�����1Ϊ�У�0Ϊû��
                swState4 = 1;                     //���ð�����־
                sw4Count = 0;
                pushFlag4 = 1;                   //�����ذ�����־
            } else
                sw4Count = 0;
        } else
            swState4 = 0;

    } else {                                        //�ް�������
        sw4Count = 0;                         //����sw4Count
        swState4 = 0;                          //���������־
        pushFlag4 = 0;                        //����ذ�����־
    }

}
/* USER CODE END 0 */
/********************************************************************************************************
**������Ϣ ��main(void)
**�������� ��
**������� ����
**������� ����
********************************************************************************************************/
int main(void)
{

    delay_init();
    LED_Init();                                                                 //��ʼ����LED���ӵ�Ӳ���ӿ�
    KEY_Init();                                                                 //��ʼ���밴�����ӵ�Ӳ���ӿ�
    keystatusInit();
    test();
    while(1) {
    }
}

/********************************************************************************************************
**������Ϣ ��delay_init(void)
**�������� ��systick��ʱ������ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void delay_init(void)
{
    if (SysTick_Config(SystemCoreClock / 1000)) {
        /* Capture error */
        while (1);
    }
    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x0);                                        //SysTick�ж����ȼ�����
}

/********************************************************************************************************
**������Ϣ ��SysTick_Handler(void)
**�������� ��������жϺ�����Systick���еݼ�
**������� ����
**������� ����
********************************************************************************************************/
void SysTick_Handler(void)
{
    TimingDelay_Decrement();
    Key_Scan();
}

/********************************************************************************************************
**������Ϣ ��TimingDelay_Decrement(void)
**�������� ����1ms���ٶȵݼ�
**������� ��pclk2������ϵͳʱ��Ϊ8MHz������Ӧ����8
**������� ����
********************************************************************************************************/
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00) {
        TimingDelay--;
    }
}

/********************************************************************************************************
**������Ϣ ��delay_ms(__IO uint32_t nTime)
**�������� ������Ӧ�õ�����ʱ��ʹ��systick
**������� ��nTime����ʱ
**������� ����
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

/********************************************************************************************************
**������Ϣ ��void KEY_Init(void)
**�������� ��������ʼ������
**������� ��
**������� ��
**    ��ע ��
********************************************************************************************************/
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;


    GPIO_Clock_Set(GPIOA, ENABLE);  //����GPIOAʱ��
    GPIO_Clock_Set(GPIOB, ENABLE);  //����GPIOBʱ��
    GPIO_Clock_Set(GPIOC, ENABLE);  //����GPIOCʱ��
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;                                 //PC13��K1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //���ó���������
    GPIO_Init(GPIOC, &GPIO_InitStructure);                                      //��ʼ��GPIOC13

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;                                  //PA0,K2��WK_UP��
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;                               //���ó���������
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //��ʼ��GPIOA0

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11;                   //PB10,PB11,K3,K4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //���ó���������
    GPIO_Init(GPIOB, &GPIO_InitStructure);                                      //��ʼ��GPIOB.10,11
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

