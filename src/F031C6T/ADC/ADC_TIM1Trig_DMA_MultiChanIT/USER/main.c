/**
******************************************************************************
* @file     main.c
* @author   AE team
* @version  V1.1.1
* @date     15/05/2019
* @brief
******************************************************************************
*/

#include "string.h"
#include "HAL_device.h"
#include "HAL_conf.h"

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


#define REFVOLATGE 3.3 //5.0                                                         //Reference voltage, the unit is: V 

#define ADCSCANNUM               3    //u16 ADCScanNumber=0;
#define AVERAGELEN                   20
u16 varADCavarage[AVERAGELEN][3];
typedef enum {
    ADCch0                 = ADC_Channel_0,
    ADCch1                 = ADC_Channel_1,
    ADCch2                 = ADC_Channel_2,
    ADCch3                 = ADC_Channel_3,
    ADCch4                 = ADC_Channel_4,
    ADCch5                 = ADC_Channel_5,
    ADCch6                 = ADC_Channel_6,
    ADCch7                 = ADC_Channel_7,
    ADCch8                 = ADC_Channel_8,
    ADCch9                 = ADC_Channel_9,
    ADCch10                = ADC_Channel_10,
    ADCch11                = ADC_Channel_11,
    ADCch12                = ADC_Channel_12,
    ADCchTemp              = ADC_Channel_TempSensor,
    ADCchVref              = ADC_Channel_Vrefint,
} ADCch;
void ADCConfig(ADCch ADC_Channel);

u16 ADCValue[20];                                                               //Used to store ADC conversion value
float ADCVolatge[3];                                                               //Used to store the voltage value
u16 ADCFilterValue[3];                                                             //Used to store the value of the ADC second-order filter
u8 ADCflag = 0;                                                                   //DMA transfer completed flag
u8 ADCTrigFilterflag;                                                               //Filter completed sign

void GPIO_Config_AIN(GPIO_TypeDef* GPIOx, u16 GPIO_Pin_n);
void ADCMultiChannelInit(ADCch *pvchan, u8 number);
void DMAInit(void);
void ADCFilter(void);
void Get_ADCVolatge(void);

ADCch vchan[] = {
    ADCch0,
    ADCch4,
    ADCch5
};
u32 adc_value = 0;
extern uint32_t SystemCoreClock;
/*******************************************************************************
* @name   : InitSystick
* @brief  : Init Systick
* @param  : None
* @retval : void
*******************************************************************************/
void InitSystick()
{
    SysTick_Config(SystemCoreClock / 1000);
    NVIC_SetPriority(SysTick_IRQn, 0x00);
}

/*******************************************************************************
* @name   : SysTick_Handler
* @brief  : Systick interrupt
* @param  : None
* @retval : void
*******************************************************************************/
void SysTick_Handler()
{
    static u32 cnt;
    cnt++;
}

void ADCConfig(ADCch ADC_Channel)
{
    if( ADCch0 == ADC_Channel ) {
        GPIO_Config_AIN(GPIOA, GPIO_Pin_0);
    } else if( ADCch1 == ADC_Channel ) {
        GPIO_Config_AIN(GPIOA, GPIO_Pin_1);
    } else if( ADCch2 == ADC_Channel ) {
        GPIO_Config_AIN(GPIOA, GPIO_Pin_2);
    } else if( ADCch3 == ADC_Channel ) {
        GPIO_Config_AIN(GPIOA, GPIO_Pin_3);
    } else if( ADCch4 == ADC_Channel ) {
        GPIO_Config_AIN(GPIOA, GPIO_Pin_4);
    } else if( ADCch5 == ADC_Channel ) {
        GPIO_Config_AIN(GPIOA, GPIO_Pin_5);
    } else if( ADCch6 == ADC_Channel ) {
        GPIO_Config_AIN(GPIOA, GPIO_Pin_6);
    } else if( ADCch7 == ADC_Channel ) {
        GPIO_Config_AIN(GPIOA, GPIO_Pin_7);
    } else if( ADCch8 == ADC_Channel ) {
        GPIO_Config_AIN(GPIOB, GPIO_Pin_0);
    } else if( ADCch9 == ADC_Channel ) {
        GPIO_Config_AIN(GPIOB, GPIO_Pin_1);
    } else if( ADCch10 == ADC_Channel ) {
        GPIO_Config_AIN(GPIOB, GPIO_Pin_3);
    } else if( ADCch11 == ADC_Channel ) {
        GPIO_Config_AIN(GPIOB, GPIO_Pin_4);
    } else if( ADCch12 == ADC_Channel ) {
        GPIO_Config_AIN(GPIOB, GPIO_Pin_7);
    } else {
    }
}
/*******************************************************************************
* @name   : TimerInit(u16 Period,u16 Prescaler)
* @brief  : Systick interrupt
* @param  : None
* @retval : void
*******************************************************************************/
void Timer1Init(u16 Period, u16 Prescaler)
{
    TIM_TimeBaseInitTypeDef TIM_StructInit;
    TIM_OCInitTypeDef TIM_OCInitStruct;
    //Enable TIM1 clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);


    TIM_StructInit.TIM_Period = Period;
    TIM_StructInit.TIM_Prescaler = Prescaler;
    TIM_StructInit.TIM_ClockDivision = TIM_CKD_DIV1;                            //divide = 1
    TIM_StructInit.TIM_CounterMode = TIM_CounterMode_Up;                        //count mode
    TIM_StructInit.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_StructInit);

    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;                              //PWM1 mode
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;                  //TIM_OutputState_Disable
    TIM_OCInitStruct.TIM_Pulse = Period / 2;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;                       //if PWM1 set Low, PWM2 set High
    TIM_OC1Init(TIM1, &TIM_OCInitStruct);

    TIM_Cmd(TIM1, ENABLE);

    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
}


/********************************************************************************************************
**函数信息 ：void Tim1_CC1_init(u16 Period,u16 Prescaler)
**功能描述 ：配置定时器1向上计数模式
**输入参数 ：Period 16位计数器重载值,Prescaler 时钟预分频值
**输出参数 ：无
********************************************************************************************************/
void Tim1_CC1_init(u16 Prescaler, u16 Period)
{
    TIM_TimeBaseInitTypeDef TIM_StructInit;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    /*使能TIM1时钟,默认时钟源为PCLK2(PCLK2未分频时不倍频,否则由PCLK2倍频输出),可选其它时钟源*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_StructInit.TIM_Period = Period;                                         //ARR寄存器值
    TIM_StructInit.TIM_Prescaler = Prescaler;                                   //预分频值
    /*数字滤波器采样频率,不影响定时器时钟*/
    TIM_StructInit.TIM_ClockDivision = TIM_CKD_DIV1;                            //采样分频值
    TIM_StructInit.TIM_CounterMode = TIM_CounterMode_Up;                        //计数模式
    TIM_StructInit.TIM_RepetitionCounter = 0;

    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;//TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_Pulse = (Period + 1) / 2; //设置待装入捕获比较寄存器的脉冲值
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

    TIM_TimeBaseInit(TIM1, &TIM_StructInit);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
//    Tim1_Int_Enable();
//    Tim1_CC1_Enable();
    TIM_Cmd(TIM1, ENABLE);
}
/*******************************************************************************
* @name   : main
* @brief  :
* @param  :
* @retval :
*******************************************************************************/
int main(void)
{
    u16 chan, averagenumber = 0;
    LED_Init();
    DMAInit();
    ADCMultiChannelInit(&vchan[0], 3);
    ADC_ExternalTrigConvCmd(ADC1, ENABLE);
    Tim1_CC1_init(SystemCoreClock / 10000 - 1, 9);//Tim1_CC1_init(SystemCoreClock / 10000 - 1, 9999);
    while(1) {
        if(ADCflag == 1) {
            ADCflag = 0;
            LED1_TOGGLE();
            LED2_TOGGLE();
            LED3_TOGGLE();
            LED4_TOGGLE();
            for(chan = 0; chan < ADCSCANNUM; chan++) {
                varADCavarage[averagenumber][chan] = ADCValue[chan];
            }
            averagenumber++;
            if(averagenumber >= AVERAGELEN) {
                averagenumber = 0;
                ADCTrigFilterflag = 1;
            }
        }
        if(ADCTrigFilterflag) {                                                 //Determine whether the second-order filter is over
            ADCFilter();
            ADCTrigFilterflag = 0;                                              //Clear the filter end flag
            Get_ADCVolatge();                                                   //Convert the filtered value to voltage

        }

    }
}

/*******************************************************************************
* @name   : GPIO_Config_AIN
* @brief  : Initialize GPIO
* @param  : GPIO_TypeDef* GPIOx, u16 GPIO_Pin_n, GPIOMode_TypeDef GPIO_Mode (x can be A,B..D, n can be 0,1..15)
* @retval : void
*******************************************************************************/
void GPIO_Config_AIN(GPIO_TypeDef* GPIOx, u16 GPIO_Pin_n)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    if(GPIOx == GPIOA) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);                         //GPIO clock starts
    }
    if(GPIOx == GPIOB) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);                         //GPIO clock starts
    }
    if(GPIOx == GPIOC) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);                         //GPIO clock starts
    }
    if(GPIOx == GPIOD) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);                         //GPIO clock starts
    }
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_n;                                 //pin
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                           //Output speed
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                               //GPIO mode
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}

/*******************************************************************************
* @name   : ADC1_SingleChannel
* @brief  : ADC initialization
* @param  : u8 ADC_Channel_x (x can be 0,1..8)
* @retval : void
*******************************************************************************/
void ADCMultiChannelInit(ADCch *pvchan, u8 number)
{
    u8  i;
    ADC_InitTypeDef  ADC_InitStructure;
    ADC_StructInit(&ADC_InitStructure);

    //Initialize PA1 to analog input mode
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);                         //Enable ADC clock

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;                     //ADC prescale factor
    ADC_InitStructure.ADC_Mode = ADC_Mode_Single_Period;                        //Set ADC mode to continuous conversion mode
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                      //AD data right-justified
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_Init(ADC1, &ADC_InitStructure);

    //ADC_RegularChannelConfig(ADC, ADC_Channel_All_Disable, 0, ADC_SampleTime_13_5Cycles);//Block all channels
    for(i = 0; i < number; i++) {
        if((*pvchan) == ADC_Channel_TempSensor) {
            ADC_TempSensorCmd(ENABLE);                                       //Enable internal temperature sensor
        } else if((*pvchan) == ADC_Channel_Vrefint) {
            ADC_VrefintCmd(ENABLE);                                       //Enable internal temperature sensor
        } else {
            ADCConfig(*pvchan);
            ADC_RegularChannelConfig(ADC1, *pvchan, 0, ADC_SampleTime_7_5Cycles);//Enable the channel
        }
        pvchan++;
    }
    ADC_DMACmd(ADC1, ENABLE);                                                    //Enable ADCDMA
    ADC_Cmd(ADC1, ENABLE);                                                       //Enable AD conversion
}

/*******************************************************************************
* @name   : DMAInit
* @brief  : Initialize DMA
* @param  : void
* @retval : void
*******************************************************************************/
void DMAInit(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32) & (ADC1->ADDATA);           //DMA transfer peripheral address
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCValue;                      //DMA transfer memory address
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          //DMA transfer direction from peripheral to memory
    DMA_InitStructure.DMA_BufferSize = ADCSCANNUM;                                       //DMA cache size
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //After receiving the data, the peripheral address is forbidden to move backward
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //After receiving the data, the memory address is shifted backward
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //Define the peripheral data width to 16 bits
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //Define the memory data width to 16 bits
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             //Cycle conversion mode
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         //DMA priority is high
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                //M2M mode is disabled
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);

    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);                              //DMA interrupt initialization
    NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

/*******************************************************************************
* @name   : ADCFilter
* @brief  : Second order filtering
* @param  : void
* @retval : void
*******************************************************************************/
void ADCFilter(void)                                                            //ADC second-order filter, when the filter is completed erection filter end marker
{
    u16 chan;
    u16 cntFilter;
    u32 lADCFilterValue[ADCSCANNUM] = {0, 0, 0};
    for(chan = 0; chan < ADCSCANNUM; chan++) {
        for(cntFilter = 0; cntFilter < AVERAGELEN; cntFilter++) {
            lADCFilterValue[chan] += varADCavarage[cntFilter][chan];
        }
        ADCFilterValue[chan] = (lADCFilterValue[chan] / 20 * 30 + ADCFilterValue[chan] * 70) / 100;
    }


}

/*******************************************************************************
* @name   : Get_ADCVolatge
* @brief  : Convert the conversion data to voltage
* @param  : void
* @retval : void
*******************************************************************************/
void Get_ADCVolatge(void)
{
    u16 chan;
    for(chan = 0; chan < ADCSCANNUM; chan++) {
        ADCVolatge[chan] = ((float)ADCFilterValue[chan] / 4095) * REFVOLATGE;
    }
}

/*******************************************************************************
* @name   : DMA1_Channel1_IRQHandler
* @brief  : When DMA send data, set ADCflag, PA8, clear the interrupt flag, stop the conversion
* @param  : void
* @retval : void
*******************************************************************************/
void DMA1_Channel1_IRQHandler(void)
{
//    ADC_SoftwareStartConvCmd(ADC1, DISABLE);                                     //Stop Conversion
    DMA_ClearITPendingBit(DMA1_IT_TC1);                                          //Clear interrupt flag
    ADCflag = 1;                                                                //Erected transmission complete flag

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
