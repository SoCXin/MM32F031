#ifndef __WKUP_H
#define __WKUP_H	 
#include "HAL_device.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//������
//�������� ����	   
//////////////////////////////////////////////////////////////////////////////////
#define GPIO_A 0
#define GPIO_B 1
#define GPIO_C 2
#define GPIO_D 3
#define GPIO_E 4
#define GPIO_F 5
#define GPIO_G 6 
#define FTIR   1  //�½��ش���
#define RTIR   2  //�����ش���
				    
	 
u8 Check_WKUP(void);  			//���WKUP�ŵ��ź�
void WKUP_Init(void); 			//PA0 WKUP���ѳ�ʼ��
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM);
void Sys_Enter_Standby(void);	//ϵͳ�������ģʽ
#endif


