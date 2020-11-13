#include "iwdg.h" 

/********************************************************************************************************
**函数信息 ：Write_Iwdg_PR(void)             
**功能描述 ：启动独立看门狗
**输入参数 ：IWDG_Prescaler 可选IWDG_Prescaler_X, X为4,8,16,32,64,128,256,对应分频值与X取值相同
Reload<=0xfff,为计数器重载值
**输出参数 ：无
**    备注 ：复位时常计算已LSI 40KHz为参考
Tiwdg=(X/LSI)*Reload
********************************************************************************************************/
void Write_Iwdg_ON(unsigned short int IWDG_Prescaler,unsigned short int Reload)
{
    /*启动内部低速时钟,等待时钟就绪*/
    RCC->CSR|=RCC_CSR_LSION;
    
    /*设置时钟预分频*/	
    IWDG->KR=0X5555;
    IWDG->PR=IWDG_Prescaler;
    
    /*设置重载寄存器值*/	
    //RVU_CheckStatus();
    IWDG->KR=0X5555;
    IWDG->RLR = (Reload&0xfff);
    /*装载并使能计数器*/	
    IWDG->KR=0XAAAA;
    IWDG->KR=0XCCCC;
}

//喂独立看门狗
void Write_Iwdg_RL(void)
{
    IWDG->KR=0XAAAA;//reload											   
}
