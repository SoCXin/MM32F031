#include "HAL_device.h"
#include "HAL_conf.h"
#include "spi.h"
#include "uart.h"	  

unsigned char tmpdata[256];
unsigned char tmpdata1[256];
unsigned char rxtmpdata[256];
volatile unsigned char spi0_rx_flag;
volatile unsigned char spi0_tx_flag;
extern void DataCompare(unsigned char *p1, unsigned char *p2,unsigned int count);
/********************************************************************************************************
**函数信息 ：SPIM_CSLow(SPI_TypeDef* SPIx)                      
**功能描述 :为选定的SPI 软件重置内部NSS 管脚 
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
********************************************************************************************************/
void SPIM_CSLow()
{
    GPIO_ResetBits( GPIOB, GPIO_Pin_12 );
}

/********************************************************************************************************
**函数信息 ：SPIM_CSHigh(SPI_TypeDef* SPIx)                      
**功能描述 :为选定的SPI 软件配置内部NSS 管脚 
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
********************************************************************************************************/
void SPIM_CSHigh()
{
    //Spi cs release
    GPIO_SetBits( GPIOB, GPIO_Pin_12 );
}
/********************************************************************************************************
**函数信息 ：SPIM_TXEn()                     
**功能描述 :关闭 SPI 在双向模式下的数据传输方向 
**输入参数 : 无
**输出参数 ：无
********************************************************************************************************/
void SPIM_TXEn()
{
    //Transmit Enable bit TXEN
    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx);
}

/********************************************************************************************************
**函数信息 ：SPIM_TXDisable()                     
**功能描述 :关闭 SPI 在双向模式下的数据传输方向 
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_TXDisable()
{
    //disable TXEN
    SPI_BiDirectionalLineConfig(SPI2, SPI_Disable_Tx);
}

/********************************************************************************************************
**函数信息 ：SPIM_RXEn()                     
**功能描述 :关闭 SPI 在双向模式下的数据传输方向 
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_RXEn()
{
    //enable RXEN
    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Rx);
}

/********************************************************************************************************
**函数信息 ：SPIM_RXDisable()                     
**功能描述 :关闭 SPI 在双向模式下的数据传输方向 
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_RXDisable()
{
    //disable RXEN
    SPI_BiDirectionalLineConfig(SPI2, SPI_Disable_Rx);
}


/********************************************************************************************************
**函数信息 ：SPIMReadWriteByte(unsigned char tx_data)                    
**功能描述 : 通过外设 SPIx 收发数据 ,用于全双工模式(同时收发)
**输入参数 ：无
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
**函数信息 ：SPIM_Init(unsigned short spi_baud_div)            
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
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;   		//spi2_cs  pb12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	// 推免复用输出
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
    
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_0);//需要将SPI的其他几个IO口复用成任意别的（默认是AF0）
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_0);
    
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;    			// mode0 SPI_CPOL_Low, SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;  			// mode3 SPI_CPOL_High,SPI_CPHA_2Edge
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = spi_baud_div;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI2, &SPI_InitStructure);
    SPIM_TXEn();
    SPIM_RXEn();	
    SPI_Cmd(SPI2, ENABLE); //Enables the specified SPI peripheral SPI使能、主机模式 8位数据模式 SPI 的波特率
    
}

/********************************************************************************************************
**函数信息 ：SPIM_ReadID(SPI_TypeDef* SPIx)                   
**功能描述 :读取ID 
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI1,SPI2
**输出参数 ：无
********************************************************************************************************/
void SPIM_ReadID()
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
**函数信息 ：SPIM_WriteEnable()                  
**功能描述 :写数据使能
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_WriteEnable()
{
    SPIM_CSLow(); 								//Spi cs assign to this pin,select
    SPIMReadWriteByte(WREN);	
    SPIM_CSHigh(); 								//Spi cs release
}

/********************************************************************************************************
**函数信息 ：SSPIM_checkStatus()                 
**功能描述 :检查数据发送是否正确
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_checkStatus()
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
**函数信息 ：SPIM_WriteEnable()                  
**功能描述 :写数据失能
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_WriteDisable()
{	
    SPIM_CSLow();
    SPIMReadWriteByte(WRDI);
    SPIM_CSHigh();
}

/********************************************************************************************************
**函数信息 ：SPIM_PageRead(unsigned long address,unsigned char *p,unsigned int number)               
**功能描述 :读取256页数据
**输入参数 ： address  ;*p ;number
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
    
    for(i=0;i<256;i++)
    {
        rxtmpdata[i] = SPIMReadWriteByte(0x00);
    }
    
    
    SPIM_CSHigh();  							//Spi cs release
}

/********************************************************************************************************
**函数信息 ：SPIM_PageProgram(unsigned long address,unsigned char *p,unsigned int number)              
**功能描述 :发送256页数据
**输入参数 ： address;*p;number
**输出参数 ：无
********************************************************************************************************/
void SPIM_PageProgram(unsigned long address,unsigned char *p,unsigned int number)
{
    unsigned int j;
    unsigned char addr0,addr1,addr2;
    address = address&0xffffff00; 					//page address
    addr0 = (unsigned char)(address>>16);
    addr1 = (unsigned char)(address>>8);
    addr2 = (unsigned char)address;
    
    SPIM_WriteEnable();
    SPIM_CSLow();  								//Spi cs assign to this pin,select
    SPIMReadWriteByte(PP);
    SPIMReadWriteByte(addr0);
    SPIMReadWriteByte(addr1);
    SPIMReadWriteByte(addr2);
    for(j=0;j<number;j++)
    {
        SPIMReadWriteByte(*(p++));
    }
    
    SPIM_CSHigh();  							//Spi cs release
    
    SPIM_checkStatus();
}

/********************************************************************************************************
**函数信息 ：SPIM_SectorErase()             
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
**函数信息 ：SPIM_BlockErase()             
**功能描述 :擦除数据
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_BlockErase()
{
    SPIM_WriteEnable();
    
    SPIM_CSLow();  								//Spi cs assign to this pin,select
    
    SPIMReadWriteByte(BE);	
    
    SPIM_CSHigh();  							//Spi cs release
    
    SPIM_checkStatus();
}
/********************************************************************************************************
**函数信息 ：SPIM_Close()             
**功能描述 : CS信号拉高，停止通信
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_Close()
{
    SPIM_CSHigh();  							//Spi cs release
    SPIMReadWriteByte(0x01);
}

/********************************************************************************************************
**函数信息 ：SPIM_Test()          
**功能描述 :测试程序，使用串口打印256页数据
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_Test(void)
{
    unsigned int i;
    for(i=0;i<256;i++)
    {
        tmpdata[i] = i*2;
    }
    
    printf("SPI2 test\r\n");
    SPIM_Init(0x8);//波特率 72M/2=36M
    
    SPIM_ReadID();
    
    SPIM_SectorErase(0);
    
    SPIM_PageProgram(0,tmpdata,256);
    
    for(i=0;i<256;i++)
    {
        rxtmpdata[i] = 0x0;
    }
    SPIM_PageRead(0,rxtmpdata,256); 		//把写进去的一页256字节读出来
    
    for(i=0;i<10;i++)
    {
        printf("rx[%d]=0x%x\r\n",i,rxtmpdata[i]);
    }
    
    printf("SPI2 test over\r\n");
    
}

