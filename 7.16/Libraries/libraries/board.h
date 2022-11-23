/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		board
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC16F40K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/



#ifndef __BOARD_H
#define __BOARD_H
#include "common.h"

//FOSC��ѡֵ:30000000, 27000000. 24000000, 22118400, 20000000, 18432000��0��
//FOSCֵ��Ϊ0������STC-ISP����ʱ�������Ƶ���Ƕ��٣�����ͨ���޸ļĴ����ķ�ʽǿ�������ں�Ƶ��ΪFOSC���⼸����ѡֵ��
//FOSCֵΪ0���Ͳ���ͨ���޸ļĴ����ķ�ʽ��ǿ���޸��ں�Ƶ�ʣ���ʱ���ں�Ƶ��ΪSTC-ISP�����ṩ���ں�Ƶ�ʣ�ʹ�ô˷�����Ҫ�ֶ��޸�board.c�е�sys_clk��ֵ��STC-ISPƵ������һ����

#define FOSC		30000000				//�ں�Ƶ������Ϊ30Mhz

#define EXTERNAL_CRYSTA_ENABLE 	0			//ʹ���ⲿ����0Ϊ��ʹ�ã�1Ϊʹ�ã�����ʹ���ڲ�����
#define PRINTF_ENABLE			1			//printfʹ�ܣ�0Ϊʧ�ܣ�1Ϊʹ��
#define ENABLE_IAP 				1			//ʹ�����һ�����ع��ܣ�0Ϊʧ�ܣ�1Ϊʹ��

#define DEBUG_UART 			  	UART_1
#define DEBUG_UART_BAUD 	  	115200
#define DEBUG_UART_RX_PIN  		UART1_RX_P30
#define DEBUG_UART_TX_PIN  		UART1_TX_P31
#define DEBUG_UART_TIM			TIM_2

#if (1==PRINTF_ENABLE)
	char putchar(char c);
#endif

#define SET_P54_RESRT 	  (RSTCFG |= 1<<4)	//����P54Ϊ��λ����

extern int32 sys_clk;

void board_init(void);
void DisableGlobalIRQ(void);
void EnableGlobalIRQ(void);




#endif

