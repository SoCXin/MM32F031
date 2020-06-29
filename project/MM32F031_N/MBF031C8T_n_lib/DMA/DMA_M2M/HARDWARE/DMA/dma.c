#include "HAL_conf.h"
#include "uart.h"
#include "dma.h"

unsigned char dma1Flag = 0x0;
unsigned char dma2Flag = 0x0;
u8  dmaTxDATA[64] ;
u8  dmaRxDATA[64] ;
/***********************	*********************************************************************************
**函数信息 ：DMA1_Channel1_IRQHandler(void)     
**功能描述 : DMA1通道1的中断函数
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void DMA1_Channel1_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC1))
    {
        DMA_ClearITPendingBit(DMA1_IT_TC1);
        dma1Flag = 0x1;
    }
}

/********************************************************************************************************
**函数信息 ：DMA1_Channel2_IRQHandler(void)     
**功能描述 : DMA1通道2的中断函数
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void DMA1_Channel2_3_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC2))
    {
        DMA_ClearITPendingBit(DMA1_IT_TC2);
        dma2Flag = 0x1;
    }
    
}

/********************************************************************************************************
**函数信息 ：DMAcheckStatus(uint32_t DMA_FLAG)     
**功能描述 : 查询DMA的标志位
**输入参数 ：uint32_t DMA_FLAG，DMA的状态标志位
**输出参数 ：无
********************************************************************************************************/
void DMAcheckStatus(uint32_t DMA_FLAG)
{
    while(1)
    {
        if(DMA_GetFlagStatus(DMA_FLAG))
        {
            DMA_ClearFlag(DMA_FLAG);
            break;
        }
    }
}

/********************************************************************************************************
**函数信息 ：DMAdisable(DMA_Channel_TypeDef* DMAy_Channelx)       
**功能描述 : DMA通道失能
**输入参数 ：DMA_Channel_TypeDef* DMAy_Channelx，选择通道
**输出参数 ：无
********************************************************************************************************/
void DMAdisable(DMA_Channel_TypeDef* DMAy_Channelx)
{
    DMA_Cmd(DMAy_Channelx, DISABLE);
}

/********************************************************************************************************
**函数信息 ：DMA_m8tom8_test(void)          
**功能描述 : 把存储器中的某一地址开始的64个8位数据搬到存储器的另一个地址，并以8位方式存储
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void DMA_m8tom8_test(void)
{
    unsigned int i;
    u8 *p;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    p = (u8*)dmaTxDATA;
    for(i=0;i<64;i++)
    {
        *(p + i) = i +1;
    }
    
    DMA_DeInit(DMA1_Channel2);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)dmaRxDATA;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)dmaTxDATA;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);
    
    
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);				
    
    dma2Flag = 0x0;
    
    DMA_Cmd(DMA1_Channel2, ENABLE);
    
    while(1)
    {
        if(dma2Flag)
        {
            dma2Flag = 0x0;
            break;
        }
    }
    
    
    DMA_Cmd(DMA1_Channel2, DISABLE);
    for(i=0;i<64;i++)
    {
        
        temp = dmaRxDATA[i];
        
        printf("temp%d=0x%x\r\n",i,temp);
    }
}

/********************************************************************************************************
**函数信息 ：DMA_m8tom16_test(void)          
**功能描述 : 把存储器中的某一地址开始的64个8位数据搬到存储器的另一个地址，并以16位方式存储
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void DMA_m8tom16_test(void)
{
    unsigned int i;
    u8 *p;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    p = (u8*)dmaTxDATA;
    for(i=0;i<64;i++)
    {
        *(p + i) = i +1;
    }
    
    DMA_DeInit(DMA1_Channel2);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)dmaRxDATA;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)dmaTxDATA;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);				
    dma1Flag = 0x0;
    
    DMA_Cmd(DMA1_Channel1, ENABLE);
    
    while(1)
    {
        if(dma1Flag)
        {
            dma1Flag = 0x0;
            break;
        }
    }
    
    DMA_Cmd(DMA1_Channel1, DISABLE);
    for(i=0;i<128;i++)
    {
        
        temp = dmaRxDATA[i];
        
        printf("temp%d=0x%x\r\n",i,temp);
    }
}

/********************************************************************************************************
**函数信息 ：DMA_m8tom32_test(void)          
**功能描述 : 把存储器中的某一地址开始的64个8位数据搬到存储器的另一个地址，并以32位方式存储
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void DMA_m8tom32_test()
{
    unsigned int i;
    u8 *p;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;
    
    p = (u8*)dmaTxDATA;
    for(i=0;i<64;i++)
    {
        *(p + i) = i +1;
    }
    
    DMA_DeInit(DMA1_Channel2);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)dmaRxDATA;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)dmaTxDATA;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; //DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);
    
    DMA_Cmd(DMA1_Channel3, ENABLE);
    
    DMAcheckStatus(DMA1_FLAG_TC3);
    
    DMA_Cmd(DMA1_Channel3, DISABLE);
    for(i=0;i<64;i++)
    {
        
        temp = dmaRxDATA[i];		
        printf("temp%d=0x%x\r\n",i,temp);
    }
}

/********************************************************************************************************
**函数信息 ：DMA_m16tom8_test(void)          
**功能描述 : 把存储器中的某一地址开始的64个16位数据搬到存储器的另一个地址，并以8位方式存储
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void DMA_m16tom8_test(void)
{
    unsigned int i;
    u8 *p;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;
    
    p = (u8*)dmaTxDATA;
    for(i=0;i<64;i++)
    {
        *(p + i) = i +1;
    }
    
    DMA_DeInit(DMA1_Channel2);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)dmaRxDATA;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)dmaTxDATA;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);
    
    
    DMA_Cmd(DMA1_Channel4, ENABLE);
    
    DMAcheckStatus(DMA1_FLAG_TC4);
    
    DMA_Cmd(DMA1_Channel4, DISABLE);
    for(i=0;i<64;i++)
    {   
        
        temp = dmaRxDATA[i];
        
        printf("temp%d=0x%x\r\n",i,temp);
    }
}

/********************************************************************************************************
**函数信息 ：DMA_m16tom16_test(void)          
**功能描述 : 把存储器中的某一地址开始的64个16位数据搬到存储器的另一个地址，并以16位方式存储
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void DMA_m16tom16_test(void)
{
    unsigned int i;
    u8 *p;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;
    
    p = (u8*)dmaTxDATA;
    for(i=0;i<64;i++)
    {
        *(p + i) = i +1;
    }
    
    DMA_DeInit(DMA1_Channel2);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)dmaRxDATA;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)dmaTxDATA;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
    
    
    DMA_Cmd(DMA1_Channel5, ENABLE);
    
    DMAcheckStatus(DMA1_FLAG_TC5);
    
    DMA_Cmd(DMA1_Channel5, DISABLE);
    for(i=0;i<64;i++)
    {
        
        temp = dmaRxDATA[i];
        
        printf("temp%d=0x%x\r\n",i,temp);
    }
}

/********************************************************************************************************
**函数信息 ：DMA_m16tom32_test(void)          
**功能描述 : 把存储器中的某一地址开始的64个16位数据搬到存储器的另一个地址，并以32位方式存储
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void DMA_m16tom32_test(void)
{
    unsigned int i;
    u8 *p;
    unsigned int temp;
    DMA_InitTypeDef DMA_InitStructure;
    
    p = (u8*)dmaTxDATA;
    for(i=0;i<64;i++)
    {
        *(p + i) = i +1;
    }
    
    DMA_DeInit(DMA1_Channel2);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)dmaRxDATA;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)dmaTxDATA;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; 
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);
    
    DMA_Cmd(DMA1_Channel5, ENABLE);
    
    DMAcheckStatus(DMA1_FLAG_TC5);
    
    DMA_Cmd(DMA1_Channel5, DISABLE);
    for(i=0;i<64;i++)
    {
        
        temp = dmaRxDATA[i];
        
        printf("temp%d=0x%x\r\n",i,temp);
    }
}

