#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "dma.h"

u32 TEXT_TO_SEND[] = {'1','2','3','4','5','6','7','8','9'};
#define TEXT_LENTH 9
u8 SendBuff[(TEXT_LENTH+2)*100];

int main(void)
{
    u16 i;
    u8 t=0; 
    
    delay_init();
    uart_initwBaudRate(115200);	 //���ڳ�ʼ��Ϊ115200
    MYDMA_Config(DMA1_Channel2,(u32)&UART1->TDR,(u32)SendBuff,(TEXT_LENTH+2)*10);//DMA1ͨ��2,����Ϊ����1,�洢��ΪSendBuff,��(TEXT_LENTH+2)*100.
    printf("\r\nDMA SEND DATA:\r\n"); 	  
    for(i=0;i<(TEXT_LENTH+2)*100;i++)//���ASCII�ַ�������
    {
        if(t>=TEXT_LENTH)//���뻻�з�
        { 
            SendBuff[i++]=0x0d; 
            SendBuff[i]=0x0a; 
            t=0;
        }else SendBuff[i]=TEXT_TO_SEND[t++];//����TEXT_TO_SEND���    
    }		 
    
    UART1->GCR |= 0x0002;
    DMA1_Channel2->CCR |= 0x00000001;  
    //�ȴ�DMA������ɣ���ʱ������������һЩ�£����
    //ʵ��Ӧ���У����������ڼ䣬����ִ�����������
    while(1)
    {
        if(DMA1->ISR&0x00000020)//�ȴ�ͨ��2�������
        {
            DMA1->IFCR = 0x00000020;
            break; 
        }
    }
    
    printf("\r\nUART DMA TEST OK!\r\n "); 	    
    
    while(1);
    
}



