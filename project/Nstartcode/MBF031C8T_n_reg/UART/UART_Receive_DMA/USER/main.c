#include "sys.h"
#include "uart.h"		
#include "delay.h"	
#include "dma.h" 

u8 DST_Buffer[10]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};//UART DMA���ܵ����ݴ������


int main(void)
{	
    u16 i;
    
    delay_init();
    uart_initwBaudRate(115200);	 	//���ڳ�ʼ��Ϊ115200		 	
    //��ʾ��ʾ��Ϣ	   
    printf("������10������!\r\n");
    printf("�������ֲ��ܹ�ѡ��������!\r\n");
    MYDMA_Config(DMA1_Channel3,(u32)&UART1->RDR,(u32)DST_Buffer,10);//DMA1ͨ��4,����Ϊ����1,�洢��ΪSendBuff,��(TEXT_LENTH+2)*100.
    
    UART1->GCR|=1<<1;           //ѡ�񴮿�1��DMA��ʽ����  
    UART1->GCR|=1<<3;           //ʹ�ܴ���1��   ���� 
    MYDMA_Enable(DMA1_Channel3);//��ʼһ��DMA���䣡	  
    while(1)
    {
        if(DMA1->ISR&(1<<9))//�ȴ�ͨ��4�������
        {
            DMA1->IFCR|=1<<9;//���ͨ��4������ɱ�־
            break; 
        }
    }
    for(i=0;i<10;i++)
    {
        printf("DST_Buffer[%d]==%d\r\n",i,(DST_Buffer[i]-0x30));
    }
    printf("\r\nUART DMA TEST OK!\r\n "); 	    
    
    while(1);
    
}


