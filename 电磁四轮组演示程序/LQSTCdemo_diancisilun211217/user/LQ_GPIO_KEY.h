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
#ifndef __LQ_KEY_H_
#define __LQ_KEY_H_

#include "include.h"

//定义模块号
typedef enum
{
  KEY0 = 0,  //母板上按键0
  KEY1 = 1,
  KEY2 = 2,
  KEY3 = 3,
  DSW0 = 4,    //母板上拨码开关0
  DSW1 = 5,
} KEYn_e;

typedef enum
{
  LOW = 0,  //按下
  HIGH = 1, //松开
  FAIL = 0xff, //错误
} KEYs_e;

typedef enum //
{
  NOKEYDOWN = 0, KEY0DOWN = 0x01,  //母板上按键0
  KEY1DOWN = 0x02,
  KEY2DOWN = 0x04,
  KEY01DOWN = 0x03,  //母板上按键0
  KEY02DOWN = 0x05,
  KEY12DOWN = 0x06,
  KEY012DOWN = 0x07,
  KEYError = 0xFF,
} KEYdown_e;
//定义的管脚要对应实际按键
#define KEY0p      P70 //母板上按键0
#define KEY1p      P71 //母板上按键1
#define KEY2p      P72 //母板上按键2
#define KEY3p      P73 //母板上按键2

#define REEDp      P37 //母板上干簧管
/*********************** UART功能函数 **************************/
//初始化
void GPIO_KEY_Init (void);
unsigned char KEY_Read (KEYn_e KEYno);
unsigned char KEY_Read_Comb (void);
void Test_GPIO_KEY (void);
void Test_ComKEY_Tft (void);
/*************************************************************************
*  函数名称：void Reed_Init(void)
*  功能说明：干簧管GPIO及中断初始化函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年11月21日
*  备    注：   中断在那个核初始化，中断VECTABNUM号必须对应该cpu,否则不会调用中断函数
*          中断服务函数PIN_INT0_IRQHandler在LQ_GPIO.c中
*************************************************************************/
void Reed_Init(void);
#endif
