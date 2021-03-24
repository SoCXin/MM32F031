#ifndef __UART_LOOP_H
#define __UART_LOOP_H
#include "HAL_device.h"

extern u32 SystemCoreClock;		   

void UartInit_Loop(void);
void Uart1RxTest(UART_TypeDef* UARTx);
		 				    
#endif
