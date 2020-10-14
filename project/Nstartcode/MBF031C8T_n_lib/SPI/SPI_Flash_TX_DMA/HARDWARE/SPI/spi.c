#include "HAL_device.h"
#include "HAL_spi.h"
#include "spi.h"
#include "uart.h"	  

unsigned char tmpdata[256];
unsigned char tmpdata1[256];
unsigned char rxtmpdata[256];
volatile unsigned char spi0_rx_flag;
volatile unsigned char spi0_tx_flag;
extern void DataCompare(unsigned char *p1, unsigned char *p2,unsigned int count);
/********************************************************************************************************
**函数信息 ：SPIM_CSLow(void)                      
**功能描述 :为选定的SPI 软件重置内部NSS 管脚 
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_CSLow(void)
{
    //Spi cs assign to this pin,select	
    SPI_CSInternalSelected(SPI2, SPI_CS_BIT0,ENABLE);
}

/********************************************************************************************************
**函数信息 ：SPIM_CSHigh(void)                      
**功能描述 :为选定的SPI 软件配置内部NSS 管脚 
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_CSHigh(void)
{
    //Spi cs release
    SPI_CSInternalSelected(SPI2, SPI_CS_BIT0,DISABLE);
}

/********************************************************************************************************
**函数信息 ：SPIM_TXEn(void)                     
**功能描述 :关闭 SPI 在双向模式下的数据传输方向 
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_TXEn(void)
{
    //Transmit Enable bit TXEN
    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx);
}

/********************************************************************************************************
**函数信息 ：SPIM_TXDisable(void)                     
**功能描述 :关闭 SPI 在双向模式下的数据传输方向 
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_TXDisable(void)
{
    //disable TXEN
    SPI_BiDirectionalLineConfig(SPI2, SPI_Disable_Tx);
}

/********************************************************************************************************
**函数信息 ：SPIM_RXEn(void)                     
**功能描述 :关闭 SPI 在双向模式下的数据传输方向 
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_RXEn(void)
{
    //enable RXEN
    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Rx);
}

/********************************************************************************************************
**函数信息 ：SPIM_RXDisable(void)                     
**功能描述 :关闭 SPI 在双向模式下的数据传输方向 
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_RXDisable(void)
{
    //disable RXEN
    SPI_BiDirectionalLineConfig(SPI2, SPI_Disable_Rx);
}


/********************************************************************************************************
**函数信息 ：SPIMReadWriteByte(unsigned char tx_data)                    
**功能描述 : 通过外设 SPIx 收发数据 ,用于全双工模式(同时收发)
**输入参数 ：tx_data
**输出参数 ：无
********************************************************************************************************/
unsigned int SPIMReadWriteByte(unsigned char tx_data)
{
    SPI_SendData(SPI2, tx_data);	
    while (1)
    {
        if(SPI_GetFlagStatus(SPI2, SPI_FLAG_RXAVL))	
        {
            return SPI_ReceiveData(SPI2);
        }
    }
}

/********************************************************************************************************
**函数信息 ：SPIM_Init( unsigned short spi_baud_div)            
**功能描述 :可修改参数初始化SPI
**输入参数 ：spi_baud_div
**输出参数 ：无
********************************************************************************************************/
void SPIM_Init(unsigned short spi_baud_div)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);  //SPI2 clk enable
    SPIM_CSHigh();
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  //开启GPIOA,GPIOB时钟
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;   		//spi2_cs  pb12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// 推免复用输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;   		//spi2_sck  pb13
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// 推免复用输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;   		//spi2_mosi  pb15
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// 推免复用输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;  		//spi2_miso  pb14
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//上拉输入   
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_0);
    
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;   //DMA时用到
    SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;    			// mode0 SPI_CPOL_Low, SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;  			// mode3 SPI_CPOL_High,SPI_CPHA_2Edge
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = spi_baud_div;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI2, &SPI_InitStructure);
    
    SPIM_TXEn();
    SPIM_RXEn();	
    SPI_Cmd(SPI2, ENABLE); 									//Enables the specified SPI peripheral SPI使能、主机模式 8位数据模式 SPI 的波特率
    
}

/********************************************************************************************************
**函数信息 ：SPIM_ReadID(void)                   
**功能描述 :读取ID 
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_ReadID(void)
{
    unsigned char temp;
    unsigned int i;
    
    SPIM_CSLow();								//Spi cs assign to this pin,select
    SPIMReadWriteByte(RDID);				
    
    for(i=0;i<3;i++)
    {
        temp = SPIMReadWriteByte(0x01);
        printf("temp=0x%x\r\n",temp);		
    }
    SPIM_CSHigh();   							//Spi cs release
}

/********************************************************************************************************
**函数信息 ：SPIM_WriteEnable(void)                  
**功能描述 :写数据使能
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_WriteEnable(void)
{
    SPIM_CSLow(); 								//Spi cs assign to this pin,select
    SPIMReadWriteByte(WREN);	
    SPIM_CSHigh(); 								//Spi cs release
}

/********************************************************************************************************
**函数信息 ：SSPIM_checkStatus(void)                 
**功能描述 :检查数据发送是否正确
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_checkStatus(void)
{
    unsigned char temp;
    SPIM_CSLow(); 								//Spi cs assign to this pin,select
    SPIMReadWriteByte(RDSR);	
    while(1)
    {
        temp = SPIMReadWriteByte(0x00);	
        if((temp&0x01)==0x0)
            break;
    }
    SPIM_CSHigh();  							//Spi cs release
}

/********************************************************************************************************
**函数信息 ：SPIM_WriteEnable(void)                  
**功能描述 :写数据失能
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_WriteDisable(void)
{	
    SPIM_CSLow();
    SPIMReadWriteByte(WRDI);
    SPIM_CSHigh();
}

/********************************************************************************************************
**函数信息 ：SPIM_PageRead(unsigned long address,unsigned char *p,unsigned int number)               
**功能描述 :读取256页数据
**输入参数 ：address  ;*p ;number
**输出参数 ：无
********************************************************************************************************/
void SPIM_PageRead(unsigned long address,unsigned char *p,unsigned int number)  //page = 256 bytes
{
    unsigned char addr0,addr1,addr2;
    unsigned int i;
    address = address&0xffffff00; 					//page address
    addr0 = (unsigned char)(address>>16);
    addr1 = (unsigned char)(address>>8);
    addr2 = (unsigned char)address;
    
    SPIM_CSLow(); 								//Spi cs assign to this pin,select
    
    SPIMReadWriteByte(READ);
    SPIMReadWriteByte(addr0);
    SPIMReadWriteByte(addr1);
    SPIMReadWriteByte(addr2);
    
    for(i=0;i<7;i++)//过滤掉前面7个错误数据
    {
        SPIMReadWriteByte(0);
    }
    for(i=0;i<256;i++)
    {
        rxtmpdata[i] = SPIMReadWriteByte(0x00);
    }
    
    
    SPIM_CSHigh();  							//Spi cs release
}

/********************************************************************************************************
**函数信息 ：SPIM_PageProgram(unsigned long address,unsigned char *p,unsigned int number)              
**功能描述 :发送256页数据
**输入参数 : address;*p;number
**输出参数 ：无
********************************************************************************************************/
void SPIM_PageProgram(unsigned long address,unsigned char *p,unsigned int number)
{
    u32 i;
    DMA_InitTypeDef  DMA_InitStructure;
    
    unsigned char addr0,addr1,addr2;
    address = address&0xffffff00; 					//page address
    addr0 = (unsigned char)(address>>16);
    addr1 = (unsigned char)(address>>8);
    addr2 = (unsigned char)address;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
    DMA_DeInit(DMA1_Channel5);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(SPI2->TXREG);
    DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)(p);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = number;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);//SPI TX为DMA通道15
    
    SPIM_WriteEnable();
    SPIM_CSLow();  								//Spi cs assign to this pin,select
    SPIMReadWriteByte(PP);
    
    SPIMReadWriteByte(addr0);
    
    SPIMReadWriteByte(addr1);
    
    SPIMReadWriteByte(addr2);
    
    DMA_Cmd(DMA1_Channel5, ENABLE);  //使能UART1 TX DMA1 所指示的通道 
    
    SPI_DMACmd(SPI2,SPI_DMAReq_EN,ENABLE);
    
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC5));
    
    DMA_ClearFlag(DMA1_FLAG_TC5);
    
    SPI_DMACmd(SPI2,SPI_DMAReq_EN,DISABLE);
    for(i=0;i<10000;i++);
    DMA_Cmd(DMA1_Channel5, DISABLE);  //使能UART1 TX DMA1 所指示的通道 
    for(i=0;i<10000;i++);
    
    SPIM_CSHigh();  							//Spi cs release
    
    SPIM_checkStatus();
}

/********************************************************************************************************
**函数信息 ：SPIM_SectorErase(unsigned long address)             
**功能描述 :擦除数据
**输入参数 ：unsigned long address,删除指定sector的地址 each sector = 64Kbytes
**输出参数 ：无
********************************************************************************************************/
void SPIM_SectorErase(unsigned long address)
{
    unsigned char addr0,addr1,addr2;
    address = address & 0xffff0000;
    addr0 = ((unsigned char)(address>>16))&0xff;
    addr1 = ((unsigned char)(address>>8))&0xff;
    addr2 = ((unsigned char)address)&0xff;
    
    SPIM_WriteEnable();
    
    SPIM_CSLow();  								//Spi cs assign to this pin,select
    
    SPIMReadWriteByte(SE);
    SPIMReadWriteByte(addr0);
    SPIMReadWriteByte(addr1);
    SPIMReadWriteByte(addr2);
    SPIM_CSHigh();  							//Spi cs release
    
    SPIM_checkStatus();
}

/********************************************************************************************************
**函数信息 ：SPIM_BlockErase(void)             
**功能描述 :擦除数据
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_BlockErase(void)
{
    SPIM_WriteEnable();
    
    SPIM_CSLow();  								//Spi cs assign to this pin,select
    
    SPIMReadWriteByte(BE);	
    
    SPIM_CSHigh();  							//Spi cs release
    
    SPIM_checkStatus();
}


/********************************************************************************************************
**函数信息 ：SPIM_Close(void)             
**功能描述 : CS信号拉高，停止通信
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_Close(void)
{
    SPIM_CSHigh();  							//Spi cs release
    SPIMReadWriteByte(0x01);
    
}

/********************************************************************************************************
**函数信息 ：SPIM_Test(void)          
**功能描述 :测试程序，使用串口打印256页数据
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_Test(void)
{
    unsigned int i;
    for(i=0;i<256;i++)
    {
        tmpdata[i] = i;
    }
    
    printf("SPI2 test\r\n");
    SPIM_Init(0x8);//波特率 72M/2=36M
    
    SPIM_ReadID();
    
    SPIM_SectorErase(0);
    
    SPIM_PageProgram(0,tmpdata,256);
    
    for(i=0;i<50000;i++);//延时不能去掉，SPI FLASH可能来不及响应
    
    for(i=0;i<256;i++)
    {
        rxtmpdata[i] = 0x0;
    }
    SPIM_PageRead(0,rxtmpdata,256); 		//把写进去的一页256字节读出来
    
    for(i=0;i<20;i++)
    {
        printf("rx[%d]=0x%x\r\n",i,rxtmpdata[i]);
    }
    
    printf("SPI2 test over\r\n");
    
    
}

