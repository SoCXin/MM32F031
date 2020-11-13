#include "uart.h"
#include "sys.h"
#ifdef __GNUC__

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

#endif 

#ifdef USE_IAR
PUTCHAR_PROTOTYPE
{
    while((UART1->CSR&((uint16_t)0x0001))==0);//ѭ������,ֱ���������   
    UART1->TDR = (ch & (uint16_t)0x00FF);      
    return ch;
}

#else
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
    int handle; 
    
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
    x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
    while((UART1->CSR&((uint16_t)0x0001))==0);//ѭ������,ֱ���������   
    UART1->TDR = (ch & (uint16_t)0x00FF);      
    return ch;
}

#endif 



void uart_initwBaudRate(u32 bound)
{  	 
    
    u32 tempBaud;	
    
    //-------------------------------------------------
    RCC->AHBENR|=1<<17;   //ʹ��PORTA��ʱ��  
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

