#include "uart_nvic.h"
#include "sys.h"



#if EN_UART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUARTx->SR�ܱ���Ī������Ĵ���   	
u8 UART_RX_BUF[UART_REC_LEN];     //���ջ���,���UART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 UART_RX_STA=0;       //����״̬���	  

void uart_nvic_init(u32 bound)
{  	 
    
    u32 tempBaud;	
    
    //-------------------------------------------------
    RCC->AHBENR|=0x7<<17;   //ʹ��GPIOA��ʱ��  
    RCC->APB2ENR|=1<<14;  //ʹ�ܴ���ʱ�� 
    
    RCC->APB2RSTR|=1<<14;   //��λ����1
    RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	 
    
    GPIOA->AFRH&=0x00f;
    GPIOA->AFRH|=0x110;
    
    //-------------------------------------------------	
    //���������� 
    /* Determine the uart_baud*/
    tempBaud = (SystemCoreClock / 16) /(bound);
    UART1->FRA=(SystemCoreClock / 16) %(bound);
    /* Write to UART BRR */
    UART1->BRR = tempBaud;
    UART1->CCR|=0X30;  //1λֹͣ,��У��λ.
    //-------------------------------------------------	
#if EN_UART1_RX		  //���ʹ���˽���
    //ʹ�ܽ����ж� 
    UART1->GCR = 0X19;			//�շ�ʹ��	UART1->CCR|=1<<5;    //���ջ������ǿ��ж�ʹ��	
    UART1->IER = 0X2;			//�����ж�ʹ��
    /*
    23.5.5 UART �ж�ʹ�ܼĴ���(UART_IER)
    ƫ�Ƶ�ַ��0x10
    ��λֵ��0x0000
    λ1
    RXIEN:���ջ����ж�ʹ��λ
    1=�ж�ʹ��
    0=�жϽ�ֹ	
    */
    UART1->ICR = 0X2;			//������ж�
    /*
    23.5.6 UART �ж�����Ĵ���(UART_ICR)
    ƫ�Ƶ�ַ��0x14
    ��λֵ��0x0000
    λ1
    RXICLR: �����ж����λ
    1=�ж����
    0=�ж�û�����
    */
    MY_NVIC_Init(3,3,UART1_IRQn,2);//��2��������ȼ� 
#endif
    /*
    λ14
    UART1EN��UART1ʱ��ʹ��
    ������á�1�����塯0��
    0��UART1ʱ�ӹرգ�
    1��UART1ʱ�ӿ�����	
    */
    GPIOA->CRH&=0XFFFFF00F;//IO״̬����
    GPIOA->CRH|=0X000008B0;//IO״̬����
    
}
#endif	
#if 1
void UART1_IRQHandler(void)                	//����1�жϷ������
{
    u8 Res;
    if((UART1->ISR & 0x2) != (uint16_t)RESET)	//���յ�����
    {	 
        Res=(uint16_t)(UART1->RDR & (uint16_t)0x00FF);
        UART1->ICR |= 2;//������ж� //		UART_ClearITPendingBit(UART1,UART_IT_RXIEN);
        if((Res==0x0d)&&((UART_RX_STA&0X3FFF)>0))
        {
            UART_RX_STA|=0x4000;
            UART_RX_BUF[UART_RX_STA&0X3FFF]=Res ;
            UART_RX_STA++;
        }
        else if((UART_RX_STA&0x4000)&&((UART_RX_STA&0X3FFF)>0))//���յ���0x0d
        {
            if(Res==0x0a)
            {
                UART_RX_STA|=0x8000;				
            }
            UART_RX_BUF[UART_RX_STA&0X3FFF]=Res ;
            UART_RX_STA++;
        }
        else{
            UART_RX_BUF[UART_RX_STA&0X3FFF]=Res ;
            UART_RX_STA++;
            UART_RX_STA=UART_RX_STA&0X3FFF;
            if((UART_RX_STA)>(UART_REC_LEN-1))
                UART_RX_STA=0;//�������ݴ���,���¿�ʼ����	
        }
    } 
} 
#endif	

