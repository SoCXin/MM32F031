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
    /*���SW1�Ƿ��� */
    KEY1BitBuf =  ( KEY1BitBuf << 1 ) ;
    if ( GPIO_ReadInputDataBit(SW1_GPIO_Port, SW1_Pin) == Bit_RESET ) {
        KEY1BitBuf = KEY1BitBuf | 0x01; //����������1λ��������ǰֵ�������λ
        if ( (0xFFFF) == KEY1BitBuf ) { //����16��ɨ��,�а������룬��16�����ڶ���⵽����״̬������Ϊ��������
            //1MS�ж�һ�Σ�sw1Count���ڵ���32���������Ѱ���32ms
            KEY1State = 1;                                                   //���ð�����־
        }
    } else {
        if ( 0x0 == KEY1BitBuf ) { //��������
            KEY1State = 0;
        }
    }
    /*���SW2�Ƿ��� */
    KEY2BitBuf =  ( KEY2BitBuf << 1 ) ;
    if ( GPIO_ReadInputDataBit(SW2_GPIO_Port, SW2_Pin) ==  Bit_SET ) {
        KEY2BitBuf = KEY2BitBuf | 0x01; //����������1λ��������ǰֵ�������λ
        if ( (0xFFFF) == KEY2BitBuf ) { //����16��ɨ��,�а������룬��16�����ڶ���⵽����״̬������Ϊ��������
            //1MS�ж�һ�Σ�sw1Count���ڵ���32���������Ѱ���32ms
            KEY2State = 1;                                                   //���ð�����־
        }
    } else {
        if ( 0x0 == KEY2BitBuf ) { //��������
            KEY2State = 0;
        }
    }
    /*���SW3�Ƿ��� */
    KEY3BitBuf =  ( KEY3BitBuf << 1 ) ;
    if ( GPIO_ReadInputDataBit(SW3_GPIO_Port, SW3_Pin) == Bit_RESET ) {
        KEY3BitBuf = KEY3BitBuf | 0x01; //����������1λ��������ǰֵ�������λ
        if ( (0xFFFF) == KEY3BitBuf ) { //����16��ɨ��,�а������룬��16�����ڶ���⵽����״̬������Ϊ��������
            //1MS�ж�һ�Σ�sw1Count���ڵ���32���������Ѱ���32ms
            KEY3State = 1;                                                   //���ð�����־
        }
    } else {
        if ( 0x0 == KEY3BitBuf ) { //��������
            KEY3State = 0;
        }
    }
    /*���SW4�Ƿ��� */
    KEY4BitBuf =  ( KEY4BitBuf << 1 ) ;
    if ( GPIO_ReadInputDataBit(SW4_GPIO_Port, SW4_Pin) == Bit_RESET ) {
        KEY4BitBuf = KEY4BitBuf | 0x01; //����������1λ��������ǰֵ�������λ
        if ( (0xFFFF) == KEY4BitBuf ) { //����16��ɨ��,�а������룬��16�����ڶ���⵽����״̬������Ϊ��������
            //1MS�ж�һ�Σ�sw1Count���ڵ���32���������Ѱ���32ms
            KEY4State = 1;                                                   //���ð�����־
        }
    } else {
        if ( 0x0 == KEY4BitBuf ) { //��������
            KEY4State = 0;
        }
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
**������Ϣ ��LED_Init(void)
**�������� ��LED��ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    //LED����ʹ��
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
**������Ϣ ��void KEY_Init(void)
**�������� ��������ʼ������
**������� ��
**������� ��
**    ��ע ��
*******************************************************************************/
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

