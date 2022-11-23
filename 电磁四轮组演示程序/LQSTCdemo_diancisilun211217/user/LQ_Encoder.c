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
【功  能】单位时间读取编码器计数
【参数值】u8 encno 编码器选择
【返回值】脉冲数，注意读取之间周期内，不要超过16位最大值
【最后更新】2021年1月22日 
【软件版本】V1.0
【使用举例】x=ADC_Read(8); //读取ADC8通道值
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
short Read_Encoder(u8 encno)
{
  short data tm=0;
  if(encno==1)
  {
    if(P07)//编码器1计数A:P06  DIR:P07 	
      tm=(T4H<<8)|T4L;	
    else 
      tm=0-(T4H<<8)|T4L;	
    
    T4T3M &= ~(1<<7);
    T4H = 0;
    T4L = 0;
    T4T3M |= (1<<7);		
  }
  else if(encno==2)
  {
    if(P05)//编码器２计数A:P04  DIR:P05 	
      tm=(T3H<<8)|T3L;	
    else
      tm=0-(T3H<<8)|T3L;	
    
    T4T3M &= ~(1<<3);
    T3H = 0;
    T3L = 0;
    T4T3M |= (1<<3);		
  }	
  return tm;
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void Timer4EncInit(void)
【功  能】编码器初始化函数
【参数值】无
【返回值】无
【最后更新】2021年1月22日 
【软件版本】V1.0
【使用举例】2个编码器
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Timer34EncInit (void)
{   
  /*
  T4T3M &= ~0xf0;     	//停止计数, 定时模式, 12T模式, 不输出时钟  
  T4T3M &= ~0x01;    		//不输出时钟
  T4T3M |=  (1<<2);    	//12T mode ;P0.4计数
  T3H = 0;
  T3L = 0;
  
  T4T3M |=  (1<<3);     //开始运行
  //ET3 = 1;   					//允许中断
  
  T4T3M &= ~0x10;    	  //不输出时钟
  T4T3M |=  (1<<6);     //12T mode ;P0.6计数
  T4H = 0;
  T4L = 0; 
  T4T3M |=  (1<<7);     //开始运行
  //ET4 = 1;   					//允许中断	  
  */
  T3L = 0; 
  T3H = 0;
  T4T3M |= 0x0c; 
  
  T4L = 0;
  T4H = 0;
  T4T3M |= 0xc0; 
}


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void TestEncoder(void)
【功  能】测试程序，TFT1.8显示
【参数值】无
【返回值】无
【最后更新】2021年1月22日 
【软件版本】V1.0
【使用举例】
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TestEncoder(void)
{
  char txt[16];
  short duty = 20;
  short enc1,enc2;
  
//  OLED_CLS();   	                         	// 清屏
//  OLED_P6x8Str(0, 0, "Test Encoder");       	// 字符串显示
//  Timer34EncInit();                          	// 编码器接口初始化
  MotorInit(MOTOR_FREQUENCY);    				// 电机初始化
  while (1)
  {
    if (KEY_Read(KEY0) == 0)      // 按下KEY0键，占空比减小
    {
      if (duty > -PWM2_CLOCK/MOTOR_FREQUENCY)	// 满占空比为10M/12.5k=800
        duty -= 50;
    }
    if (KEY_Read(KEY2) == 0)      // 按下KEY2键，占空比加大
    {
      if (duty < PWM2_CLOCK/MOTOR_FREQUENCY)  // 满占空比为10M/12.5k=800
        duty += 50;
    }
    if (KEY_Read(KEY1) == 0)      // 按下KEY1键，占空比中值
    {
      duty = 20;
    }
    
    MotorCtrl(duty,duty);
    
    /* 获取编码器值 */
    enc1 = Read_Encoder(1); 			        // 左电机 母板上编码器1，小车前进为负值
    enc2 = Read_Encoder(2); 			        // 右电机 母板上编码器2，小车后退为正值
    sprintf(txt, "Enc1: %04d  ", enc1);
    OLED_P6x8Str(0, 3, txt);			        // 字符串显示
    sprintf(txt, "PWM: %05d;", duty);
    OLED_P6x8Str(0, 4, txt);   					// 字符串显示
    sprintf(txt, "Enc2: %04d  ", enc2);
    OLED_P6x8Str(0, 5, txt);			        // 字符串显示
    
    LED_Ctrl(LED0, RVS);       		        	// 电平翻转,LED闪烁
    delayms(50);							 	// 延时等待
  }
}
//