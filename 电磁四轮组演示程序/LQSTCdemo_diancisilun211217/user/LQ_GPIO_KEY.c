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
【函数名】void GPIO_KEY_Init(void)
【功  能】GPIO初始化函数
【参数值】无
【返回值】无 
【最后更新】2021年1月22日 
【软件版本】V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GPIO_KEY_Init(void)
{
  P7M1 &= 0xF0;
  P7M1 |= 0x00;
  P7M0 &= 0xF0;
  P7M0 |= 0x00;   //设置为推挽输出  
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】unsigned char KEY_Read(KEYn_e KEYno)
【功  能】读取按键状态
【参数值】KEYn_e KEYno按键编号
【返回值】按键状态，0/1
【最后更新】2021年1月22日 
【软件版本】V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char KEY_Read(KEYn_e KEYno)
{
  switch(KEYno)
  {
  case KEY0:
    if(P70)
      return 1;				//母板上按键0
    else
    {
      while(P70==0);  //等待松开按键
      return 0;	
    }
    break;
    
  case KEY1:
    if(P71)
      return 1;				//母板上按键0
    else
    {
      while(P71==0);  //等待松开按键
      return 0;	
    }
    break;
    
  case KEY2:
     if(P72)
      return 1;				//母板上按键0
    else
    {
      while(P72==0);  //等待松开按键
      return 0;	
    }
    break;
    
  case KEY3:
    if(P73)
      return 1;				//母板上按键3
    else
    {
      while(P73==0);  //等待松开按键
      return 0;	
    }
    break;
    
  case DSW0:
    return P32;				//母板上拨码开关0
    break;
    
  case DSW1:
    return P33;				//母板上拨码开关1
    break;
  default:
    return 0XFF;
  }
  return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】unsigned char KEY_Read_Comb(void)
【功  能】读取全部按键状态,读取三个按键状态，方便组合键使用
【参数值】无
【返回值】按键组合状态，0--7八种状态 
【最后更新】2021年1月22日 
【软件版本】V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char KEY_Read_Comb(void)
{
  unsigned char tm=0;
  
  tm = P70 | (P7^1 << 1) | (P7^2 << 2);//读取各个按键状态并编码	
  if(tm==0x07)
  {
    return 0;
  }
  return  (~tm)&0X07;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void Test_GPIO_KEY(void)
【功  能】测试程序,按下KEY0/1/2任意键 LED全亮
【参数值】无
【返回值】无 
【最后更新】2021年1月22日 
【软件版本】V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_GPIO_KEY(void)
{
  unsigned char k=0xFF;
  GPIO_LED_Init();
  GPIO_KEY_Init();
  while(1)
  {
    k= KEY_Read(KEY0)& KEY_Read(KEY1)& KEY_Read(KEY2)& KEY_Read(KEY3);
    
    if(k==0) LED_Ctrl(LEDALL, ON); //按下KEY0/1/2任意键 LED全亮
    else     LED_Ctrl(LEDALL, OFF);//松开KEY0/1/2任意键 LED全灭
    
    delayms(100);                	//延时等待
  }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void Test_ComKEY_Tft(void)
【功  能】测试组合按键并在彩屏显示,按键状态彩屏显示
【参数值】无
【返回值】无 
【最后更新】2021年1月22日 
【软件版本】V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_ComKEY_Tft(void)
{
  unsigned char k=0xFF;
  OLED_Init();                                              	// OLED初始化
  OLED_CLS();                                               	// 清屏		
  OLED_P14x16Str(0,0,(unsigned char*)"北京龙邱智能科技");		// 字符串显示
  OLED_P6x8Str(0,2,"Long Qiu i.s.t.");	                  	// 字符串显示
  delayms(1000);                                            	// 延时等待
  OLED_CLS();                                             	// 清屏		
  OLED_P6x8Str(0,1,"KEY Pressed:  ");	                    	// 字符串显示
  GPIO_KEY_Init();
  while(1)
  {
    k= KEY_Read_Comb();
    switch(k)
    {
    case NOKEYDOWN:
      //OLED_P6x8Str(35,3,"NO key!  ");			// 字符串显示
      break;
    case KEY0DOWN:
      OLED_P6x8Str(35,3,"KEY0!    ");		  	// 字符串显示
      break;
    case KEY1DOWN:
      OLED_P6x8Str(35,3,"KEY1!    ");			// 字符串显示
      break;
    case KEY2DOWN:
      OLED_P6x8Str(35,3,"KEY2!    ");		  	// 字符串显示
      break;	
    case KEY01DOWN:
      OLED_P6x8Str(35,3,"KEY0&1!  ");		  	// 字符串显示
      break;
    case KEY02DOWN:
      OLED_P6x8Str(35,3,"KEY0&2!  ");		  	// 字符串显示
      break;
    case KEY12DOWN:
      OLED_P6x8Str(35,3,"KEY1&2!  ");		  	// 字符串显示
      break;
    case KEY012DOWN:
      OLED_P6x8Str(35,3,"KEY0&1&2!");	  		// 字符串显示
      break;
    default:
      break;
    }
    LED_Ctrl(LED0,RVS);                     		// 四个LED同时闪烁
    delayms(100);                       			// 延时等待
  }
}



