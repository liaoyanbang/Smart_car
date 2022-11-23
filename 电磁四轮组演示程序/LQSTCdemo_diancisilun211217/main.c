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
芯片最后一行2038...然后IRCBND=0，下载频率为30M；
芯片最后一行2010...或者IRCBND=1，下载频率为30M；
官方出库批次不同，略有差异，好用为准！！！
=================================================================
 【注意事项】注意，一定要对舵机打角进行限制
 使用龙邱母板测试舵机流程：
 1.先使用万用表测量电池电压，务必保证电池电压在7V以上，否则无力不反应！
 2.然后确定舵机供电电压，SD5/SD012舵机用5V供电，S3010用6-7V供电！！！
 3.把舵机的舵盘去掉，让舵机可以自由转动；
 4.烧写程序并运行，让舵机转动到中值附近；如果没反应重复1-2步，或者调整舵机的PWM频率计占空比，能受控为准；
 5.舵机受控后用手轻转，舵机会吱吱响，对抗转动，此时可以装上舵盘；
 6.按键K0/K1确定舵机的左右转动极限，并记下来，作为后续限幅防止舵机堵转烧毁！
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void main(void)  
【功  能】主函数
【参数值】无
【返回值】无 
【最后更新】2021年1月22日 
【软件版本】V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void main(void)  
{	 	
  sysinit(); 	
 // UART1_config(1);						// 使用定时器2做串口4的波特率生成时钟
  OLED_Init();                // OLED初始化
 // OLED_CLS();   							// 清屏		
  OLED_P14x16Str(0,2,(unsigned char*)"北京龙邱智能科技");		// 字符串显示
  //OLED_P6x8Str(0,4,"Long Qiu i.s.t.");	// 字符串显示
  delayms(200);
  //Test_GPIO_LED();
  GPIO_KEY_Init();            // 按键初始化
	MotorInit(12500);
	ServoInit(100);
	ADC_Init();		// ADC初始化
	Timer34EncInit();                          	// 编码器接口初始化
	Timer0_init();
	Timer1_init();
  OLED_CLS();     // 清屏		
  
	EnableInterrupts;					           // 允许中断
	//Test_ComKEY_Tft();
	//Test_ADC();
	//TestMotor();
  //TestServo();
	//TestEncoder();
	//ElectroMagneticCar();
  while (1)
  {	  
		//InductorNormal();
		OLED_show();
  }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void sysinit(void)  
【功  能】部分系统功能初始化
【参数值】无
【返回值】无 
【最后更新】2021年1月22日 
【软件版本】V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void sysinit(void)  
{	
  WTST = 0; 
  P_SW2 |= 0x80;	
	// 看单片机最下面一行的前5位，2010A，30M需要 IRCBAND=1; IRCBAND=0; 频率改变
	// 看单片机最下面一行的前5位，2038A，30M需要 IRCBAND=0; 
	IRCBAND=1;       	 	    // 1：35M频段；0：24M频段
  //IRTRIM =0;            // 频率校正，用通过ISP校正
  //LIRTRIM=0;            // 频率校正，用通过ISP校正
  CLKDIV = 0;				      // 分频设置
  P_SW2 &= 0x7F;
  RSTCFG|=1<<4;   		    // 使能P5.4复位管脚  
  P0M1 =0x40; P0M0 = 0;   // 设置为准双向口,P0.6  0.7为输入口，编码器用
  P0PU =0x40;             // P0.6  0.7使能内部4.1K上拉电阻
  P1M1 = 0;   P1M0 = 0;   // 设置为准双向口
  P3M1 = 0;   P3M0 = 0;   // 设置为准双向口
  P4M1 = 0;   P4M0 = 0;   // 设置为准双向口
  P5M1 = 0;   P5M0 = 0;   // 设置为准双向口
  //P6M1 = 0;   P6M0 = 0; // 设置为准双向口，OLED用
  //P7M1 =0xFF; P7M0 = 0; // 设置为准双向口，舵机和按键使用
  P7PU =0xFF;	            // 设置为准双向口,P7.0123为输入口接按键,P7.4567为输出口接电机驱动
  P40 = 0;				        // 打开LED电源 
  EA=0;                   // 关闭总中断
}
