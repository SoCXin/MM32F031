#ifndef __UART_NVIC_H
#define __UART_NVIC_H
#include "HAL_device.h"
#include  "stdio.h"

//������
//����1��ʼ��		   
#define UART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_UART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
extern u32 SystemCoreClock;

extern u8 UART_RX_BUF[UART_REC_LEN];  //���ջ���,���UART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 UART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_nvic_init(u32 bound);
#endif


