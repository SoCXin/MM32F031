#include "tim2.h" 
/********************************************************************************************************
**������Ϣ ��void TIM2_IRQHandler(void)                  
**�������� ����ʱ��2�жϺ���
**������� ����
**������� ����
********************************************************************************************************/
void TIM2_IRQHandler(void)
{
    /*�����ֶ�����жϱ�־λ*/
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    
    /*ȫ�ֱ�־*/
    ucTim2Flag++;
    
    
}

/********************************************************************************************************
**������Ϣ ��void Tim2_UPCount_test1(u16 Period,u16 Prescaler)                     
**�������� �����ö�ʱ��2���ϼ���ģʽ
**������� ��Period 16λ����������ֵ
Prescaler ʱ��Ԥ��Ƶֵ
**������� ����
********************************************************************************************************/
void Tim2_UPCount_test(u16 Prescaler,u16 Period)
{
    TIM_TimeBaseInitTypeDef TIM_StructInit;
    NVIC_InitTypeDef NVIC_StructInit;
    
    /*ʹ��TIM1ʱ��,Ĭ��ʱ��ԴΪPCLK1(PCLK1δ��Ƶʱ����Ƶ,������PCLK1��Ƶ���),��ѡ����ʱ��Դ*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    
    TIM_StructInit.TIM_Period=Period;                                                                      //ARR�Ĵ���ֵ
    TIM_StructInit.TIM_Prescaler=Prescaler;                                                                //Ԥ��Ƶֵ
    /*�����˲�������Ƶ��,��Ӱ�춨ʱ��ʱ��*/
    TIM_StructInit.TIM_ClockDivision=TIM_CKD_DIV1;                                                         //������Ƶֵ
    TIM_StructInit.TIM_CounterMode=TIM_CounterMode_Up;                                                     //����ģʽ
    TIM_StructInit.TIM_RepetitionCounter=0;
    
    TIM_TimeBaseInit(TIM2, &TIM_StructInit);
    
    /* ���ö�ʱ��2�ж�ͨ�������ȼ� */
    NVIC_StructInit.NVIC_IRQChannel=TIM2_IRQn;
    NVIC_StructInit.NVIC_IRQChannelPriority=1;
    NVIC_StructInit.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_StructInit);
    
    /*���¶�ʱ��ʱ���������ʱ��,�����־λ*/
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    /*����ʱ��2�����ж�*/
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    
    TIM_Cmd(TIM2, ENABLE);	
}
/********************************************************************************************************
**������Ϣ ��Tim2_UPStatusOVCheck_test1(void)                       
**�������� ���ȴ���ʱ�����
**������� ����
**������� ����
********************************************************************************************************/
void Tim2_UPStatusOVCheck_test(void)
{
    /*�ȴ��¼����±�־λ*/
    while(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update)==RESET);
    /*���¼���־λ*/
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
}


