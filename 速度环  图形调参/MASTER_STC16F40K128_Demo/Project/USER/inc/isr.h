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


extern short GoalPulseL; 				  // ��������ȫ��������
extern short GoalPulseR;						// �����ҵ��ȫ��������
extern short flag_star;

extern short PulseLL;

extern short PulseL ;          // �����������ȫ�ֱ��� 
extern short PulseR ;          // �����ұ�����ȫ�ֱ���

void TFTLCD_show(void);

void Normal_handle(void);
void Round_Rhandle(void);
void Round_Lhandle(void);
void Cross_handle(void);
void Fork_Rhandle(void);
void Fork_Lhandle(void);
void Descent_handle(void);
void Control(void);
void Send_Message(void);

void pid_duoji(void);

#endif