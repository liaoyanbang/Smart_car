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

#define EX2 0x10
#define EX3 0x20
#define EX4 0x40

u8  INT0_cnt, INT1_cnt; 					//测试用的计数变量
u8  INT2_cnt, INT3_cnt, INT4_cnt; //测试用的计数变量

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】
【功  能】INT0123等中断函数，不要多处调用
【参数值】无
【返回值】无 
【作  者】chiusir
【最后更新】2021年1月22日 
【软件版本】V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void INT0_int (void) interrupt 0  //进中断时已经清除标志
{
  INT0_cnt++;										//中断+1
}

/********************* INT1中断函数 *************************/
void INT1_int (void) interrupt 2      //进中断时已经清除标志
{
  INT1_cnt++; //中断+1
}

/********************* INT2中断函数 *************************/
/*
void INT2_int (void) interrupt 10 //进中断时已经清除标志
{//IMU用,balance.c中
INT2_cnt++; 									//中断+1
LED_Ctrl(LEDALL,RVS);	
}
*/
/********************* INT3中断函数 *************************/
void INT3_int (void) interrupt 11 //进中断时已经清除标志
{//干簧管用
  INT3_cnt++; 									//中断+1
  //LED_Ctrl(LED0, RVS);   				//LED闪烁;
}

/********************* INT4中断函数 *************************/
void INT4_int (void) interrupt 16 //进中断时已经清除标志
{
  INT4_cnt++; 									//中断+1
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void GPIO_EXIT_Init(void)
【功  能】GPIO外部中断初始化初始化
【参数值】无
【返回值】无 
【作  者】chiusir
【最后更新】2021年1月22日 
【软件版本】V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GPIO_EXIT_Init(void)
{                                
  IE1 = 0;                      // 外中断1标志位
  IE0 = 0;                      // 外中断0标志位    
  EX0 = 0;                      // INT0 不使能
  EX1 = 0;                      // INT1 不使能
  IT0 = 1;                      // INT0 下降沿中断       
  //  IT0 = 0;                  // INT0 上升,下降沿中断
  IT1 = 1;                      // INT1 下降沿中断 
  
  // INT2, INT3, INT4 ，供需要时参考使用
  INTCLKO |= EX2;             	// 使能 INT2 下降沿中断 IMU模块 P3.6
  //INTCLKO |= EX3; 	          // 使能 INT3 下降沿中断 干簧管  P3.7
  //INTCLKO |= EX4;             // 使能 INT4 下降沿中断  
  EA = 1;     					        // 允许总中断
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void Test_GPIO_EXIT_Init(void)
【功  能】测试程序,板上的LED灯闪烁
【参数值】无
【返回值】无 
【作  者】chiusir
【最后更新】2021年1月22日 
【软件版本】V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_GPIO_EXIT_Init(void)
{
  GPIO_LED_Init();				
  GPIO_EXIT_Init();
  while(1)
  {
    //LED_Ctrl(LEDALL,RVS);		     //LED闪烁
    delayms(200);                //延时等待
  }
}
