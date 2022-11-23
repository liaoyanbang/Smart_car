/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		isr
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC16F40K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/

#ifndef __ISR_H_
#define __ISR_H_


extern short GoalPulseL; 				  // 定义左电机全局输入量
extern short GoalPulseR;						// 定义右电机全局输入量
extern short flag_star;

extern short PulseLL;

extern short PulseL ;          // 定义左编码器全局变量 
extern short PulseR ;          // 定义右编码器全局变量

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