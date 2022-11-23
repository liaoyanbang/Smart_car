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
【函数名】ADC1_SingleChannel_Get(uint8_t ADC_Channel_x) 
【功  能】获取ADC转换数据
【参数值】ADC_Channel_x , x为0~11
【返回值】ADC读到的值，默认是12位
【最后更新】2021年1月22日 
【软件版本】V1.0
【使用举例】x=ADC_Read(8); //读取ADC8通道值
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
u16 ADC_Read(uint8 ADC_Channel_x)
{ 
  unsigned short data temp;
	
  ADC_RES = 0;
  ADC_RESL = 0;
  
  ADC_CONTR &= 0xF0;
  ADC_CONTR = ADC_CONTR | 0x40 | ADC_Channel_x;    //启动 AD 转换
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  
  while((ADC_CONTR & 0x20) == 0)  ;   //wait for ADC finish
  ADC_CONTR &= ~0x20;     			//清除ADC结束标志
  temp = ADC_RES & 0x0F;
  temp = (temp<<8) | ADC_RESL;
  return temp;						//默认是12位
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】u16 ADC_Read_Average(uint8 ADC_Channel_x,uint8 times)
【功  能】获取几次ADC1采样值的平均值
【参数值】ADC_Channel_x , x为0~11
【返回值】ADC读到的值
【作  者】chiusir
【最后更新】2021年1月22日 
【软件版本】V1.0
【使用举例】x=ADC_Read(8,10); //读取ADC8通道值,10次均值
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
u16 ADC_Read_Average(uint8 ADC_Channel_x,uint8 times)
{
  u32 temp_val=0;
  u8 t;
  u8 delay;
  for(t=0;t<times;t++)
  {
    temp_val+=ADC_Read(ADC_Channel_x);
    for(delay=0;delay<100;delay++);
  }
  return temp_val/times;
} 

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void ADC_Init (void)
【功  能】四个电感ADC初始化函数；
【参数值】无
【返回值】无 
【作  者】chiusir
【最后更新】2021年1月22日 
【软件版本】V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void ADC_Init (void)
{
  P_SW2 |= 0x80;
  ADCTIM = 0x3f;					//设置 ADC 内部时序，ADC采样时间建议设最大值
  P_SW2 &= 0x7f;
  ADCCFG |= 0x2f;					//设置 ADC 时钟为系统时钟/2/16/16
  ADC_CONTR = 0x80; 			//使能 ADC 模块
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void Test_ADC(void)
【功  能】测试程序，屏幕显示ADC采集的电感值
【参数值】无
【返回值】无 
【作  者】chiusir
【最后更新】2021年1月22日 
【软件版本】V1.0
【使用举例】
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_ADC(void)
{
  char txt[30]; 
  unsigned int data diangan[4];
	
  OLED_Init();    // OLED初始化
  OLED_CLS();  	// 清屏		
  OLED_P14x16Str(0,2,(unsigned char*)"北京龙邱智能科技");		// 字符串显示
  OLED_P6x8Str(0,4,"Long Qiu i.s.t.");						// 字符串显示
  delayms(1000);  // 延时等待
  OLED_CLS();     // 清屏		
  ADC_Init();		// ADC初始化
  while(1)
  {
    diangan[0]=ADC_Read(8);    		// 读取ADC8通道值
    diangan[1]=ADC_Read(9);			// 读取ADC9通道值
    diangan[2]=ADC_Read(10); 		// 读取ADC10通道值
    diangan[3]=ADC_Read(11);		// 读取ADC11通道值	
    sprintf(txt,"AD0:%05d ",diangan[0]);	// 显示ADC8的值
    OLED_P8x16Str(0,0,(u8*)txt);
    sprintf(txt,"AD1:%05d ",diangan[1]);	// 显示ADC9的值
    OLED_P8x16Str(0,2,(u8*)txt);
    sprintf(txt,"AD2:%05d ",diangan[2]);	// 显示ADC10的值
    OLED_P8x16Str(0,4,(u8*)txt);
    sprintf(txt,"AD3:%05d ",diangan[3]);	// 显示ADC11的值
    OLED_P8x16Str(0,6,(u8*)txt);
  }
}











