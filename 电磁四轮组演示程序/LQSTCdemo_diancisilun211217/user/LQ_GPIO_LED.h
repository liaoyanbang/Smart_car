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
#ifndef __LQ_LED_H_
#define __LQ_LED_H_

#include "include.h"

//����ģ���
typedef enum
{
  LED0=0,  //һ�����LED0
  LED1=1,  //һ�����LED1���ڶ������
  LED2=2,  //���İ���LED0
  LED3=3,  //���İ���LED1
  LEDALL=4
} LEDn_e;

typedef enum
{
  ON=0,  //��
  OFF=1, //��
  RVS=2, //��ת
}LEDs_e;

//����Ĺܽ�Ҫ��Ӧʵ�ʰ���
#define LED0p      P20   //һ�����LED0 ����
#define LED1p      P21   //���İ���LED1 ����
#define LED2p      P42   //���İ���LED0
#define LED3p      P62   //���İ���LED1 //��Ҫ��OLED��ͻ��

/*********************** UART���ܺ��� **************************/
//��ʼ��
void GPIO_LED_Init(void);
void LED_Ctrl(LEDn_e LEDno, LEDs_e sta);
void Test_GPIO_LED(void);
void delayms(u16 nms);
void Led_Flash(u16 time);
#endif
