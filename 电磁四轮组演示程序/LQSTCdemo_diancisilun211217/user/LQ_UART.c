/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技STC16核心板
【修    改】chiusir
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

u8 Uart4_Receive;
#define Baudrate1   (65536 - MAIN_Fosc / 115200 / 4)
#define Baudrate4   (65536 - MAIN_Fosc / 115200 / 4)

//========================================================================
// 函数: void UART1_int (void) interrupt UART1_VECTOR
// 描述: UART1中断函数。
// 参数: nine.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-11-28
// 备注: 
//========================================================================
void UART1_int (void) interrupt 4
{
  if(RI)
  {
    RI = 0;    
  }
  
  if(TI)
  {
    TI = 0;    
  }
}
//========================================================================
// 函数: void UART4_int (void) interrupt UART4_VECTOR
// 描述: UART4中断函数。蓝牙遥控。
// 参数: nine.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-11-28
// 备注: 
//========================================================================
extern short MotorDuty1, MotorDuty2,ServoDuty;				  // 电机舵机PMW变量
extern unsigned char Flag_Stop;
extern short Velocity;

void UART4_int (void) interrupt 18
{  
  static int uart_receive=0;//蓝牙接收相关变量
  static u8 Flag_PID,i,j,Receive[50];                      
  static float Data;                                       
  
  if((S4CON & 0x01) != 0)                                  
    {                                                        
		S4CON &= ~0x01;                               //Clear Rx flag
                                                                 
		uart_receive = S4BUF;                                      
		Uart4_Receive = uart_receive;                              
	                                           
	
	}  											 

    if((S4CON & 0x02) != 0)
    {
        S4CON &= ~0x02;                           //Clear Tx flag        
    } 
}
//========================================================================
// 函数: SetTimer2Baudraye(u32 dat)
// 描述: 设置Timer2做波特率发生器。
// 参数: dat: Timer2的重装值.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-11-28
// 备注: 
//========================================================================
void SetTimer2Baudraye(u32 dat)
{
  AUXR &= ~(1<<4);                                         //Timer stop
  AUXR &= ~(1<<3);                                         //Timer2 set As Timer
  AUXR |=  (1<<2);                                         //Timer2 set as 1T mode
  T2H = (u8)(dat / 256);
  T2L = (u8)(dat % 256);
  IE2  &= ~(1<<2);                                         //禁止中断
  AUXR |=  (1<<4);                                         //Timer run enable
}
//========================================================================
// 函数: void   UART1_config(u16 tmr)
// 描述: UART1初始化函数。
// 参数: tmr: 选择波特率, 2: 使用Timer2做波特率, 其它值: 使用Timer1做波特率.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-11-28
// 备注: 
//========================================================================
void UART1_config(u16 tmr)
{  
  /*********** 波特率使用定时器2 *****************/
  if(tmr == 2)
  {
    AUXR |= 0x01;                       //S1 tmr Use Timer2;
    SetTimer2Baudraye(Baudrate1);
  }
  /*********** 波特率使用定时器1 *****************/
  else
  {
    TR1 = 0;
    AUXR &= ~0x01;                       //S1 tmr Use Timer1;
    AUXR |=  (1<<6);                     //Timer1 set as 1T mode
    TMOD &= ~(1<<6);                     //Timer1 set As Timer
    TMOD &= ~0x30;                       //Timer1_16bitAutoReload;
    TH1 = (u8)(Baudrate1 / 256);
    TL1 = (u8)(Baudrate1 % 256);
    ET1 = 0;                             //禁止中断
    INTCLKO &= ~0x02;                                         //不输出时钟
    TR1  = 1;
  }
  /*************************************************/  
  SCON = (SCON & 0x3f) | 0x40;                                //UART1模式, 0x00: 同步移位输出, 0x40: 8位数据,可变波特率, 0x80: 9位数据,固定波特率, 0xc0: 9位数据,可变波特率
  //  PS  = 1;                                                    //高优先级中断
  ES  = 1;                                                    //允许中断
  REN = 1;                                                    //允许接收
  P_SW1 &= 0x3f;
  //  P_SW1 |= 0x00;                                          //UART1 switch to, 0x00: P3.0 P3.1, 0x40: P3.6 P3.7, 0x80: P1.6 P1.7, 0xC0: P4.3 P4.4
}

void UART4_config(u16 tmr)                                    // 选择波特率, 2: 使用Timer2做波特率, 其它值: 使用Timer4做波特率.
{
  if(tmr == 2)
  {
    SetTimer2Baudraye(Baudrate4);
    S4CON = 0x10;                                            //8位数据, 使用Timer2做波特率发生器, 允许接收
  }
  else
  {
    S4CON = 0x50;                                            //8位数据, 使用Timer4做波特率发生器, 允许接收
    T4H = (Baudrate4) / 256;
    T4L = (Baudrate4) % 256;
    T4T3M = 0xA0;
  }
  IE2 |= 0x10;                                               //允许UART4中断
  //    P_SW2 &= ~0x04;                                      //UART4 switch bit2 to: 0: P0.2 P0.3
  P_SW2 |= 0x04;                                             //UART4 switch bit2 to: 1: P5.2 P5.3
}

/*************************************************************************
*  函数名称：void UART_PutChar(UART_t  uratn, char ch)
*  功能说明：UART发送字节函数,使用前请先初始化对应串口
*  参数说明：uratn ： UART0 - UART3/ ch    ： 要打印的字符
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：UART_PutChar(UART0, 'a');  //打印字符a
*************************************************************************/
void UART1_PutChar(char ch)
{
  SBUF = ch;    
}
void UART4_PutChar(char ch)
{
  S4BUF = ch;    
}

/*************************************************************************
*  函数名称：void UART_PutStr(UART_t  uratn, char *st)
*  功能说明：UART发送字符串函数(遇 NULL 停止发送),使用前请先初始化对应串口
*  参数说明：uratn ： UART0 - UART3/ str   ： 要打印的字符串地址
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：UART_PutStr(UART3, "123456789"); //发送9个字节
*************************************************************************/
void UART1_PutStr(char *st)
{
  while(*st)
  {
    UART1_PutChar(*st++);
  }
}
void UART4_PutStr(char *st)
{
  while(*st)
  {
    UART4_PutChar(*st++);
  }
}

/*************************************************************************
*  函数名称：void UART4_PutBuff(unsigned char *buff, unsigned long len)
*  功能说明：UART发送字节函数,使用前请先初始化对应串口
*  参数说明：
* @param    buff  ： 要打印的字符串地址
* @param    len   ： 要打印的长度
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：UART4_PutBuff("123456789",5);//实际发送5个字节‘1’‘2’‘3’‘4’‘5’
*************************************************************************/
void UART4_PutBuff(unsigned char *buff, unsigned long len)
{
  while(len--)
  {
    UART4_PutChar(*buff);
    buff++;
  }
}

/*************************************************************************
*  函数名称：char UART_GetChar(UART_t  uratn)
*  功能说明：UART读取字节 使用前请先初始化对应串口 调用前请确认有接收到数据 否则会等待数据到来
*  参数说明：uratn ： UART0 - UART3
*  函数返回：读取字节
*  修改时间：2020年3月10日
*  备    注：UART_GetChar(UART3); //接收一个字符
*************************************************************************/
char UART1_GetChar(void)
{  
  return (SBUF);
}
char UART4_GetChar(void)
{  
  return (S4BUF);
}

//
