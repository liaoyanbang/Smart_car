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
#ifndef __IOI2C_H
#define __IOI2C_H

#include "LQ_SOFTI2C.h"

//IO��������
//#define SDA_IN()  {P2M1 = 1;P2M0 = 0;}
//#define SDA_OUT() {P2M1 = 0;P2M0 = 0;}

//IO��������	 
#define SOFT_IIC_SCL    P25      //SCK
#define SOFT_IIC_SDA    P24      //��Ƭ��MOSI��ģ��--SDI
#define READ_SDA   P24      //��Ƭ��MOSI��ģ��--SDI  

//IIC���в�������
void SOFT_IIC_Init(void);                //��ʼ��IIC��IO��				 
int  SOFT_IIC_Start(void);				//����IIC��ʼ�ź�
void SOFT_IIC_Stop(void);	  			//����IICֹͣ�ź�
void SOFT_IIC_Send_Byte(unsigned char txd);			//IIC����һ���ֽ�
unsigned char   SOFT_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
int  SOFT_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void SOFT_IIC_Ack(void);					//IIC����ACK�ź�
void SOFT_IIC_NAck(void);				//IIC������ACK�ź�

void SOFT_IIC_Write_One_Byte(unsigned char daddr,unsigned char addr,unsigned char dat);
unsigned char SOFT_IIC_Read_One_Byte(unsigned char daddr,unsigned char addr);	 
unsigned char I2C_Readkey(unsigned char I2C_Addr);

unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr);
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char dat);
unsigned char IICwriteBytes(unsigned char dev, unsigned char reg, unsigned char length, unsigned char* dat);
unsigned char IICwriteBits(unsigned char dev,unsigned char reg,unsigned char bitStart,unsigned char length,unsigned char dat);
unsigned char IICwriteBit(unsigned char dev,unsigned char reg,unsigned char bitNum,unsigned char dat);
unsigned char IICreadBytes(unsigned char dev, unsigned char reg, unsigned char length, unsigned char *dat);

int LQ_I2C_Write(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *dat);
int LQ_I2C_Read(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *buf);
void delayms(unsigned int ms);
void delayus(unsigned int us);

#endif

//------------------End of File----------------------------
