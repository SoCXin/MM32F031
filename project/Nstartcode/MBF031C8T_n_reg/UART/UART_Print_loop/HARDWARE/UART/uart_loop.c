#include "sys.h"
#include "uart_loop.h"
/********************************************************************************************************
**������Ϣ UartInit_Loop(void)                      
**�������� ����ʼ������
**������� ����
**������� ����
********************************************************************************************************/
void UartInit_Loop(void)
{
    
    //GPIO�˿�����	 
    RCC->AHBENR|=1<<17;   //ʹ��GPIOA��ʱ��  
    RCC->APB2ENR|=1<<14;  //ʹ�ܴ���ʱ�� 
    
    
    GPIOA->AFRH&=0xf00f;
    GPIOA->AFRH|=0x110;
    
    
    //UART ��ʼ������
    UART1->BRR = (SystemCoreClock / 16) /(115200);
    UART1->FRA=(SystemCoreClock / 16) %(115200);
    UART1->CCR|=0x30; //110000
    UART1->GCR|=0x19;  //11001;
    UART1->GCR |= 0x0001;                    //ʹ�ܴ���1 
    
    GPIOA->CRH&=0XFFFFF00F;//IO״̬����
    GPIOA->CRH|=0X000008B0;//IO״̬����
    
    printf("UART OK!\r\n");
}


/********************************************************************************************************
**������Ϣ ��uart_send(UART_TypeDef* UARTx,char c)                    
**�������� �����ڷ����ֽ�
**������� ��UART_TypeDef* UARTx ��ѡ��UART1��UART2��UART3
**������� ��char c ,���ڷ��͵��ֽ�
**������� ����
********************************************************************************************************/
void uart_send(char c)
{
    UART1->TDR = c;
    while(1)
    {
        if(UART1->ISR&0x1)
        {
            UART1->ICR = 0x1;
            break;
        }
    }
}

/********************************************************************************************************
**������Ϣ ��unsigned char inbyte(UART_TypeDef* UARTx)                
**�������� �����ڽ��պ���
**������� ��UART_TypeDef* UARTx ��ѡ��UART1��UART2��UART3
**������� ��unsigned char ���ڽ��շ��ص��ֽ�
********************************************************************************************************/
unsigned char inbyte(UART_TypeDef* UARTx)
{
    unsigned char temp;
    
    while(1)
    {
        if(UART1->ISR&0x2)
        {
            UART1->ICR = 0x2;
            break;
        }
    }
    temp = (uint8_t)UART1->RDR;
    if(temp==0xd)//���������յ�����
    {
        return 0;		
    }
    return temp;
}


/********************************************************************************************************
**������Ϣ ��void Uart1RxTest(UART_TypeDef* UARTx)               
**�������� �����ڽ��պ�������
**������� ��UART_TypeDef* UARTx ��ѡ��UART1��UART2��UART3
**������� ����
********************************************************************************************************/

void Uart1RxTest(UART_TypeDef* UARTx)
{
    unsigned char temp;
    temp = inbyte(UARTx);
    if(temp!=0)
    {
        printf("�����������Ϊ:%c\r\n",temp);	
    }			
}





