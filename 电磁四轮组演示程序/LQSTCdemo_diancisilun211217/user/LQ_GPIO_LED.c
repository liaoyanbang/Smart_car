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

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void LED_Init(void)
【功  能】GPIO初始化函数 LED灯所用
【参数值】无
【返回值】无 
【最后更新】2021年1月22日 
【软件版本】V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GPIO_LED_Init(void)
{
  P2M1 &= 0xFE;
  P2M1 |= 0x00;
  P2M0 &= 0xFE;
  P2M0 |= 0x01;   //设置为推挽输出
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void LED_Ctrl(LEDn_e LEDno, LEDs_e sta)
【功  能】LED控制
【参数值】LEDn_e LEDno编号,LEDs_e sta状态亮/灭/翻转
【返回值】无 
【最后更新】2021年1月22日 
【软件版本】V1.0
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
【函数名】
【功  能】LED闪烁
【参数值】闪烁频率 
【返回值】无 
【最后更新】2021年1月22日 
【软件版本】V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Led_Flash(u16 time)
{
  static int temp;
  if(0==time) LED2p=0;
  else if(++temp>time)	LED2p=~LED2p,temp=0;
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void Test_GPIO_LED(void)
【功  能】核心板上的LED灯闪烁--四个LED同时闪烁
【参数值】无
【返回值】无 
【最后更新】2021年1月22日 
【软件版本】V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_GPIO_LED(void)
{
  GPIO_LED_Init();
  while(1)
  {
    LED_Ctrl(LEDALL,RVS);        // LED同时闪烁
    delayms(200);                // 延时等待
  }
}
