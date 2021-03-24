#include "delay.h"
#include "sys.h"
#include "uart_nvic.h"
#include "led.h"

int main(void)
{
    u8 t;
    u8 len;	
    u16 times=0; 
    
    delay_init();
    LED_Init();
    uart_nvic_init(115200);	 //串口初始化为115200
    while(1)
    {
        if(UART_RX_STA&0x8000)
        {					   
            len=UART_RX_STA&0x3fff;//得到此次接收到的数据长度
            printf("您发送的消息为:\r\n");
            for(t=0;t<len;t++)
            {
                while((UART1->CSR&0x1)==0);//等待发送结束
                UART1->TDR=UART_RX_BUF[t];    
            }
            printf("\r\n\r\n");//插入换行
            UART_RX_STA=0;
        }else
        {
            times++;
            if(times%5000==0)
            {
                printf("\r\nMini Board 串口实验\r\n");
            }
            if(times%200==0)printf("请输入数据,以回车键结束\r\n");  
            if(times%30==0)LED1_TOGGLE();//闪烁LED,提示系统正在运行.
            delay_ms(10);   
        }
    }	 
}




