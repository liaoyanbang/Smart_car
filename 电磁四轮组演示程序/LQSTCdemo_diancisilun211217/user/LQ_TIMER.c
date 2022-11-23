/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�STC16���İ�
����    �ġ�chiusir
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

#define Timer0_Int_Freq  200
#define Timer1_Int_Freq  200
#define Timer2_Int_Freq  100

#define Timer0_Reload   (MAIN_Fosc /Timer0_Int_Freq)      //Timer 0 �ж�Ƶ��, n��/��
#define Timer1_Reload   (MAIN_Fosc /Timer1_Int_Freq)      //Timer 1 �ж�Ƶ��, n��/��
#define Timer2_Reload   (MAIN_Fosc /Timer2_Int_Freq)      //Timer 2 �ж�Ƶ��, n��/��
#define Timer3_Reload   (MAIN_Fosc / 300)                 //Timer 3 �ж�Ƶ��, 400��/��
#define Timer4_Reload   (MAIN_Fosc / 300)                 //Timer 4 �ж�Ƶ��, 500��/��

//========================================================================
// ����: void timer0_int (void) interrupt TIMER0_VECTOR
// ����: timer0�жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2015-1-12
//========================================================================
/*
void timer0_int (void) interrupt 1
{
  LED_Ctrl(LED0, RVS); //ƽ�⳵ռ��
}
*/

//========================================================================
// ����: void timer1_int (void) interrupt TIMER1_VECTOR
// ����: timer1�жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2015-1-12
//========================================================================
/*void timer1_int (void) interrupt 3 
{
   LED_Ctrl(LED0, RVS);
}
*/
//========================================================================
// ����: void timer2_int (void) interrupt TIMER2_VECTOR
// ����: timer2�жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2015-1-12
//========================================================================
void timer2_int (void) interrupt 12
{
  //LED_Ctrl(LED0, RVS);//������
}

void timer3_int (void) interrupt 19
{
	LED_Ctrl(LED0, RVS);
}
void timer4_int (void) interrupt 20
{
	LED_Ctrl(LED0, RVS);
}

//========================================================================
// ����: void   Timer0_init(void)
// ����: timer0��ʼ������.
// ����: none.
// ����: none.
// �汾: V1.0, 2015-1-12
//========================================================================
void Timer0_init(void)
{
  TR0 = 0;    //ֹͣ����
  
#if (Timer0_Reload < 64)    						// ����û�����ֵ�����ʣ� ��������ʱ��
#error "Timer0���õ��жϹ���!"
  
#elif ((Timer0_Reload/12) < 65536UL)    // ����û�����ֵ�����ʣ� ��������ʱ��
  ET0 = 1;    	                      	//�����ж�
  //  PT0 = 1;                          //�����ȼ��ж�
  TMOD &= ~0x03;
  TMOD |= 0;                            //����ģʽ, 0: 16λ�Զ���װ, 1: 16λ��ʱ/����, 2: 8λ�Զ���װ, 3: 16λ�Զ���װ, ���������ж�
  //  TMOD |=  0x04;                    //����������Ƶ
  TMOD &= ~0x04;  //��ʱ
  //  INTCLKO |=  0x01;                 //���ʱ��
  INTCLKO &= ~0x01;                     //�����ʱ��
  
#if (Timer0_Reload < 65536UL)
  AUXR |=  0x80;                        //1T mode
  TH0 = (u8)((65536UL - Timer0_Reload) / 256);
  TL0 = (u8)((65536UL - Timer0_Reload) % 256);
#else
  AUXR &= ~0x80;  //12T mode
  TH0 = (u8)((65536UL - Timer0_Reload/12) / 256);
  TL0 = (u8)((65536UL - Timer0_Reload/12) % 256);
#endif
  
  TR0 = 1;                              //��ʼ����
  
#else
#error "Timer0���õ��жϹ���!"
#endif
}

//========================================================================
// ����: void   Timer1_init(void)
// ����: timer1��ʼ������.
// ����: none.
// ����: none.
// �汾: V1.0, 2015-1-12
//========================================================================
void    Timer1_init(void)
{
  TR1 = 0;                               //ֹͣ����
  
#if (Timer1_Reload < 64)                 // ����û�����ֵ�����ʣ� ��������ʱ��
#error "Timer1���õ��жϹ���!"
  
#elif ((Timer1_Reload/12) < 65536UL)     // ����û�����ֵ�����ʣ� ��������ʱ��
  ET1 = 1;    	                         //�����ж�
  //  PT1 = 1;                           //�����ȼ��ж�
  TMOD &= ~0x30;
  TMOD |= (0 << 4);                      //����ģʽ, 0: 16λ�Զ���װ, 1: 16λ��ʱ/����, 2: 8λ�Զ���װ
  //  TMOD |=  0x40;                     //����������Ƶ
  TMOD &= ~0x40;  //��ʱ
  //  INTCLKO |=  0x02;                  //���ʱ��
  INTCLKO &= ~0x02;  		                 //�����ʱ��
  
#if (Timer1_Reload < 65536UL)
  AUXR |=  0x40;                         //1T mode
  TH1 = (u8)((65536UL - Timer1_Reload) / 256);
  TL1 = (u8)((65536UL - Timer1_Reload) % 256);
#else
  AUXR &= ~0x40;                         //12T mode
  TH1 = (u8)((65536UL - Timer1_Reload/12) / 256);
  TL1 = (u8)((65536UL - Timer1_Reload/12) % 256);
#endif
  
  TR1 = 1;                               //��ʼ����
  
#else
#error "Timer1���õ��жϹ���!"
#endif
}

//========================================================================
// ����: void   Timer2_init(void)
// ����: timer2��ʼ������.
// ����: none.
// ����: none.
// �汾: V1.0, 2015-1-12
//========================================================================
void    Timer2_init(void)
{
  AUXR &= ~0x1c;      			             //ֹͣ����, ��ʱģʽ, 12Tģʽ
   
#if (Timer2_Reload < 64)                 // ����û�����ֵ�����ʣ� ��������ʱ��
#error "Timer2���õ��жϹ���!"
  
#elif ((Timer2_Reload/12) < 65536UL)     // ����û�����ֵ�����ʣ� ��������ʱ��
  ET2 = 1;    			                    	//�����ж�
  //  INTCLKO |=  0x04;                  //���ʱ��
  INTCLKO &= ~0x04;                      //�����ʱ��
  
#if (Timer2_Reload < 65536UL)
  AUXR |=  (1<<2);                       //1T mode
  T2H = (u8)((65536UL - Timer2_Reload) / 256);
  T2L = (u8)((65536UL - Timer2_Reload) % 256);
#else
  T2H = (u8)((65536UL - Timer2_Reload/12) / 256);
  T2L = (u8)((65536UL - Timer2_Reload/12) % 256);
#endif
  
  AUXR |=  (1<<4);                       //��ʼ����
  
#else
#error "Timer2���õ��жϹ���!"
#endif
}

//========================================================================
// ����: void   Timer3_init(void)
// ����: timer3��ʼ������.
// ����: none.
// ����: none.
// �汾: V1.0, 2015-1-12
//========================================================================
void    Timer3_init(void)
{
  T4T3M &= ~0x0f;                         //ֹͣ����, ��ʱģʽ, 12Tģʽ, �����ʱ��
  
#if (Timer3_Reload < 64)                  // ����û�����ֵ�����ʣ� ��������ʱ��
#error "Timer3���õ��жϹ���!"
  
#elif ((Timer3_Reload/12) < 65536UL)      // ����û�����ֵ�����ʣ� ��������ʱ��
  ET3 = 1;                                //�����ж�
  //  T4T3M |=  0x01;                     //���ʱ��
  //  T4T3M &= ~0x01;                     //�����ʱ��
  
#if (Timer3_Reload < 65536UL)
  T4T3M |=  (1<<1);                       //1T mode
  T3H = (u8)((65536UL - Timer3_Reload) / 256);
  T3L = (u8)((65536UL - Timer3_Reload) % 256);
#else
  T3H = (u8)((65536UL - Timer3_Reload/12) / 256);
  T3L = (u8)((65536UL - Timer3_Reload/12) % 256);
#endif
  
  T4T3M |=  (1<<3);                       //��ʼ����
  
#else
#error "Timer3���õ��жϹ���!"
#endif
}

//========================================================================
// ����: void   Timer4_init(void)
// ����: timer4��ʼ������.
// ����: none.
// ����: none.
// �汾: V1.0, 2015-1-12
//========================================================================
void    Timer4_init(void)
{
  T4T3M &= ~0xf0;     			              //ֹͣ����, ��ʱģʽ, 12Tģʽ, �����ʱ��
  
#if (Timer4_Reload < 64)                  // ����û�����ֵ�����ʣ� ��������ʱ��
#error "Timer4���õ��жϹ���!"
  
#elif ((Timer4_Reload/12) < 65536UL)      // ����û�����ֵ�����ʣ� ��������ʱ��
  ET4 = 1;    //�����ж�
  //  T4T3M |=  0x10;                   	//���ʱ��
  //  T4T3M &= ~0x10;    	                //�����ʱ��
  
#if (Timer4_Reload < 65536UL)
  T4T3M |=  (1<<6);    			              //12T mode ;P0.6����
  T4H = (u8)((65536UL - Timer4_Reload) / 256);
  T4L = (u8)((65536UL - Timer4_Reload) % 256);
#else
  T4H = (u8)((65536UL - Timer4_Reload/12) / 256);
  T4L = (u8)((65536UL - Timer4_Reload/12) % 256);
#endif
  
  T4T3M |=  (1<<7);    			              //��ʼ����
  
#else
#error "Timer4���õ��жϹ���!"
#endif
}