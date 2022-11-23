/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC16F40K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/

#include "headfile.h"


/*
 *�����ں�Ƶ�ʵ��趨�����Բ鿴board.h�ļ�
 *��board_init��,�Ѿ���P54��������Ϊ��λ
 *�����Ҫʹ��P54����,������board.c�ļ��е�board_init()������ɾ��SET_P54_RESRT����
 */


//=======�����ƹ�˵��=========
//1��������˸��ʾ��ʾ��ǰΪ�ӻ�ģʽ����δ����
//2�����Ʋ�����ʾ��ǰΪ����ģʽ����δ����
//3�����Ƴ�����ʾ������
//4������������ӳɹ��ˣ�����͸����ʱ�����ƻ���˸

//=========��������==========
//��Ҫ��ӻ��������ײ���
//��Ҫ�ڳ�ʼ������������д���Ӧ��ַ���磺bluetooth_ch9141_init(MASTER_MODE��"80:90:2C:E4:C2:84,000000");
//�鿴ģ���ַ��ʽ
//1.ʹ�����ߵ���	��������ӻ���ʽ���ܲ鿴��
//	��ʼ����bluetooth_ch9141_get_mac_address()������һ���Ӹ��ϵ㣬���е��ϵ��鿴mac_address���飬�õ���17λ�ַ���Ϊ��ַ
//2. ʹ���ֻ�	 �����شӻ������ʹ�ã�
//	�ϵ����к󣬴��ֻ�BLE�����������������ģ������֣������·����ǵ�ַ
//	ģ�������Ϊ��ʼ����bluetooth_ch9141_set_name�����Ĳ���
//	���ʹ���ֻ��鿴������mac��ַ���������ʱ���뽫mac����һ��
//	�����ֻ��鿴����mac��ַΪ61:62:63:64:65:67����ʹ�ñ��������ӵ�ʱ��Ӧ��д
//bluetooth_ch9141_init(MASTER_MODE��"67:65:64:63:62:61,000000");
//��ַ��д��ɺ����д��룬����ӻ������У��������Զ����Ӵӻ�

//��������Ч����
//���������������ݣ��ӻ����յ����ݺ�Ὣ���ݷ��ͻ������������յ��ӻ����ص����ݺ�������ʾ����Ļ��
uint8 test1[] = "1111111111\r\n";
uint8 test2[] = "222222222222\r\n";
uint8 send_value = 'a';
uint8 str[30];
void OLED_show(void);
#define LED P52
int main(void)
{
    DisableGlobalIRQ();
    board_init();   //��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���
	
		lcd_init_simspi();//��ʼ��TFT��
		
		pit_timer_ms(TIM_1,20);//��ʼ����ʱ��1 5ms  Control �ٶȻ�
//		pit_timer_ms(TIM_2,100);//��ʼ����ʱ��1 5ms  Control �ٶȻ�
//		
//		exit_init(INT0_P32, FALLING_EDGE);	  //�½��ش���INT0_P32�ж�  KEY1
//		exit_init(INT1_P33, FALLING_EDGE);	  //�½��ش���INT1_P33�ж�  KEY2
//		exit_init(INT2_P36, FALLING_EDGE);	  //�½��ش���INT2_P36�ж�  KEY3
//		exit_init(INT3_P37, FALLING_EDGE);	  //�½��ش���INT3_P37�ж�  KEY4
	
		pwm_init(PWMA_CH2P_P62,10000,0);//��ʼ������
		pwm_init(PWMA_CH1P_P60,10000,0);//��ʼ���ҵ��
		
		ctimer_count_init(CTIM0_P34);//������1��ʼ��
		ctimer_count_init(CTIM3_P04);//������2��ʼ��
		
	
	
	
		//��λ������
		uart_init(UART_1, UART1_RX_P30, UART1_TX_P31, 115200, TIM_2); 
		delay_ms(300);  //��ʱ300ms���ȴ��������������ϵ�ɹ�
    EnableGlobalIRQ();
		flag_star = 1;
    while(1)
    {
			ANO_DT_SendtoUSER(PulseL,GoalPulseL,PulseR,GoalPulseR);
			//delay_ms(20);
			TFTLCD_show();
			if(P32 == 0)
			{
				delay_ms(10);
				if(P32 == 0)
				{
					while(P32);
					Velocity_KI_L +=0.1;
				}
			}
			else if(P33 == 0)
			{
				delay_ms(10);
				if(P33 == 0)
				{
					while(P33);
					LED = !LED;	//��תLED
					//k=at24c02_read_byte(AT24C02_DEV_ADDR);
					Velocity_KI_L -=0.1;
				}
			}
			else if(P45 == 0)
			{
				delay_ms(10);
				if(P45 == 0)
				{
					while(P45);
					LED = !LED;	//��תLED
					Velocity_KP_L +=0.1;
				}
			}
			else if(P46 == 0)
			{
				delay_ms(10);
				if(P46 == 0)
				{
					while(P46);
					LED = !LED;	//��תLED
					Velocity_KP_L -=0.1;
				}
			}
			
			
    }
}

