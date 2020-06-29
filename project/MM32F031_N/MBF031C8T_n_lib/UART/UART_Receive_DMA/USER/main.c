#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "led.h"

u8 DST_Buffer[10]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};//UART DMA���ܵ����ݴ������
/* Private function prototypes -----------------------------------------------*/
void DMA_Configuration(void);

/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� ��������ʹ�ô������ַ���10�����֣�ע�⴮�����ֲ���ѡ��������
**������� ��
**������� ��
********************************************************************************************************/

int main(void)
{
    //ע�⣺���ڵ������ֲ��ܹ�ѡ��������
    u8 i;
    
    delay_init();
    LED_Init();		  	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
    uart_initwBaudRate(115200);	 	//���ڳ�ʼ��Ϊ115200
    
    
    DMA_Configuration();//UART DMA����
    printf("������10������!\r\n");  
    printf("�������ֲ��ܹ�ѡ��������!\r\n");  
    while(1)
    {
        for(i=0;i<10;i++)
        {
            DST_Buffer[i]=0;//������������
        }
        while(!DMA_GetFlagStatus(DMA1_FLAG_TC3));//�ȴ�DMA�������
        DMA_ClearFlag(DMA1_FLAG_TC3);//���DMA������ɱ�־λ
        for(i=0;i<10;i++)
        {
            printf("DST_Buffer[%d]==%d\r\n",i,(DST_Buffer[i]-0x30));
        }
        printf("\r\n");
    }
    
    
}


/********************************************************************************************************
**������Ϣ ��DMA_Configuration                         
**�������� ��UART����DMA����
**������� ����
**������� ����
********************************************************************************************************/
void DMA_Configuration(void)
{
    DMA_InitTypeDef  DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    /* UARTy_DMA1_Channel Config */
    DMA_DeInit(DMA1_Channel3);
    DMA_InitStructure.DMA_PeripheralBaseAddr =(u32) &(UART1->RDR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DST_Buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 10;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ��ģʽ����λ�����Բ�����������
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);
    
    UART_DMACmd(UART1,UART_DMAReq_EN,ENABLE); //����UART1 DMA   
    /* UARTy_DMA1_Channel enable */
    DMA_Cmd(DMA1_Channel3, ENABLE);
}



