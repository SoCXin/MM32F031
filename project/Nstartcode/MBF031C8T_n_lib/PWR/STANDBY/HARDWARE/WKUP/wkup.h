#ifndef __WKUP_H
#define __WKUP_H	 
#include "dtype.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//������
//�������� ����	   
//////////////////////////////////////////////////////////////////////////////////
					    

	 
unsigned char Check_WKUP(void);  			//���WKUP�ŵ��ź�
void WKUP_Init(void); 			//PA0 WKUP���ѳ�ʼ��
void Sys_Enter_Standby(void);	//ϵͳ�������ģʽ
#endif

