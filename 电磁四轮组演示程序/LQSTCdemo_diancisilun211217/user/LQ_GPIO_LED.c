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

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void LED_Init(void)
����  �ܡ�GPIO��ʼ������ LED������
������ֵ����
������ֵ���� 
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GPIO_LED_Init(void)
{
  P2M1 &= 0xFE;
  P2M1 |= 0x00;
  P2M0 &= 0xFE;
  P2M0 |= 0x01;   //����Ϊ�������
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void LED_Ctrl(LEDn_e LEDno, LEDs_e sta)
����  �ܡ�LED����
������ֵ��LEDn_e LEDno���,LEDs_e sta״̬��/��/��ת
������ֵ���� 
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LED_Ctrl(LEDn_e LEDno, LEDs_e sta)
{
  switch(LEDno)
  {
  case LED0:
    if(sta==ON)        LED0p=0;
    else if(sta==OFF)  LED0p=1;
    else if(sta==RVS) (LED0p)?(LED0p=0):(LED0p=1);
    break;
    
  case LED1:
    if(sta==ON)        LED1p=0;
    else if(sta==OFF)  LED1p=1;
    else if(sta==RVS) (LED1p)?(LED1p=0):(LED1p=1);
    break;
    
  case LED2:
    if(sta==ON)        LED2p=0;
    else if(sta==OFF)  LED2p=1;
    else if(sta==RVS) (LED2p)?(LED2p=0):(LED2p=1);
    break;
    
  case LED3:
    if(sta==ON)        LED3p=0;
    else if(sta==OFF)  LED3p=1;
    else if(sta==RVS) (LED3p)?(LED3p=0):(LED3p=1);
    break;
  case LEDALL:
    if(sta==ON)
    {
      LED0p=0;
      LED1p=0;
      LED2p=0;
      LED3p=0;
    }
    else if(sta==OFF)
    {
      LED0p=1;
      LED1p=1;
      LED2p=1;
      LED3p=1;
    }
    else if(sta==RVS)
    {
      (LED0p)?(LED0p=0):(LED0p=1);
      (LED1p)?(LED1p=0):(LED1p=1);
      (LED2p)?(LED2p=0):(LED2p=1);
      (LED3p)?(LED3p=0):(LED3p=1);
    }
    break;
  default : break;
  }
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������
����  �ܡ�LED��˸
������ֵ����˸Ƶ�� 
������ֵ���� 
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Led_Flash(u16 time)
{
  static int temp;
  if(0==time) LED2p=0;
  else if(++temp>time)	LED2p=~LED2p,temp=0;
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void Test_GPIO_LED(void)
����  �ܡ����İ��ϵ�LED����˸--�ĸ�LEDͬʱ��˸
������ֵ����
������ֵ���� 
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_GPIO_LED(void)
{
  GPIO_LED_Init();
  while(1)
  {
    LED_Ctrl(LEDALL,RVS);        // LEDͬʱ��˸
    delayms(200);                // ��ʱ�ȴ�
  }
}
