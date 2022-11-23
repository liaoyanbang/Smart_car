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
#include "LQ_dianci.h"
#include "include.h"
short leftP=0,leftV=0,rightV=0,rightP=0;
short TempAngle = 0;   
short MotorDuty1=0; 				  // 定义左电机全局变量
short MotorDuty2=0;						// 定义右电机全局变量

short ECPULSE1 = 0;          // 定义左编码器全局变量 
short ECPULSE2 = 0;          // 定义右编码器全局变量
short MtTargetDuty =0; 	   // 电机PWM
short Kp=10;
short ServoDuty =0;



void timer0_int (void) interrupt 1
{
		ECPULSE1 = Read_Encoder(1); 			        // 左电机 母板上编码器1，小车前进为负值
		ECPULSE2 = Read_Encoder(2); 			        // 右电机 母板上编码器2，小车后退为正值
		Control();
		ServoCtrl(Servo_Center-TempAngle);
	  MotorCtrl(MotorDuty1,MotorDuty2);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void InductorNormal (void)
【功  能】电磁车演示程序
【作  者】chiusir
【最后更新】2021年1月22日 
【软件版本】V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned int data diangan[4];
void InductorNormal (void)
{
    diangan[0]=ADC_Read(8);    	// 读取ADC8通道值   P00
    diangan[1]=ADC_Read(9);			// 读取ADC9通道值	  P01
    diangan[2]=ADC_Read(10); 		// 读取ADC10通道值	P02
    diangan[3]=ADC_Read(11);		// 读取ADC11通道值	P03
		
		leftP =  (float)(diangan[0] - 100.0) / (2300.0 - 100.0) * 100.0;		// 电感归一化
		leftV =  (float)(diangan[1] - 100.0) / (2300.0 - 100.0) * 100.0;		// 电感归一化
		rightV = (float)(diangan[2] - 100.0) / (2300.0 - 100.0) * 100.0;		// 电感归一化
		rightP = (float)(diangan[3] - 100.0) / (2300.0 - 100.0) * 100.0;		// 电感归一化			
		
	  leftP = (leftP < 0) ? 0 : leftP;            //归一化后限制输出幅度
		leftP = (leftP > 100) ? 100 : leftP;				//归一化后限制输出幅度
		rightV = (rightV < 0) ? 0 : rightV;					//归一化后限制输出幅度
		rightV = (rightV > 100) ? 100 : rightV;			//归一化后限制输出幅度
	  leftV = (leftV < 0) ? 0 : leftV;						//归一化后限制输出幅度
		leftV = (leftV > 100) ? 100 : leftV;				//归一化后限制输出幅度
		rightP = (rightP < 0) ? 0 : rightP;					//归一化后限制输出幅度
		rightP = (rightP > 100) ? 100 : rightP;			//归一化后限制输出幅度			
}


int16_t Last_ServoDuty = 0; // 记忆打脚
int16_t M_error=0,Last_Target=0;      // 记忆速度
int time = 0;

int count = 1;             // 干航管触发计数
int Fork = 0,Forking=0,Forkout=0;              // 三岔路标志
int Round=0,Round_In=0,Rounding=0,Round_Out=0;
int Detection_L=0,Fork_R=0,Detection_R=0,Fork_L=0;

int RAllPulse=0;
void Control(void)
{
	InductorNormal();
	    // 圆环处理
   if(Round==1)
    {
      // Target_Speed =30;
     //  Target_Speed = 50;
        Discern();
			

    }
    // 三岔路口处理 第一圈右侧 第二圈左侧
   ////////////右岔路//////////
    else if(Fork == 1&&count==0&&Round==0&&Fork_R==0)  // 右岔路
    {
        TempAngle = -150;
       // Target_Speed = 40;
        Fork_R = 1;
			MtTargetDuty =150;

    }
    else if(rightP<5&&leftP<5&&Fork_R==1&&Forking==0)  // 判断在岔路中
   {
        Forking=1;
   }

    else if(Forking==1&&Forkout==0&&Fork_R==1)				// 岔路中循迹
    {
        TempAngle=(leftV-rightV)*220/(leftV+rightV);
       // Target_Speed = 40;
			  MtTargetDuty =150;

           if(leftP>18&&leftV>18)											// 判断岔路出口
            {
                Forkout=1;
                RAllPulse = 0;
							MtTargetDuty =180;
            }
    }
    else if(Forking==1&&Forkout==1&&Fork_R==1) 	//出岔路 清除标志位
    {
        TempAngle=(leftV-rightV)*220/(leftV+rightV);
			  MtTargetDuty =180;

            Fork = 0;
            Fork_R=0;
            Forking=0;
            Forkout=0;

    }

       ///////////// 左岔路///////
    else if(Fork == 1&&count==1&&Round==0&&Fork_L==0)  // 左岔路
    {
        TempAngle = 180;
       // Target_Speed = 20;
        Fork_L = 1;
    }

    else if(rightP<5&&leftP<5&&Fork_L==1&&Forking==0)	// 判断在岔路中
      {
           Forking=1;
      }

    else if(Forking==1&&Forkout==0&&Fork_L==1)// 岔路中循迹
      {
        TempAngle=(leftV-rightV)*200/(leftV+rightV);
        //  Target_Speed = 20;
				MtTargetDuty =180;
           if(rightV>18&&rightP>18)		// 判断岔路出口
              {
                  Forkout=1;
                  RAllPulse = 0;
              }

      }
       else if(Forking==1&&Forkout==1&&Fork_L==1) //出岔路 清除标志位
       {
           TempAngle=(leftV-rightV)*200/(leftV+rightV);
				   MtTargetDuty =180;
               Fork = 0;
               Fork_R=0;
               Fork_L=0;
               Forking=0;
               Forkout=0;
               count=0;
       }

     //正常循迹
   else
   {
            // 圆环识别
       if((Detection_L==0)&&(leftV>90)&&(rightV>80)&&(leftP<25))    //左环判断
       {

           Detection_L=1;
				   
         //  Target_Speed = 0;
       }
       else if(leftV>92&&rightV>70&&Round==0&&Detection_L==1&&(leftP>45)) //左环进
       {
           Round = 1;
           Round_In = 1;
			    	

       }
            // 三岔路口识别
          //  
      else if(leftP>15&&leftV<50&&rightV<50&&rightP>15&&Round==0&&Fork==0&&Forkout==0) //识别三岔路口
            {
                Fork = 1;
                MtTargetDuty =100;
            }
      else if(Round == 0&&Fork == 0)			// 正常循迹
            {
							if((leftV<5)&&(rightV<5))
							{
								MtTargetDuty = 0;
							}
							else if((leftV>15)&&(rightV>15))
							{
								TempAngle=(leftV-rightV)*140/(leftV+rightV);
								MtTargetDuty = 250;
							}
							else 
							{
								TempAngle=(leftV-rightV)*330/(leftV+rightV);
								MtTargetDuty =200;
							}
            }
    }

    // 电机闭环控制

	if(TempAngle>150)TempAngle=150;
	if(TempAngle<-150)TempAngle=-150;
	if(TempAngle<0)
	{
		MotorDuty1 = MtTargetDuty +TempAngle/2;//差速 
		MotorDuty2 = MtTargetDuty ;	
	}
		if(TempAngle>0)
	{
		MotorDuty1 = MtTargetDuty ;
		MotorDuty2 = MtTargetDuty-TempAngle/2 ;	
	}
}


void Discern(void) // 识别圆环 此圆环只写了左环 右环自己参考书写
{
    if(Round_In==1)  // 进圆环
    {
        TempAngle=(leftP-rightP)*400/(leftP+rightP);	//切换垂直电感循迹
		  	MtTargetDuty =0;
    }
    if((rightP<20)&&(rightV<30)&&Round_In==1)					// 判断在圆环中
    {

        Rounding=1;
        Round_In=0;
    }
    else if(Rounding==1)												// 圆环中循迹 切回水平电感循迹
    {
        TempAngle=(leftV-rightV)*200/(leftV+rightV);
			  MtTargetDuty =200;
    }
    if(Rounding==1&&((leftP>80&&leftV>80)))
    {
        Round_Out = 1;
        RAllPulse = 0;
    }
    if(Round_Out == 1)
    {

        TempAngle=(leftV-rightV)*280/(leftV+rightV);
			  MtTargetDuty =220;
        if(leftP<60&&leftV<60)           // 出圆环 清楚标志位
        {
         
            Round = 0;
            Rounding=0;
            Round_Out = 0;
            RAllPulse = 0;
            Detection_R=0;
        }
    }
}







/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void OLED_show (void)
【功  能】OLED显示函数
【作  者】chiusir
【最后更新】2021年12月22日 
【软件版本】V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void OLED_show(void)
{
	  char txt[30];
		sprintf(txt,"LP:%03d RP:%03d",leftP,rightP);		// 显示ADC8的值
    OLED_P6x8Str(0,0,(u8*)txt);
    sprintf(txt,"LV:%03d RV:%03d",leftV,rightV);		// 显示ADC9的值
    OLED_P6x8Str(0,1,(u8*)txt);
		sprintf(txt,"E:%04d %04d",ECPULSE1,ECPULSE2);		// 显示ADC11的值
    OLED_P6x8Str(0,2,(u8*)txt);
		sprintf(txt,"M:%04d Mt:%04d",MotorDuty1,MotorDuty2);	// 显示ADC11的值
    OLED_P6x8Str(0,3,(u8*)txt);
		sprintf(txt,"T:%04d",TempAngle);	// 显示ADC11的值
    OLED_P6x8Str(0,4,(u8*)txt);
}
