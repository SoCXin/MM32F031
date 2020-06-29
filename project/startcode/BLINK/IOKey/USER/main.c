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
#define V_KEY1                  GPIO_ReadInputDataBit(KEY1_PORT,KEY1_PIN)		//��ȡ����1

#define KEY1  GPIO_ReadInputDataBit(KEY1_PORT,KEY1_PIN)//��ȡ����1
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����2 
#define KEY3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)//��ȡ����3
#define KEY4  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)//��ȡ����4

#define KEY1_PRES	1		//KEY1 
#define WKUP_PRES	2		//WK_UP  
#define KEY3_PRES	3		//KEY3 
#define KEY4_PRES	4		//KEY4 

/********************************************************************************************************
**������Ϣ ��main(void)
**�������� ��
**������� ����
**������� ����
********************************************************************************************************/
int main(void)
{
    u8 t = 0;

    delay_init();
    LED_Init();                                                                 //��ʼ����LED���ӵ�Ӳ���ӿ�
    KEY_Init();                                                                 //��ʼ���밴�����ӵ�Ӳ���ӿ�
    while(1) {
        t = KEY_Scan(0);                                                        //�õ���ֵ
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
    GPIO_Clock_Set(KEY1_PORT, ENABLE);  //����GPIOBʱ��
    GPIO_Clock_Set(GPIOC, ENABLE);  //����GPIOCʱ��
    GPIO_InitStructure.GPIO_Pin  = KEY1_PIN;                                 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //���ó���������
    GPIO_Init(KEY1_PORT, &GPIO_InitStructure);                                      //��ʼ��GPIOC13

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;                                  //PA0,K2��WK_UP��
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;                               //���ó���������
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //��ʼ��GPIOA0

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11;                   //PB10,PB11,K3,K4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //���ó���������
    GPIO_Init(GPIOB, &GPIO_InitStructure);                                      //��ʼ��GPIOB.10,11
}

/********************************************************************************************************
**������Ϣ ��u8 KEY_Scan(u8 mode)
**�������� ������������
**������� ��mode  mode:0,��֧��������;1,֧��������;
**������� ��0,û�а�������;KEY1_PRES,KEY1����;WKUP_PRES,WK_UP����;KEY3_PRES,KEY3����;KEY4_PRES��KEY4����
**    ��ע ��
********************************************************************************************************/
u8 KEY_Scan(u8 mode)
{
    static u8 key_up = 1;                                                       //�������ɿ���־
    if(mode)key_up = 1;                                                         //֧������
    if(key_up && (KEY1 == 0 || WK_UP == 1 || KEY3 == 0 || KEY4 == 0)) {
        delay_ms(10);                                                           //ȥ����
        key_up = 0;
        if(KEY1 == 0)return KEY1_PRES;
        else if(WK_UP == 1)return WKUP_PRES;
        else if(KEY3 == 0)return KEY3_PRES;
        else if(KEY4 == 0)return KEY4_PRES;
    } else if(KEY1 == 1 && KEY3 == 1 && KEY4 == 1 && WK_UP == 0)key_up = 1;
    return 0;                                                                   //�ް�������
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

