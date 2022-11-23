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

sbit SDA = P3^3;
sbit SCL = P3^2;
#define SLAW    0xA2
#define SLAR    0xA3

void I2C_init(void)
{
  I2CCFG = 0xe0;                          //ʹ��I2C����ģʽ
  I2CMSST = 0x00;
}

void Wait()
{
  while (!(I2CMSST & 0x40));
  I2CMSST &= ~0x40;
}

void Start()
{
  I2CMSCR = 0x01;                         //����START����
  Wait();
}

void SendData(char dat)
{
  I2CTXD = dat;                           //д���ݵ����ݻ�����
  I2CMSCR = 0x02;                         //����SEND����
  Wait();
}

void RecvACK()
{
  I2CMSCR = 0x03;                         //���Ͷ�ACK����
  Wait();
}

char RecvData()
{
  I2CMSCR = 0x04;                         //����RECV����
  Wait();
  return I2CRXD;
}

void SendACK()
{
  I2CMSST = 0x00;                         //����ACK�ź�
  I2CMSCR = 0x05;                         //����ACK����
  Wait();
}

void SendNAK()
{
  I2CMSST = 0x01;                         //����NAK�ź�
  I2CMSCR = 0x05;                         //����ACK����
  Wait();
}

void Stop()
{
  I2CMSCR = 0x06;                         //����STOP����
  Wait();
}

void WriteNbyte(u8 addr, u8 *p, u8 number)  /*  WordAddress,First Data Address,Byte lenth   */
{
  Start();                                //������ʼ����
  SendData(SLAW);                         //�����豸��ַ+д����
  RecvACK();
  SendData(addr);                         //���ʹ洢��ַ
  RecvACK();
  do
  {
    SendData(*p++);
    RecvACK();
  }
  while(--number);
  Stop();                                 //����ֹͣ����
}

void ReadNbyte(u8 addr, u8 *p, u8 number)   /*  WordAddress,First Data Address,Byte lenth   */
{
  Start();                                //������ʼ����
  SendData(SLAW);                         //�����豸��ַ+д����
  RecvACK();
  SendData(addr);                         //���ʹ洢��ַ
  RecvACK();
  Start();                                //������ʼ����
  SendData(SLAR);                         //�����豸��ַ+������
  RecvACK();
  do
  {
    *p = RecvData();
    p++;
    if(number != 1) SendACK();          //send ACK
  }
  while(--number);
  SendNAK();                              //send no ACK	
  Stop();                                 //����ֹͣ����
}
/****************************/
