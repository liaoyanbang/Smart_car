/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技STC16核心板
【编    写】chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2021年1月23日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【IDE】STC16
【Target 】 C251Keil5.6及以上
【SYS PLL】 30MHz使用内部晶振
=================================================================
STC16相关配套视频：
龙邱科技B站网址：https://space.bilibili.com/95313236
STC16环境下载参考视频： https://www.bilibili.com/video/BV1gy4y1p7T1/
STC16一体板子介绍视频： https://www.bilibili.com/video/BV1Jy4y1e7R4/
=================================================================
下载时, 选择时钟 30MHZ (用户可自行修改频率).
STC16F初次下载:先用IRCBND=0，ISP界面设置为24M，
然后IRCBND=0，下载频率为30M；
或者IRCBND=1，下载频率为30M；好用为准
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"

sbit SDA = P3^3;
sbit SCL = P3^2;
#define SLAW    0xA2
#define SLAR    0xA3

void I2C_init(void)
{
  I2CCFG = 0xe0;                          //使能I2C主机模式
  I2CMSST = 0x00;
}

void Wait()
{
  while (!(I2CMSST & 0x40));
  I2CMSST &= ~0x40;
}

void Start()
{
  I2CMSCR = 0x01;                         //发送START命令
  Wait();
}

void SendData(char dat)
{
  I2CTXD = dat;                           //写数据到数据缓冲区
  I2CMSCR = 0x02;                         //发送SEND命令
  Wait();
}

void RecvACK()
{
  I2CMSCR = 0x03;                         //发送读ACK命令
  Wait();
}

char RecvData()
{
  I2CMSCR = 0x04;                         //发送RECV命令
  Wait();
  return I2CRXD;
}

void SendACK()
{
  I2CMSST = 0x00;                         //设置ACK信号
  I2CMSCR = 0x05;                         //发送ACK命令
  Wait();
}

void SendNAK()
{
  I2CMSST = 0x01;                         //设置NAK信号
  I2CMSCR = 0x05;                         //发送ACK命令
  Wait();
}

void Stop()
{
  I2CMSCR = 0x06;                         //发送STOP命令
  Wait();
}

void WriteNbyte(u8 addr, u8 *p, u8 number)  /*  WordAddress,First Data Address,Byte lenth   */
{
  Start();                                //发送起始命令
  SendData(SLAW);                         //发送设备地址+写命令
  RecvACK();
  SendData(addr);                         //发送存储地址
  RecvACK();
  do
  {
    SendData(*p++);
    RecvACK();
  }
  while(--number);
  Stop();                                 //发送停止命令
}

void ReadNbyte(u8 addr, u8 *p, u8 number)   /*  WordAddress,First Data Address,Byte lenth   */
{
  Start();                                //发送起始命令
  SendData(SLAW);                         //发送设备地址+写命令
  RecvACK();
  SendData(addr);                         //发送存储地址
  RecvACK();
  Start();                                //发送起始命令
  SendData(SLAR);                         //发送设备地址+读命令
  RecvACK();
  do
  {
    *p = RecvData();
    p++;
    if(number != 1) SendACK();          //send ACK
  }
  while(--number);
  SendNAK();                              //send no ACK	
  Stop();                                 //发送停止命令
}
/****************************/
