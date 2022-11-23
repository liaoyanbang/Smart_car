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
#ifndef __LQ_LED_H_
#define __LQ_LED_H_

#include "include.h"

//定义模块号
typedef enum
{
  LED0=0,  //一体板上LED0
  LED1=1,  //一体板上LED1，第二版才有
  LED2=2,  //核心板上LED0
  LED3=3,  //核心板上LED1
  LEDALL=4
} LEDn_e;

typedef enum
{
  ON=0,  //亮
  OFF=1, //灭
  RVS=2, //反转
}LEDs_e;

//定义的管脚要对应实际按键
#define LED0p      P20   //一体板上LED0 翠绿
#define LED1p      P21   //核心板上LED1 蓝灯
#define LED2p      P42   //核心板上LED0
#define LED3p      P62   //核心板上LED1 //不要跟OLED冲突了

/*********************** UART功能函数 **************************/
//初始化
void GPIO_LED_Init(void);
void LED_Ctrl(LEDn_e LEDno, LEDs_e sta);
void Test_GPIO_LED(void);
void delayms(u16 nms);
void Led_Flash(u16 time);
#endif
