#include "uart.h"

volatile unsigned char rx_flag = 0;
volatile unsigned char tx_flag = 0;
extern uint16_t I2C_DMA_DIR;
unsigned char tx_buffer0[16] = {0xa,0xb,0xc,0xd,0xe,0xf,1,2,3,4,5,6,7,8,9,0};
unsigned char rx_buffer0[16] ;
extern void I2C_ReadCmd(I2C_TypeDef* I2Cx);
#define FLASH_DEVICE_ADDR 0xa0
/********************************************************************************************************
**������Ϣ ��I2CInitMasterMode(unsigned long apb_mhz,unsigned long i2c_baud_rate) //unit is Khz                    
**�������� ����ʼ��I2C
**������� ����
**������� ����
********************************************************************************************************/
void I2CInitMasterMode() 
{
    I2C_InitTypeDef I2C_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  //����GPIOA,GPIOBʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);  
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9;  //I2C1��ӳ��IO�� 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				//�������߿��м�CLK&DATAΪ��
    GPIO_Init(GPIOB, &GPIO_InitStructure);	
    
    I2C_InitStructure.I2C_Mode = I2C_Mode_MASTER;//��ģʽ
    I2C_InitStructure.I2C_OwnAddress = FLASH_DEVICE_ADDR;
    I2C_InitStructure.I2C_Speed = I2C_Speed_STANDARD;
    I2C_InitStructure.I2C_ClockSpeed = 100000;
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;				//��Ҫ�������
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_1);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_1);
    
}


/********************************************************************************************************
**������Ϣ ��I2CSetDeviceAddr(I2C_TypeDef *I2Cx,unsigned char deviceaddr)          
**�������� ��ѡ��ӻ���ַ
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2;deviceaddr:�ӻ���ַ
**������� ����
********************************************************************************************************/
void I2CSetDeviceAddr(unsigned char deviceaddr)  
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9;  //I2C1��ӳ��IO�� 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);	
    
    I2C_Cmd(I2C1,DISABLE);
    I2C_Send7bitAddress(I2C1, deviceaddr , I2C_Direction_Transmitter);	
    I2C_Cmd(I2C1, ENABLE);
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/********************************************************************************************************
**������Ϣ ��I2CTXEmptyCheck(I2C_TypeDef *I2Cx)                 
**�������� ����鷢���жϱ�־λ
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2
**������� ����
********************************************************************************************************/
void I2CTXEmptyCheck(I2C_TypeDef *I2Cx)
{
    while(1)
    {
        if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TX_EMPTY))
        {
            break;
        }
    }
}
/********************************************************************************************************
**������Ϣ ��I2CTXEmptyCheck(I2C_TypeDef *I2Cx)                 
**�������� ���������жϱ�־λ
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2
**������� ����
********************************************************************************************************/
void I2CRXFullCheck(I2C_TypeDef *I2Cx)
{
    
    while(1)
    {
        if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RX_FULL))
        {
            break;
        }	
    }
}
/********************************************************************************************************
**������Ϣ ��I2CTXByte(I2C_TypeDef *I2Cx,unsigned short cmd,unsigned char temp)              
**�������� ��I2C��������
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2��cmd��temp
**������� ����
********************************************************************************************************/
void I2CTXByte(I2C_TypeDef *I2Cx,unsigned short cmd,unsigned char temp)
{
    I2C_SendData(I2Cx, temp);
    I2CTXEmptyCheck(I2Cx);
    
}
/********************************************************************************************************
**������Ϣ ��I2CRXByte(I2C_TypeDef *I2Cx)             
**�������� ��I2C��������
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2
**������� ��temp
********************************************************************************************************/
unsigned char I2CRXByte(I2C_TypeDef *I2Cx)
{
    unsigned short temp;
    
    I2CRXFullCheck(I2Cx);
    
    temp = I2C_ReceiveData(I2Cx);
    return (unsigned char)temp;
}


void I2C1_DMA_TX(unsigned char *tx_buf,unsigned short size)
{
    DMA_InitTypeDef DMA_InitStruct;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
    DMA_DeInit(DMA1_Channel2);	
    
    /* Initialize the DMA_PeripheralBaseAddr member */
    DMA_InitStruct.DMA_PeripheralBaseAddr = I2C1_BASE+0x10;//I2C���շ����ݵ�ַ
    /* Initialize the DMA_MemoryBaseAddr member */
    DMA_InitStruct.DMA_MemoryBaseAddr = (unsigned int)tx_buf;
    /* Initialize the DMA_DIR member */
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
    /* Initialize the DMA_BufferSize member */
    DMA_InitStruct.DMA_BufferSize = size;
    /* Initialize the DMA_PeripheralInc member */
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    /* Initialize the DMA_MemoryInc member */
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable ;
    /* Initialize the DMA_PeripheralDataSize member */
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    /* Initialize the DMA_MemoryDataSize member */
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    /* Initialize the DMA_Mode member */
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    /* Initialize the DMA_Priority member */
    DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
    /* Initialize the DMA_M2M member */
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel2, &DMA_InitStruct);//TX
    DMA_Cmd(DMA1_Channel2, ENABLE);
    
    I2C1->IC_DMA_CR |= TDMAE_SET;//ʹ��DMA����
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC2));//�ȴ�DMA�������
    DMA_Cmd(DMA1_Channel2, DISABLE);
    I2C1->IC_DMA_CR &= ~TDMAE_SET;//�ر�DMA����
}



/*-------------------DMA----------------------------------------------
----------------------------------------------------------------------*/
/*-------------------DMA----------------------------------------------
----------------------------------------------------------------------*/

void DMA_I2C1MasterWrite(unsigned char device_id, unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data )
{
    
    I2C1_DMA_TX((u8*)&mem_byte_addr,1);
    I2C1_DMA_TX(tx_data,tx_count);
    //  while(!I2C_GetFlagStatus(I2C1,I2C_STATUS_FLAG_TFE));
    while(!(I2C1->IC_STATUS&0x4));
    I2C_GenerateSTOP( I2C1, ENABLE ); 
}
//*******************************************************************************************************
//������Ϣ ��I2C1_DMA_RX  
//�������� ��
//������� ��
//������� ����
//******************************************************************************************************/
void I2C1_DMA_RX(unsigned char *rx_buf,unsigned short size)
{
    
    unsigned int ui_Tdata = 0x100;
    DMA_InitTypeDef DMA_InitStruct;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
    
    I2C1->IC_DMA_CR |= RDMAE_SET|TDMAE_SET;//ʹ��DMA�շ�
    
    DMA_DeInit(DMA1_Channel3);	 
    
    /* Initialize the DMA_PeripheralBaseAddr member */
    DMA_InitStruct.DMA_PeripheralBaseAddr = I2C1_BASE+0x10;
    /* Initialize the DMA_MemoryBaseAddr member */
    DMA_InitStruct.DMA_MemoryBaseAddr = (unsigned int)rx_buf;
    /* Initialize the DMA_DIR member */
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
    /* Initialize the DMA_BufferSize member */
    DMA_InitStruct.DMA_BufferSize = size;
    /* Initialize the DMA_PeripheralInc member */
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    /* Initialize the DMA_MemoryInc member */
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable ;
    /* Initialize the DMA_PeripheralDataSize member */
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    /* Initialize the DMA_MemoryDataSize member */
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    /* Initialize the DMA_Mode member */
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    /* Initialize the DMA_Priority member */
    DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
    /* Initialize the DMA_M2M member */
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel3, &DMA_InitStruct);//RX
    DMA_Cmd(DMA1_Channel3, ENABLE);
    
    DMA_DeInit(DMA1_Channel2);	
    
    /* Initialize the DMA_PeripheralBaseAddr member *///����DMA���͸���I2C������
    DMA_InitStruct.DMA_PeripheralBaseAddr = I2C1_BASE+0x10;
    /* Initialize the DMA_MemoryBaseAddr member */
    DMA_InitStruct.DMA_MemoryBaseAddr = (unsigned int)&ui_Tdata;
    /* Initialize the DMA_DIR member */
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
    /* Initialize the DMA_BufferSize member */
    DMA_InitStruct.DMA_BufferSize = size;
    /* Initialize the DMA_PeripheralInc member */
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    /* Initialize the DMA_MemoryInc member */
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable  ;
    /* Initialize the DMA_PeripheralDataSize member */
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    /* Initialize the DMA_MemoryDataSize member */
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    /* Initialize the DMA_Mode member */
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    /* Initialize the DMA_Priority member */
    DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
    /* Initialize the DMA_M2M member */
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel2, &DMA_InitStruct);//TX
    DMA_Cmd(DMA1_Channel2, ENABLE);
    
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC3))//�ȴ�DMA�������,Ҳ�����ж�ʵ��
    {
        //I2C1->IC_DATA_CMD = 0x100;
    }
    DMA_Cmd(DMA1_Channel3, DISABLE);
    DMA_Cmd(DMA1_Channel2, DISABLE);
    I2C1->IC_DMA_CR &= ~(RDMAE_SET|TDMAE_SET);//�ر�I2C_DMA����
}

void DMA_I2C1MasterRead(unsigned char device_id, unsigned short mem_byte_addr, unsigned short rx_count, unsigned char *rx_data )
{
    
    I2C1_DMA_TX((u8*)&mem_byte_addr,1);
    I2C1_DMA_RX(rx_data,rx_count);
    while(!(I2C1->IC_STATUS&0x4));
    I2C_GenerateSTOP( I2C1, ENABLE ); 
}

void I2C1MasterDMATest()
{
    unsigned int i;
    I2C_DeInit(I2C1);
    printf("i2c1 dma test start\r\n");
    
    I2CInitMasterMode();
    DMA_I2C1MasterWrite(FLASH_DEVICE_ADDR, 16*0, 16, tx_buffer0 );
    i = 1000000;while(i--);//���������ʱ
    for(i = 0; i < 16 ;i ++)
    {
        printf("TX data is: %x \r\n", tx_buffer0[i]);
    }
    DMA_I2C1MasterRead(FLASH_DEVICE_ADDR, 16*0, 16, rx_buffer0 );
    
    for(i=0;i<16;i++)
    {
        printf("RX data%d is  : %x \r\n",i,rx_buffer0[i]);
    }
    printf("i2c1 dma test over\r\n");
}

