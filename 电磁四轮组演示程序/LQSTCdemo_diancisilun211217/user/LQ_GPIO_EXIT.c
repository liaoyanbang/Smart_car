/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�STC16���İ�
����    д��chiusir
��E-mail  ��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2021��1��23��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��IDE��STC16
��Target �� C251Keil5.6������
��SYS PLL�� 30MHzʹ���ڲ�����
=================================================================
STC16���������Ƶ��
����Ƽ�Bվ��ַ��https://space.bilibili.com/95313236
STC16�������زο���Ƶ�� https://www.bilibili.com/video/BV1gy4y1p7T1/
STC16һ����ӽ�����Ƶ�� https://www.bilibili.com/video/BV1Jy4y1e7R4/
=================================================================
����ʱ, ѡ��ʱ�� 30MHZ (�û��������޸�Ƶ��).
STC16F��������:����IRCBND=0��ISP��������Ϊ24M��
Ȼ��IRCBND=0������Ƶ��Ϊ30M��
����IRCBND=1������Ƶ��Ϊ30M������Ϊ׼
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"

#define EX2 0x10
#define EX3 0x20
#define EX4 0x40

u8  INT0_cnt, INT1_cnt; 					//�����õļ�������
u8  INT2_cnt, INT3_cnt, INT4_cnt; //�����õļ�������

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������
����  �ܡ�INT0123���жϺ�������Ҫ�ദ����
������ֵ����
������ֵ���� 
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void INT0_int (void) interrupt 0  //���ж�ʱ�Ѿ������־
{
  INT0_cnt++;										//�ж�+1
}

/********************* INT1�жϺ��� *************************/
void INT1_int (void) interrupt 2      //���ж�ʱ�Ѿ������־
{
  INT1_cnt++; //�ж�+1
}

/********************* INT2�жϺ��� *************************/
/*
void INT2_int (void) interrupt 10 //���ж�ʱ�Ѿ������־
{//IMU��,balance.c��
INT2_cnt++; 									//�ж�+1
LED_Ctrl(LEDALL,RVS);	
}
*/
/********************* INT3�жϺ��� *************************/
void INT3_int (void) interrupt 11 //���ж�ʱ�Ѿ������־
{//�ɻɹ���
  INT3_cnt++; 									//�ж�+1
  //LED_Ctrl(LED0, RVS);   				//LED��˸;
}

/********************* INT4�жϺ��� *************************/
void INT4_int (void) interrupt 16 //���ж�ʱ�Ѿ������־
{
  INT4_cnt++; 									//�ж�+1
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void GPIO_EXIT_Init(void)
����  �ܡ�GPIO�ⲿ�жϳ�ʼ����ʼ��
������ֵ����
������ֵ���� 
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GPIO_EXIT_Init(void)
{                                
  IE1 = 0;                      // ���ж�1��־λ
  IE0 = 0;                      // ���ж�0��־λ    
  EX0 = 0;                      // INT0 ��ʹ��
  EX1 = 0;                      // INT1 ��ʹ��
  IT0 = 1;                      // INT0 �½����ж�       
  //  IT0 = 0;                  // INT0 ����,�½����ж�
  IT1 = 1;                      // INT1 �½����ж� 
  
  // INT2, INT3, INT4 ������Ҫʱ�ο�ʹ��
  INTCLKO |= EX2;             	// ʹ�� INT2 �½����ж� IMUģ�� P3.6
  //INTCLKO |= EX3; 	          // ʹ�� INT3 �½����ж� �ɻɹ�  P3.7
  //INTCLKO |= EX4;             // ʹ�� INT4 �½����ж�  
  EA = 1;     					        // �������ж�
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void Test_GPIO_EXIT_Init(void)
����  �ܡ����Գ���,���ϵ�LED����˸
������ֵ����
������ֵ���� 
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_GPIO_EXIT_Init(void)
{
  GPIO_LED_Init();				
  GPIO_EXIT_Init();
  while(1)
  {
    //LED_Ctrl(LEDALL,RVS);		     //LED��˸
    delayms(200);                //��ʱ�ȴ�
  }
}
