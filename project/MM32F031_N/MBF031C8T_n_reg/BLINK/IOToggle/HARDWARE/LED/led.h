#ifndef __LED_H
#define __LED_H	 
#include "HAL_device.h"

#define LED4_ON()  	  GPIOA->BRR=0x8000;// PA15
#define LED4_OFF()    GPIOA->BSRR = 0x8000// PA15
#define LED4_TOGGLE()  (GPIOA->ODR&0x8000)?(GPIOA->BRR=0x8000):(GPIOA->BSRR = 0x8000)


#define LED3_ON()  	GPIOA->BRR=0x0080// PB3
#define LED3_OFF() 	GPIOA->BSRR = 0x0080;// PB3
#define LED3_TOGGLE()  (GPIOA->ODR&0x0080)?(GPIOA->BRR=0x0080):(GPIOA->BSRR = 0x0080)

#define LED2_ON()  	GPIOA->BRR=0x0040// PB4
#define LED2_OFF() 	GPIOA->BSRR = 0x0040// PB4
#define LED2_TOGGLE()  (GPIOA->ODR&0x0040)?(GPIOA->BRR=0x0040):(GPIOA->BSRR = 0x0040)

#define LED1_ON()   GPIOA->BRR=0x0020// PB5
#define LED1_OFF() 	GPIOA->BSRR=0x0020// PB5
#define LED1_TOGGLE()  (GPIOA->ODR&0x0020)?(GPIOA->BRR=0x0020):(GPIOA->BSRR = 0x0020)

void LED_Init(void);//≥ı ºªØ


#endif
