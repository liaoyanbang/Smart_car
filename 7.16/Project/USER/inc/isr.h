/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		isr
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC16F40K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/

#ifndef __ISR_H_
#define __ISR_H_
#include "common.h"




#define Servo_Left_Max  745//715
#define Servo_Middle    668//665
#define Servo_Right_Min 590//570


enum position
{
	Normal = 1,
	Fork_L,
	Fork_R,
	Round,
	Cross,
	Descent
};

enum normal_process
{
	Normal_R,
	Normal_L,
	Normal_ing
};

enum fork_process
{
	Forking=1,
	Fork_In,
	Fork_Ining,
	Fork_Inover,
	Forkouting,
	Forkoutover,
	Fork_duixian
};

enum round_process
{
	Round_In=1,
	Round_Ining,
	Round_IningL,//��
	Round_InoverL,
	Round_OutL,
	Round_OutingL,
	
	Round_IningR,//�һ�
	Round_InoverR,
	Round_OutR,
	Round_OutingR,
	
	
	Round_Outover,
};

enum cross_process
{
	Crossing=1,
	Cross_In,
	Cross_duixian,
	Crossover
};

enum descent_process
{
	Descenting=1,
	Descentovering,
	Descentover
};
extern enum position Position;
extern enum normal_process Normal_Process ;
extern enum fork_process Fork_Process;
extern enum round_process Round_Process;
extern enum cross_process Cross_Process;
extern enum descent_process Descent_Process;

extern short GoalPulseLL; 				  // ��������ȫ��������
extern short GoalPulseRR;						// �����ҵ��ȫ��������

extern short PulseL ;          // �����������ȫ�ֱ��� 
extern short PulseR ;          // �����ұ�����ȫ�ֱ���
extern short flag_star ;
extern uint8 dis_flag;
extern uint16 distance;
extern uint16 time_5ms;
extern short Speed_target ;
extern int16 error_dig4;
extern int16 dig4;

extern void TFTLCD_show(void);
float err_get(void);
float err_get_f(float A, float B);
float err_get_b(float A, float B);
void Normal_handle(void);
void Round_handle(void);
void Cross_handle(void);
void Fork_Rhandle(void);
void Fork_Lhandle(void);
void Descent_handle(void);
void Control(void);
void Send_Message(void);
void shibie(void);
void ADV_Init(void);
uint8 key_read(void);
void Turn_control(void);
void show_dirve(void);
void Star_handle(void);//��ʼ����
void sss(void);

//void  INT0_Isr()  interrupt 0;
//void  TM0_Isr()   interrupt 1;
//void  INT1_Isr()  interrupt 2;
//void  TM1_Isr()   interrupt 3;
//void  UART1_Isr() interrupt 4;
//void  ADC_Isr()   interrupt 5;
//void  LVD_Isr()   interrupt 6;
//void  PCA_Isr()   interrupt 7;
//void  UART2_Isr() interrupt 8;
//void  SPI_Isr()   interrupt 9;
//void  INT2_Isr()  interrupt 10;
//void  INT3_Isr()  interrupt 11;
//void  TM2_Isr()   interrupt 12;
//void  INT4_Isr()  interrupt 16;
//void  UART3_Isr() interrupt 17;
//void  UART4_Isr() interrupt 18;
//void  TM3_Isr()   interrupt 19;
//void  TM4_Isr()   interrupt 20;
//void  CMP_Isr()   interrupt 21;
//void  I2C_Isr()   interrupt 24;
//void  USB_Isr()   interrupt 25;
//void  PWM1_Isr()  interrupt 26;
//void  PWM2_Isr()  interrupt 27;
#endif