#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "led.h"
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
    printf("\r\nDMA SEND DATA:\r\n"); 	  
    MYDMA_Config(DMA1_Channel2,(u32)&UART1->TDR,(u32)SendBuff,(TEXT_LENTH+2)*10);//DMA1ͨ��2,����Ϊ����1,�洢��ΪSendBuff,��(TEXT_LENTH+2)*10.
    for(i=0;i<(TEXT_LENTH+2)*100;i++)//���ASCII�ַ�������
    {
        if(t>=TEXT_LENTH)//���뻻�з�
        { 
            SendBuff[i++]=0x0d; 
            SendBuff[i]=0x0a; 
            t=0;
        }else SendBuff[i]=TEXT_TO_SEND[t++];//����TEXT_TO_SEND���    
    }		 
    
    UART_DMACmd(UART1,UART_DMAReq_EN,ENABLE); //ʹ��uart1 DMA
    MYDMA_Enable(DMA1_Channel2);//��ʼһ��DMA���䣡	  
    //�ȴ�DMA������ɣ���ʱ������������һЩ�£����
    //ʵ��Ӧ���У����������ڼ䣬����ִ�����������
    while(1)
    {
        if(DMA_GetFlagStatus(DMA1_FLAG_TC2)!=RESET)//�ȴ�ͨ��2�������
        {
            DMA_ClearFlag(DMA1_FLAG_TC2);//���ͨ��2������ɱ�־
            break; 
        }
    }
    
    printf("\r\nUART DMA TEST OK!\r\n "); 	    
    
    while(1);
    
}



