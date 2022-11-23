///*********************************************************************************************************************
// * COPYRIGHT NOTICE
// * Copyright (c) 2020,逐飞科技
// * All rights reserved.
// * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
// *
// * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
// * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
// *
// * @file       		isr
// * @company	   		成都逐飞科技有限公司
// * @author     		逐飞科技(QQ790875685)
// * @version    		查看doc内version文件 版本说明
// * @Software 			MDK FOR C251 V5.60
// * @Target core		STC16F40K128
// * @Taobao   			https://seekfree.taobao.com/
// * @date       		2020-4-14
// ********************************************************************************************************************/
#include "headfile.h"

//UART1中断
void UART1_Isr() interrupt 4
{
    uint8 res;
	static uint8 dwon_count;
    if(UART1_GET_TX_FLAG)
    {
        UART1_CLEAR_TX_FLAG;
        busy[1] = 0;
    }
    if(UART1_GET_RX_FLAG)
    {
        UART1_CLEAR_RX_FLAG;
        res = SBUF;
        //程序自动下载
        if(res == 0x7F)
        {
            if(dwon_count++ > 20)
                IAP_CONTR = 0x60;
        }
        else
        {
            dwon_count = 0;
        }
    }
}

//UART2中断
void UART2_Isr() interrupt 8
{
    if(UART2_GET_TX_FLAG)
	{
        UART2_CLEAR_TX_FLAG;
				busy[2] = 0;
	}
    if(UART2_GET_RX_FLAG)
	{
        UART2_CLEAR_RX_FLAG;
		//接收数据寄存器为：S2BUF
	}
}


//UART3中断
void UART3_Isr() interrupt 17
{
    if(UART3_GET_TX_FLAG)
	{
        UART3_CLEAR_TX_FLAG;
				busy[3] = 0;
	}
    if(UART3_GET_RX_FLAG)
	{
        UART3_CLEAR_RX_FLAG;
		//接收数据寄存器为：S3BUF
	}
}


//UART4中断
void UART4_Isr() interrupt 18
{
    if(UART4_GET_TX_FLAG)
	{
        UART4_CLEAR_TX_FLAG;
				busy[4] = 0;
	}
    if(UART4_GET_RX_FLAG)
	{
        UART4_CLEAR_RX_FLAG;

		//接收数据寄存器为：S4BUF;
		if(wireless_type == WIRELESS_SI24R1)
    {
       wireless_uart_callback();           //无线转串口回调函数
    }
    else if(wireless_type == WIRELESS_CH9141)
    {
       bluetooth_ch9141_uart_callback();   //蓝牙转串口回调函数
    }
	}
}


#define Servo_Middle 645
short TempAngle = 0;   

//轮胎周长20.5cm·  一周2320个脉冲   1m/s  = 4.88r/s  =11317个脉冲 / s   5ms的周期 应该给 11317/200个脉冲 = 57  2m/s 为114  
short GoalPulseLL=80; 				  // 定义左电机全局显示变量
short GoalPulseRR=80;					// 定义右电机全局显示变量
short GoalPulseL=200; 				  // 定义左电机全局输入量
short GoalPulseR=200;						// 定义右电机全局输入量

short MotorDutyL;             // 定义左电机全局输出量
short MotorDutyR;             // 定义右电机全局输出量

short PulseL = 0;          // 定义左编码器全局变量 
short PulseR = 0;          // 定义右编码器全局变量
short MtTargetDuty =0; 	   // 电机PWM

short ServoDuty =Servo_Middle;
short Last_Duty = 0;
short chashu = 10;
float f_duoji;
float f_tanA;

#define Servo_Left_Max 715
#define Servo_Right_Min 550
#define DIR1 P44  //左轮方向
#define DIR2 P63  //右轮方向
#define LED P52
short flag_star =0;
void INT0_Isr() interrupt 0
{
	delay_ms(255);
	LED = !LED;	//反转LED
	if(flag_star)
		flag_star =0;
	else flag_star = 1;
}
void INT1_Isr() interrupt 2
{
	delay_ms(255);
	LED = !LED;	//反转LED
}
void INT2_Isr() interrupt 10
{
	delay_ms(500);
	LED = !LED;	//反转LED
	INT2_CLEAR_FLAG;  //清除中断标志
}
void INT3_Isr() interrupt 11
{
	delay_ms(500);
	LED = !LED;	//反转LED
	INT3_CLEAR_FLAG;  //清除中断标志
}

void INT4_Isr() interrupt 16
{
	INT4_CLEAR_FLAG;  //清除中断标志
}


int16 dig1,dig2,dig3;// dig1内侧差值  dig2外侧水平差值  dig3外侧垂直差值
int16 angle;
short count =0;
short jianshu_flag=0,jiashu_flag=0;
int16 AD_V[7];//电感值
int16 AD[7];//归一化的值 0~100
float sensor_to_one[7];//归一化的值 0~1
enum 
{
	Normal = 1,
	Fork_L,
	Fork_R,
	Round_L,
	Round_R,
	Cross,
	Descent
} Position;

enum 
{
	Forking=1,
	Fork_In,
	Forkout
} Fork_Process;

enum 
{
	Round_In=1,
	Rounding,
	Round_Out
} Round_Process;

enum 
{
	Crossing=1,
	Crossover
} Cross_Process;

enum 
{
	Descenting=1,
	Descentover
}Descent_Process;

void Read_ADC(void)
{
	int16  i,j,k,temp;
  int16  ad_valu[7][5],ad_sum[7];
	for(i=0;i<5;i++)
	{
		ad_valu[0][i]=adc_once(ADC_P00,ADC_10BIT);
		ad_valu[1][i]=adc_once(ADC_P01,ADC_10BIT);
		ad_valu[2][i]=adc_once(ADC_P05,ADC_10BIT);
		ad_valu[3][i]=adc_once(ADC_P06,ADC_10BIT);//
		ad_valu[4][i]=adc_once(ADC_P13,ADC_10BIT);//
		ad_valu[5][i]=adc_once(ADC_P14,ADC_10BIT);
		ad_valu[6][i]=adc_once(ADC_P15,ADC_10BIT);
	}
	//冒泡排序///
      for(i=0;i<7;i++)     //7个电感     
      {
         for(j=0;j<4;j++)  //五个数据排序
         {
            for(k=0;k<4-j;k++)
            {
               if(ad_valu[i][k] > ad_valu[i][k+1])  //前面的比后面的大  则进行交换
               {
                  temp = ad_valu[i][k+1];
                  ad_valu[i][k+1] = ad_valu[i][k];
                  ad_valu[i][k] = temp;
               }
            }
         }
      }
      for(i=0;i<7;i++)    //求中间三项的和
      {
         ad_sum[i] = ad_valu[i][1] + ad_valu[i][2] + ad_valu[i][3];       
         AD_V[i] = ad_sum[i] / 3.0;
      }
}
int16 min_v = 0;
int16 max_v = 700;
void Date_analyse()
{ 
     int16  i; 
     static int16 max_old = 1,max_crosstalk = 1;
     static int16 position_last = 2;
     Read_ADC();
     
     /*********************归一化处理********************/
     for(i=0;i<7;i++) 
     {
				//AD_V[i] = 2;
        sensor_to_one[i] = (float)(AD_V[i] - min_v)/(float)(max_v - min_v); //最大最小值可改为数组 
        if(sensor_to_one[i]<=0.0)  sensor_to_one[i]=0.001;
        if(sensor_to_one[i]>1.0)  sensor_to_one[i]=1.0; 
        AD[i] = 100*sensor_to_one[i];     //AD[i]为归一化后的值  范围为0-100
		}
		dig1 = AD[2]-AD[4];
		dig2 = AD[1]-AD[5];
		dig3 = AD[0]-AD[6];
		 
}		 
void TM0_Isr() interrupt 1
{	
}
float yaw;
short PulseLL = 0;
void TM1_Isr() interrupt 3
{
		static int16 flag = 0;
//		static time  = 0 ;
//		time++;
//		if(time==20)
//		{
//		if(flag==0)
//		{
//			GoalPulseL+=10;
//			GoalPulseR+=10;
//			if(GoalPulseL>=100)
//			{
//				flag = 1;
//			}
//		}
//		else 
//		{
//			GoalPulseL-=10;
//			GoalPulseR-=10;
//			if(GoalPulseL<=30)
//			{
//				flag = 0;
//			}
//		}
//		time=0;
		//}
		PulseL = ctimer_count_read(CTIM0_P34);//读取编码器的值
		//PulseLL += PulseL;
		PulseR = 0- ctimer_count_read(CTIM3_P04);
		if(flag_star)
		{
				MotorDutyL = Incremental_PI_L(PulseL,GoalPulseL);
				MotorDutyR = Incremental_PI_R(PulseR,GoalPulseR);
		}
		
		if(MotorDutyL>=0) DIR1=1;
		else {DIR1=0; MotorDutyL= 0-MotorDutyL;}
		if(MotorDutyR>=0) DIR2=1;
		else {DIR2=0;	MotorDutyR= 0-MotorDutyR;}
		if(flag_star==0)
		{
			MotorDutyL=MotorDutyR=0;
		}
		
		pwm_duty(PWMA_CH2P_P62,MotorDutyL);  //左边电机
		pwm_duty(PWMA_CH1P_P60,MotorDutyR);  //右边电机
		
}

void TM2_Isr() interrupt 12
{
	TIM2_CLEAR_FLAG;  //清除中断标志
}
void TM3_Isr() interrupt 19
{
	TIM3_CLEAR_FLAG; //清除中断标志
	
}

void TM4_Isr() interrupt 20
{
	TIM4_CLEAR_FLAG; //清除中断标志
//	ccd_collect();	 //CCD采集数据

}

void Control(void)
{
	switch(Position)  //状态机
	{
		case Normal:
		{
			Normal_handle();
			break;
		}
		case Fork_R:
		{
			Fork_Rhandle();
			break;
		}
		case Fork_L:
		{
			Fork_Lhandle();
			break;
		}
		case Round_L:
		{
			Round_Lhandle();
			break;
		}
		case Round_R:
		{
			Round_Rhandle();
			break;
		}
		case Cross:
		{
			Cross_handle();
			break;
		}
		case Descent:
		{
			Descent_handle();
			break;
		}
		default : 
		{
			Position = Normal;
			break;
		}
	}
}
void Normal_handle(void)//正常巡线处理
{
	//pid闭环控制舵机
		if(dig1<15 && dig1>-15)
		{
			ServoDuty = Servo_Middle + (-dig1)*0.5;
		}
		else  pid_duoji();
		if(AD[3]<=20)//丢线处理
		{
			ServoDuty = Servo_Middle + (-dig3)*4.0;
		}
}

void Round_Rhandle(void) // 右环处理
{
	switch(Round_Process)
	{
		case Round_In:
		{
			ServoDuty = Servo_Middle ;
			if(AD[0]>=80)//即将进环
			{
				Round_Process = Rounding;
				ServoDuty = Servo_Middle - 35;
			}
			break;
		}
		case Rounding:
		{
			//右环循迹
			ServoDuty = Servo_Middle + (-dig3)*1.3;
			if(AD[1]<=60 && AD[2]<=60 && AD[3]>=60)//判断出环
			{
				Round_Process = Round_Out ;
			}
			break;
		}
		case Round_Out:
		{
			ServoDuty = Servo_Middle + 35;
			Position = Normal;
			break;
		}
	}
}
void Round_Lhandle(void) // 左环处理
{
	switch(Round_Process)
	{
		case Round_In:
		{
			ServoDuty = Servo_Middle ;
			if(AD[6]>=80)//即将进环
			{
				Round_Process = Rounding;
				ServoDuty = Servo_Middle + 35;
			}
			break;
		}
		case Rounding:
		{
			//左环循迹
			ServoDuty = Servo_Middle + (-dig3)*1.3;//需要再改
			if(AD[6]<=60 && AD[5]<=60 &&AD[3]>=60) //判断出环 保证不是因为丢线造成的
			{
				Round_Process = Round_Out ;
			}
			break;
		}
		case Round_Out:
		{
			ServoDuty = Servo_Middle -35;
			Position = Normal;
			break;
		}
	}
}

void Fork_Rhandle(void)
{
	switch(Fork_Process)
	{
		case Fork_In:
		{
			ServoDuty = Servo_Middle - 70;
			if(AD[6]>=45 &&AD[5]>=45 && AD[0]<=20 && AD[1]<=20)//判断是否在岔路中
			{
				Fork_Process = Forking;
			}
			break;
		}
		case Forking:
		{
			//右岔路循迹
			ServoDuty = Servo_Middle + (-dig3)*0.8;
			if(AD[1]>=85) //判断出口 
			{
				Fork_Process = Forkout; 
				count=0;
			}
			break;
		}
		case Forkout:
		{
			Position = Normal;
			break;
		}
	}
}
void Fork_Lhandle(void)
{
	switch(Fork_Process)
	{
		case Fork_In:
		{
			ServoDuty = Servo_Middle + 70;
			if(AD[0]>=45&&AD[1]>=45&&AD[5]<=20&&AD[6]<=20)//判断是否在岔路中
			{
				Fork_Process = Forking ;
			}
			break;
		}
		case Forking:
		{
			//左岔路循迹
			ServoDuty = Servo_Middle + (-dig3)*0.8;
			if(AD[5]>=85)  //判断出口
			{  
				Fork_Process = Forkout;
				count = 1;
			}
			break;
		}
		case Forkout:
		{
			Position = Normal;
			break;
		}
	}
}

void Cross_handle(void)
{
	switch(Cross_Process)
	{
		case Crossing:
		{
			//直走
			ServoDuty=Servo_Middle;
			if(AD[0]<20 || AD[6] <20)
			{
				Cross_Process = Crossover;
			}
			break;
		}
		case Crossover:
		{
			Position = Normal;
			break;
		}
	}
}
void Descent_handle(void)
{
	switch(Descent_Process)
	{
		case Descenting:
		{
			jiashu_flag = 1;
			//ServoDuty = Servo_Middle+ (-dig1)*0.6;//坡道循迹
			if(AD[3]<85)
			{
				Descent_Process = Descentover;
			}
			break;
		}
		case Descentover:
		{
			jiashu_flag = 0;
			Position = Normal;
			break;
		}
	}
}
void TFTLCD_show(void)
{
	char txt[30];
//	sprintf((char*)txt,"Yaw:%.1f %.1f     ",yaw,icm_acc_z);	// 显示角度
//  lcd_showstr_simspi(0,0,(char*)txt);
//	switch(Position)
//	{
//			case Normal:
//			{
//				sprintf(txt,"Position: Normal ");
//				break;
//			}
//			case Round_R:
//			{
//				sprintf(txt,"Position: Round_R ");		//赛道元素
//				break;
//			}
//			case Round_L:
//			{
//				sprintf(txt,"Position: Round_L ");		//赛道元素
//				break;
//			}
//			case Fork_L:
//			{
//				sprintf(txt,"Position: Fork_L ");	
//				break;
//			}
//			case Fork_R:
//			{
//				sprintf(txt,"Position: Fork_R ");	
//				break;
//			}
//			case Cross:
//			{
//				sprintf(txt,"Position: Cross ");	
//				break;
//			}
//			case Descent:
//			{
//				sprintf(txt,"Position: Descent ");	
//				break;
//			}
//	}
	sprintf(txt,"P:%.2f I:%.2f %.2f    ",Velocity_KP_L,Velocity_KI_L,Velocity_KD_L);	//显示kpki的值
  lcd_showstr_simspi(0,1,(char*)txt);
//	sprintf(txt,"L:%03d R:%03d L:%d      ",PulseL,PulseR,PulseLL);	//显示编码器的值
//  lcd_showstr_simspi(0,2,(char*)txt);
//	sprintf(txt,"L:%03d R:%03d S:%.2f    ",MotorDutyL,MotorDutyR,f_duoji);	//显示电机pwm的值
//  lcd_showstr_simspi(0,3,(char*)txt);
//	sprintf(txt,"T:%.2f %d %d %d       ",f_tanA,AD_V[6],AD_V[5],AD_V[4]);	// 显示舵机的角度
//  lcd_showstr_simspi(0,4,(char*)txt);
//	sprintf((char*)txt,"%d %d %d %d       ",AD_V[3],AD_V[2],AD_V[1],AD_V[0]);	// 显示电感的值
//  lcd_showstr_simspi(0,5,(char*)txt);
//	sprintf(txt,"%02d %02d %02d %02d %02d %02d %02d   ",AD[6],AD[5],AD[4],AD[3],AD[2],AD[1],AD[0]);	// 显示电感归一化的值
//  lcd_showstr_simspi(0,6,(char*)txt);
}
void Send_Message(void)
{
	switch(Position)
	{
			case Round_R:
			{
				bluetooth_ch9141_send_buff("Round_R\r\n",8);
				break;
			}
			case Round_L:
			{
				bluetooth_ch9141_send_buff("Round_L\r\n",8);
				break;
			}
			case Fork_L:
			{
				bluetooth_ch9141_send_buff("Fork_L\r\n",7);
				break;
			}
			case Fork_R:
			{
				bluetooth_ch9141_send_buff("Fork_R\r\n",7);
				break;
			}
			case Cross:
			{
				bluetooth_ch9141_send_buff("Cross\r\n",6);
				break;
			}
			case Descent:
			{
				bluetooth_ch9141_send_buff("Descent\r\n",8);
				break;
			}
	}
}
float duojikp = 0.7;
float duojikd = 1.1;
void pid_duoji()
{
	static int16 error = 0;
	static int16 last_error = 0;
	duojikp = Fuzzy_Update(&Duoji_P,error,last_error-error);
	ServoDuty = Servo_Middle - duojikp * error - duojikd * (last_error-error);
	last_error = error;
	error = dig3;
}
//void  INT0_Isr()  interrupt 0;
//void  TM0_Isr()   interrupt 1;
//void  INT1_Isr()  interrupt 2;
//void  TM1_Isr()   interrupt 3;
//void  UART1_Isr() interrupt 4;
//void  ADC_Isr()   interrupt 5;
//void  LVD_Isr()   interrupt 6;
//void  PCA_Isr()   interrupt 7;
//void  UART2_Isr() interrupt 8;
//void  SPI_Isr()   interrupt 9;
//void  INT2_Isr()  interrupt 10;
//void  INT3_Isr()  interrupt 11;
//void  TM2_Isr()   interrupt 12;
//void  INT4_Isr()  interrupt 16;
//void  UART3_Isr() interrupt 17;
//void  UART4_Isr() interrupt 18;
//void  TM3_Isr()   interrupt 19;
//void  TM4_Isr()   interrupt 20;
//void  CMP_Isr()   interrupt 21;
//void  I2C_Isr()   interrupt 24;
//void  USB_Isr()   interrupt 25;
//void  PWM1_Isr()  interrupt 26;
//void  PWM2_Isr()  interrupt 27;