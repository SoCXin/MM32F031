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

u8 FLASH_Program(void);

static __IO uint32_t TimingDelay;
#define BANK1_WRITE_START_ADDR  ((uint32_t)0x08002800)
uint32_t Data = 0x12345679;

volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

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





int FLASH_EnableFullMainFlashReadOutProtect(void);
uint32_t CheckReadProtect(void);

//            void _enable_irq(void)  ;  //���PRIMASKֵ
//            void _disable_irq(void) ;  //����PRIMASKֵ
void SoftReset(void)
{
    //__set_FAULTMASK(1);//For Cortex-M3,M4  Disable Interrupt
    NVIC_SystemReset();//Reset

}

/********************************************************************************************************
**������Ϣ ��main(void)
**�������� ��
**������� ����
**������� ����
********************************************************************************************************/
int main(void)
{
    int status;
    u32 t = 0;
    u32 getresult = 0;
    delay_init();
    LED_Init();

    // Enable the Read Protect
    t = CheckReadProtect();
    if (t == 0) { //not protect
        FLASH_Unlock();
        status = FLASH_EnableFullMainFlashReadOutProtect();
        FLASH_Lock();
        if (status == 0) {
            getresult = 0;
        } else {
            getresult = 1;
            //set read protect failure
        }
    } else {
        //chip is set protect before
        getresult = 1;
        //TO DO
        //need to Set ReadProtectDisable
    }

    if (getresult == 0) { //success
        {
            LED1_TOGGLE();
            LED2_TOGGLE();
            LED3_TOGGLE();
            LED4_TOGGLE();
            delay_ms(500);
        }
    } else { //fail
        while (1) {
            LED1_TOGGLE();
            LED2_TOGGLE();
            LED3_TOGGLE();
            LED4_TOGGLE();
            delay_ms(100);
        }
    }
    while (1) {           //����ѭ��
        LED3_TOGGLE();
        LED4_TOGGLE();
        delay_ms(100);
    }
}

uint32_t CheckWriteProtectblank(void)
{

    uint32_t protectstatus = 0;
    u16 data1;
    int i = 0;

    for (i = 0; i < 4; i++) {
        data1 = *(u16 *)(0x1ffff808 + i * 2); //��ַAddress������2��������
        if (data1 != 0xFFFF) {
            protectstatus = 1;
            break;
        }
    }
    return protectstatus;
}


uint32_t CheckReadProtect(void)
{

    uint32_t protectstatus = 0;
    u16 data1;
    int i = 0;



    if ((FLASH->OBR & 0x02) != (uint32_t)RESET) {
        // Read Protect on 0x1FFFF800 is set
        protectstatus = 1;
    } else {
        for (i = 0; i < 8; i++) {
            data1 = *(u16 *)(0x1ffe0000 + i * 2); //��ַAddress������2��������
            if (data1 != 0xFFFF) {
                protectstatus = 2;
                break;
            }
        }
    }
    return protectstatus;

}
//portect Full main Flash
int FLASH_EnableFullMainFlashReadOutProtect(void)
{
    FLASH_Status status = FLASH_COMPLETE;
    int ret = 0;
    status = FLASH_ProgramOptionHalfWord(0x1ffe0000, 0x7F80);
    if (status != FLASH_COMPLETE)
        ret = 1;
    status = FLASH_ProgramOptionHalfWord(0x1ffe0002, 0xFF00);
    if (status != FLASH_COMPLETE)
        ret = 1;
    return ret;
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

    while (TimingDelay != 0);
}

/*******************************************************************************
* @name   : GPIO_Clock_Set
* @brief  : RCC clock set
* @param  : Portx , State
* @retval : void
*******************************************************************************/
void GPIO_Clock_Set(GPIO_TypeDef *GPIOx, FunctionalState NewState)
{

    if (GPIOx == GPIOA) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, NewState);                         //GPIO clock starts
    }
    if (GPIOx == GPIOB) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, NewState);                         //GPIO clock starts
    }
    if (GPIOx == GPIOC) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, NewState);                         //GPIO clock starts
    }
    if (GPIOx == GPIOD) {
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

