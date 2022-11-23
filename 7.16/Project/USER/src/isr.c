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
#define LED P52

int8 match = 0;  
uint16 distance;
uint8 dis_flag = 0;

enum position Position;
enum normal_process Normal_Process = Normal_ing;
enum fork_process Fork_Process;
enum round_process Round_Process;
enum cross_process Cross_Process;
enum descent_process Descent_Process;


#define DIR1 P44  //左轮方向
#define DIR2 P63  //右轮方向

#define FORK_InServo  50
#define FORK_OutServo  50

#define Round_Servo 45

#define FROK_ADD 0

#define Descent_Speedadd 30
#define Descent_Speedsub 30

#define FORK_Speedsub 30
//轮胎周长20.5cm·  一周2320个脉冲   1m/s  = 4.88r/s  =11317个脉冲 / s   5ms的周期 应该给 11317/200个脉冲 = 57  2m/s 为114  
//Speed_target 在isr.h中定义
short Speed_target  = 0;

short GoalPulseLL=0; 				  // 定义左电机全局设置变量
short GoalPulseRR=0;					// 定义右电机全局设置变量

short GoalPulseL=0; 				  // 定义左电机全局输入量
short GoalPulseR=0;						// 定义右电机全局输入量

short MotorDutyL =0;
short MotorDutyR = 0;

short PulseL = 0;          // 定义左编码器全局变量 
short PulseR = 0;          // 定义右编码器全局变量

int16 ServoDuty =Servo_Middle; 
//差速公式
int8 chashu_flag = 0;
float chashu = 0.6;
float f_duoji;
float f_tanA;
//开始标志位
short flag_star =0;
int16 dig1,dig2,dig3,dig4;// dig1内侧差值  dig2外侧水平差值  dig3外侧垂直差值 dig4后排垂直差值
int16 error_dig4;
int16 dig_sum=0;
float input = 0.0; //差比和
float input1 = 0.0; //差比和 开方
//圈数
short count =0;
//三岔标志位
int8 fork_flag = 1;
//十字计数器
short cross_cnt = 0;

int16 AD_V[11];//电感值
int16 AD[11];//归一化的值 0~100
float sensor_to_one[11];//归一化的值 0~1
int16 min_v = 0;
int16 max_v[11] = {0,0,0,0,0,0,0,0,0,0,0};
int16 max_fork_h[4]= {0,0,0,0};
uint16 time_5ms = 0;
int16 timeout = 0;
//是否采集adc最大值
int8 adc_get = 0;
int8 adc_get_flag = 0;
//比赛标志位
int8 match_flag = 0;
//菜单页面
uint8 view = 0;
//编码器积分
int8 cnt_star = 0;
int32 cnt = 0;
int8 cnt_left = 0;
//加减倍数
int16 beishu = 1;
//补偿标志位 补偿里昂
float ADC_Compensate = 0;//差比和补偿量 用于入环出环  进三岔 出三岔  0~10  0.2
int8 Compensate_flag = 0;//差比和补偿量标志位 在入环出环置1

//三岔中间阈值
//三岔判断阈值
int16 fork_mid_max = 0;
int16 fork_mid_min = 0;
int16 fork_side_max = 0;
int16 fork_side_min = 0;
int16 fork_max = 0;
int16 fork_turn = 0;
int16 fork_back_side_max = 0;
int16 fork_back_mid_max = 0;
int16 fork_cnt_max = 0;
//圆环
int16 circle_before_cnt = 0;//进环前
int16 circle_in_cnt = 0;//进环中
int8  cirlces_flag = 0;//多环
int16 circle = 0;//大小顺序
int16 circles_size[6]={123,132,321,312,231,213};
int16 circle_size[3]={1,2,3};
int16 circle_run_p  = 0;

float Circle_H = 0.3;
float Circle_S = 0.7;
//进环阈值
int16 circle_max = 0;
int16 circle_in_max_left = 0;
int16 circle_in_max_right = 0;
//环中阈值
int16 circle_run_max_cnt = 0;
int16 circle_out_max_cnt = 0;
//出环阈值
int16 circle_out_max = 0;
uint16 distances   = 0;
float cross2_out = 0.0;
//十字阈值
int16 Cross_sum = 0;
int16 cross_run_p = 0;
int16 cross_cnt_max = 0;
int16 cross1_in_max = 0;
int16 cross2_in_max = 0;
//十字权重
float Cross_H = 0.20;
float Cross_S = 0.80;
//记录最大偏差 以及最大偏差变化率
uint16 max_dig = 0;
uint16 max_bianhua = 0;
int16 ad3_grow_flag = 0;
//横竖电感权重
float  Normal_H = 0.80;
float  Normal_S = 0.20;

//每个速度的参数 120 125 130 135 140 145 150 155   （8个） 8*8*2=128btye
int16 temp_normal_kp[8];
int16 temp_fork_turn[8];
int16 temp_cross_run_p[8];
int16 temp_cross1_in_max[8];
int16 temp_cross2_in_max[8];
int16 temp_circle_in_max_right[8];
float temp_ADC_Compensate[8];
int16 temp_circle_run_p[8];

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
//该函数在isr.c文件中的UART3_Isr函数进行回调
void Split_Ultrasonic_callback(uint8 buf)
{
    static uint8 dat[3];
    static uint8 num = 0;
    
		if(dis_flag==0)
		{
			dat[num] = buf;
			if(dat[0] != 0xa5)  num = 0;//检查头帧是否正确，不正确就重新接收
			else                num++;
			
			if(num==3)                  //接收完成，开始处理数据
			{
					num = 0;
					//将数据的高八位与低八位进行组合，得到距离信息单位MM
					distance = dat[1]<<8	 | dat[2];
					dis_flag = 0;
			}
		}
}
void UART3_Isr() interrupt 17 //超声波
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
		   Split_Ultrasonic_callback(S3BUF);
	}

}
void UART4_Isr() interrupt 18 //蓝牙接收中断
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
		if(wireless_type == WIRELESS_CH9141)
    {
			res= BLUETOOTH_CH9141_DATA_BUF;
			if(res == 'G')
			{
				if(match)
				Star_handle();
			}
      // bluetooth_ch9141_uart_callback();   //蓝牙转串口回调函数
    }
	}
}
void INT0_Isr() interrupt 0
{
	LED = !LED;	//反转LED
	if(flag_star)
		flag_star =0;
	else flag_star = 1;
}
void INT1_Isr() interrupt 2
{
//	Velocity_KP+=0.1;
	LED = !LED;	//反转LED
}
void INT2_Isr() interrupt 10
{
	LED = !LED;	//反转LED
//	GoalPulseLL+=10;
//	GoalPulseRR+=10;
	
	INT2_CLEAR_FLAG;  //清除中断标志
}
void INT3_Isr() interrupt 11
{
	LED = !LED;	//反转LED
//	GoalPulseLL-=10;
//	GoalPulseRR-=10;
	INT3_CLEAR_FLAG;  //清除中断标志
}

void INT4_Isr() interrupt 16
{
	INT4_CLEAR_FLAG;  //清除中断标志
}



void ADV_Init(void)
{
	char txt[20];
	int16 i = 0, j =0;
	int16 ad_valu[11]={0,0,0,0,0,0,0,0,0,0,0};
	
	adc_get = at24c02_read_byte(0);
	match   = at24c02_read_byte(1);
	if(adc_get == 0)
	{
		for(i=0;i<11;i++)
		{
				max_v[i]      = at24c02_read_uint16(2+2*i);
				delay_ms(10);
		}
	}
	//VIEW0
	Speed_target        = at24c02_read_uint16(24);
	duojikp_normal_sqrt = at24c02_read_uint16(26);
	duojikd_normal_sqrt = at24c02_read_uint16(28);
	chashu_flag = at24c02_read_byte(46);
	chashu = (float)at24c02_read_byte(60)/10;
	cross_run_p=	at24c02_read_uint16(79);
	//view1
	Cross_H          = at24c02_read_uint16(30)/100.0;
	cross1_in_max    = at24c02_read_uint16(32);
	cross2_in_max    = at24c02_read_uint16(34);
	Circle_H         = at24c02_read_uint16(36)/100.0;
	fork_cnt_max     = at24c02_read_uint16(38);
	cross_cnt_max    = at24c02_read_uint16(40);
	cross2_out       = at24c02_read_uint16(42)/100.0;
	fork_turn        = at24c02_read_uint16(44);
	
	/*
			at24c02_write_uint16(30,Cross_H*100);
			at24c02_write_uint16(32,cross1_in_max);
			at24c02_write_uint16(34,cross2_in_max);
			at24c02_write_uint16(36,Circle_H*100);
			at24c02_write_uint16(38,fork_cnt_max);
			at24c02_write_uint16(40,cross_cnt_max);
			at24c02_write_uint16(42,cross2_out*100);
			at24c02_write_uint16(44,fork_turn);
	*/
	//view2
	circle_before_cnt     = at24c02_read_uint16(47);
	circle_in_cnt     = at24c02_read_uint16(49);
	fork_flag = at24c02_read_byte(51);
	circle_run_p       = at24c02_read_uint16(52);
	circle_in_max_left = at24c02_read_uint16(54);
	circle_in_max_right= at24c02_read_uint16(56);
	circle_out_max     = at24c02_read_uint16(58);
	ADC_Compensate     = (float)at24c02_read_uint16(61) /100;
	//view3
	Cross_sum=			at24c02_read_uint16(63);
	fork_mid_max=		at24c02_read_uint16(65);
	fork_mid_min=		at24c02_read_uint16(67);
	fork_side_max=		at24c02_read_uint16(69);
	fork_side_min=		at24c02_read_uint16(71);
	circle_run_max_cnt=		at24c02_read_uint16(73);
	circle_out_max_cnt = 		at24c02_read_uint16(75);
	circle_max  = at24c02_read_uint16(77);
	
	
	//读取对应速度的对应参数
	/*0111111
int16 temp_normal_kp[8]; 79+2*8
int16 temp_fork_turn[8]; 95+2*8
int16 temp_cross_run_p[8]; 111+2*8
int16 temp_cross1_in_max[8]; 127+2*8
int16 temp_cross2_in_max[8]; 143+2*8
int16 temp_circle_in_max_right[8]; 159+2*8
float temp_ADC_Compensate[8]; 175+2*8
int16 temp_circle_run_p[8];191 +2*8
	*/
	for(i = 0;i<8;i++)
	{
		if(Speed_target==120+5*i)
		{
			duojikp_normal_sqrt = at24c02_read_uint16(79+2*i);
			fork_turn = at24c02_read_uint16(95+2*i);
			cross_run_p = at24c02_read_uint16(111+2*i);
			cross1_in_max = at24c02_read_uint16(127+2*i);
			cross2_in_max = at24c02_read_uint16(143+2*i);
			circle_in_max_right = at24c02_read_uint16(159+2*i);
			ADC_Compensate = at24c02_read_uint16(175+2*i)/100.0;
			circle_run_p = at24c02_read_uint16(191+2*i);0111111
		}
	}
	
	if(adc_get==1)
	{
		lcd_showstr_simspi(0,0,(char*)"Get Max adv...");
		while(1)
		{
			ad_valu[0]=adc_once(ADC_P00,ADC_10BIT);
			ad_valu[1]=adc_once(ADC_P01,ADC_10BIT);
			ad_valu[2]=adc_once(ADC_P05,ADC_10BIT);
			ad_valu[3]=adc_once(ADC_P06,ADC_10BIT);
			ad_valu[4]=adc_once(ADC_P13,ADC_10BIT);
			ad_valu[5]=adc_once(ADC_P14,ADC_10BIT);
			ad_valu[6]=adc_once(ADC_P15,ADC_10BIT);
			ad_valu[7]=adc_once(ADC_P11,ADC_10BIT);
			ad_valu[8]=adc_once(ADC_P10,ADC_10BIT);
			ad_valu[9]=adc_once(ADC_P02,ADC_10BIT);
			ad_valu[10]=adc_once(ADC_P03,ADC_10BIT);
			for(j=0;j<11;j++)
			{
				if(max_v[j]<ad_valu[j])
				{
					max_v[j] = ad_valu[j];
				}
			}
			delay_ms(5);
			sprintf((char*)txt,"7:%3d 6:%3d 5:%3d       ",max_v[6],max_v[5],max_v[4]);	// 显示电感的值
			lcd_showstr_simspi(0,2,(char*)txt);
			sprintf(txt,"4:%3d 3:%3d             ",max_v[3],max_v[2]);	// 显示电感归一化的值
			lcd_showstr_simspi(0,3,(char*)txt);
			sprintf(txt,"2:%3d 1:%3d            ",max_v[1],max_v[0]);	// 显示电感归一化的值
			lcd_showstr_simspi(0,4,(char*)txt);
			sprintf(txt,"7:%3d 8:%3d          ",max_v[7],max_v[8]);	// 显示电感归一化的值
			lcd_showstr_simspi(0,5,(char*)txt);
			sprintf(txt,"9:%3d 10:%3d          ",max_v[9],max_v[10]);	// 显示电感归一化的值
			lcd_showstr_simspi(0,6,(char*)txt);
			delay_ms(50);
			if(key_read()==1) break;
		}
		for(i=0;i<11;i++)//gai
		{
			at24c02_write_uint16(2+2*i,max_v[i]);
			delay_ms(10);
		}
		delay_ms(2000);
	}
	else
	{
			lcd_showstr_simspi(0,0,(char*)"Now Max adv...");
			sprintf((char*)txt,"6:%3d 5:%3d 4:%3d       ",max_v[6],max_v[5],max_v[4]);	// 显示电感的值
			lcd_showstr_simspi(0,2,(char*)txt);
			sprintf(txt,"3:%3d 2:%3d             ",max_v[3],max_v[2]);	// 显示电感归一化的值
			lcd_showstr_simspi(0,3,(char*)txt);
			sprintf(txt,"1:%3d 0:%3d            ",max_v[1],max_v[0]);	// 显示电感归一化的值
			lcd_showstr_simspi(0,4,(char*)txt);
			sprintf(txt,"7:%3d 8:%3d          ",max_v[7],max_v[8]);	// 显示电感归一化的值
			lcd_showstr_simspi(0,5,(char*)txt);
			sprintf(txt,"9:%3d 10:%3d          ",max_v[9],max_v[10]);	// 显示电感归一化的值
			lcd_showstr_simspi(0,6,(char*)txt);
			delay_ms(1000);
	}
	lcd_clear_simspi(BLACK);
}



void Read_ADC(void)//读取信号
{
	static int16  i = 0;
  static int16  ad_valu_v[11];
	static int16  ss[] = {0,1,2,3,4,5,6,8,9};

	ad_valu_v[0]=adc_once(ADC_P00,ADC_10BIT);
	ad_valu_v[1]=adc_once(ADC_P01,ADC_10BIT);
	ad_valu_v[2]=adc_once(ADC_P05,ADC_10BIT);
	ad_valu_v[3]=adc_once(ADC_P06,ADC_10BIT);
	ad_valu_v[4]=adc_once(ADC_P13,ADC_10BIT);
	ad_valu_v[5]=adc_once(ADC_P14,ADC_10BIT);
	ad_valu_v[6]=adc_once(ADC_P15,ADC_10BIT);
//	ad_valu_v[7]=adc_once(ADC_P11,ADC_10BIT);
	ad_valu_v[8]=adc_once(ADC_P10,ADC_10BIT);
	ad_valu_v[9]=adc_once(ADC_P02,ADC_10BIT);
//	ad_valu_v[10]=adc_once(ADC_P03,ADC_10BIT);
	for(i=1;i<5;i++)
	{
		ad_valu_v[0]+=adc_once(ADC_P00,ADC_10BIT);
		ad_valu_v[1]+=adc_once(ADC_P01,ADC_10BIT);
		ad_valu_v[2]+=adc_once(ADC_P05,ADC_10BIT);
		ad_valu_v[3]+=adc_once(ADC_P06,ADC_10BIT);//
		ad_valu_v[4]+=adc_once(ADC_P13,ADC_10BIT);//
		ad_valu_v[5]+=adc_once(ADC_P14,ADC_10BIT);
		ad_valu_v[6]+=adc_once(ADC_P15,ADC_10BIT);
//		ad_valu_v[7]+=adc_once(ADC_P11,ADC_10BIT);
		ad_valu_v[8]+=adc_once(ADC_P10,ADC_10BIT);
		ad_valu_v[9]+=adc_once(ADC_P02,ADC_10BIT);
//		ad_valu_v[10]+=adc_once(ADC_P03,ADC_10BIT);
	}
			
      for(i=0;i<(sizeof(ss)/sizeof(ss[0]));i++)    //求平均
      {      
         AD_V[ss[i]] = ad_valu_v[ss[i]] / 5;
      }
}

void Date_analyse()//信号处理
{
     static int16  i =0; 
		 static int16  aa[] = {0,1,2,3,4,5,6,8,9};
		 static int16 last_ad3 = 0;
     Read_ADC();
		 for(i =0;i<(sizeof(aa)/sizeof(aa[0]));i++)
		 {
				sensor_to_one[aa[i]] = (float)(AD_V[aa[i]] - 0)/(float)(max_v[aa[i]] - min_v); //最大最小值可改为数组 
        if(sensor_to_one[aa[i]]<=0.0)  sensor_to_one[aa[i]]=0.001;
        if(sensor_to_one[aa[i]]>1.0)  sensor_to_one[aa[i]]=1.0; 
        AD[aa[i]] = 100*sensor_to_one[aa[i]];     //AD[i]为归一化后的值  范围为0-100
		 }
     /*********************归一化处理********************/
//     for(i=0;i<7;i++) 
//     {
//        sensor_to_one[i] = (float)(AD_V[i] - min_v)/(float)(max_v[i] - min_v); //最大最小值可改为数组 
//        if(sensor_to_one[i]<=0.0)  sensor_to_one[i]=0.001;
//        if(sensor_to_one[i]>1.0)  sensor_to_one[i]=1.0; 
//        AD[i] = 100*sensor_to_one[i];     //AD[i]为归一化后的值  范围为0-100
//		}
//		if(Position == Fork_R || Position == Fork_L)
//		{
////			for(i=8;i<10;i++)
////			{
////					sensor_to_one[i] = (float)(AD_V[i] - min_v)/(float)(max_fork_h[i-7] - min_v); //最大最小值可改为数组 
////					if(sensor_to_one[i]<=0.0)  sensor_to_one[i]=0.001;
////					if(sensor_to_one[i]>1.0)  sensor_to_one[i]=1.0; 
////					AD[i] = 100*sensor_to_one[i];     //AD[i]为归一化后的值  范围为0-100
////			}
//				sensor_to_one[1] = (float)(AD_V[1] - min_v)/(float)(max_fork_h[1] - min_v); //最大最小值可改为数组 
//        if(sensor_to_one[1]<=0.0)  sensor_to_one[1]=0.001;
//        if(sensor_to_one[1]>1.0)  sensor_to_one[1]=1.0; 
//        AD[1] = 100*sensor_to_one[1];     //AD[i]为归一化后的值  范围为0-100
//			
//				sensor_to_one[5] = (float)(AD_V[5] - min_v)/(float)(max_fork_h[2] - min_v); //最大最小值可改为数组 
//        if(sensor_to_one[5]<=0.0)  sensor_to_one[5]=0.001;
//        if(sensor_to_one[5]>1.0)  sensor_to_one[5]=1.0; 
//        AD[5] = 100*sensor_to_one[5];     //AD[i]为归一化后的值  范围为0-100
//		}
			if(AD[3]-last_ad3>0)
			{
				ad3_grow_flag = 1;
			}
			else 
			{
				ad3_grow_flag = 0;
			}
			last_ad3  = AD[3];
		
		
}		 
void TM0_Isr() interrupt 1
{	
}
void TM1_Isr() interrupt 3  
{		
}
void TM2_Isr() interrupt 12 
{
	Turn_control(); 
	TIM2_CLEAR_FLAG;  //清除中断标志
}
void TM3_Isr() interrupt 19
{
	TIM3_CLEAR_FLAG; //清除中断标志
}



void TM4_Isr() interrupt 20//速度环
{
		static int16 flag = 0;
		static int16 time_50ms =0;
		//time_50ms++;
		if(AD[9]<=10&& AD[8]<=10)//停车  
		{
			timeout++;
			if(timeout>=20&&timeout<100)
			{	
				GoalPulseLL = 0;
				GoalPulseRR = 0;
				//DisableGlobalIRQ();
			}
			else if(timeout>=100)
			{
				flag_star=0;
			}
		}
		PulseL = ctimer_count_read(CTIM0_P34);//读取编码器的值
		PulseR = 0- ctimer_count_read(CTIM3_P04);
		if(cnt_star) 
		{
			if(cnt_left)
			{
				cnt += PulseL;
			}
			else
			{
				cnt += PulseR;
			}
		}
		if(chashu_flag)
		{
			f_duoji = (Servo_Middle - ServoDuty) / 180.0;  //阿克曼变速
			f_tanA = chashu* (f_duoji + f_duoji * f_duoji * f_duoji / 3.0 + 2 * f_duoji * f_duoji * f_duoji * f_duoji / 15.0); 
			if (f_tanA > 0) //右
			{
				GoalPulseL=GoalPulseLL+ 1.0 * GoalPulseLL * f_tanA;
				GoalPulseR=GoalPulseRR- 0.8 * GoalPulseRR * f_tanA;
			}
			else 
			{
				GoalPulseL=GoalPulseLL+ 0.8 * GoalPulseLL * f_tanA;
				GoalPulseR=GoalPulseRR- 1.0 * GoalPulseRR * f_tanA;
			}
			////pid_control 输入变量为编码器的值  目标变量为上面得到的值 通过pid 得到pwm输出
			if(flag_star)
			{
					MotorDutyL = Incremental_PI_L(PulseL,GoalPulseL);
					MotorDutyR = Incremental_PI_R(PulseR,GoalPulseR);
			}
		}
		else 
		{
			if(flag_star)
			{
					MotorDutyL = Incremental_PI_L(PulseL,GoalPulseLL);
					MotorDutyR = Incremental_PI_R(PulseR,GoalPulseRR);
			}
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
		
		if(ServoDuty>Servo_Left_Max) ServoDuty = Servo_Left_Max;
		else if(ServoDuty<Servo_Right_Min) ServoDuty = Servo_Right_Min;
		pwm_duty(PWMB_CH1_P74,ServoDuty);
	
	TIM4_CLEAR_FLAG; //清除中断标志

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
		case Round:
		{
			Round_handle();
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
void Turn_control(void)
{
		Date_analyse();//ADC采集处理
		if(Position == Normal)
		{
			shibie();//识别元素
		}
		Control();//执行对应元素的过程
		////限幅
		if(ServoDuty>Servo_Left_Max) ServoDuty = Servo_Left_Max;
		else if(ServoDuty<Servo_Right_Min) ServoDuty = Servo_Right_Min;
		pwm_duty(PWMB_CH1_P74,ServoDuty);
}
void Star_handle(void)//开始处理
{
	
	flag_star = 1;
	timeout = 0;
	GoalPulseLL = Speed_target ;
	GoalPulseRR = Speed_target ;
	Position = Fork_R;
	Fork_Process = Forking;
	fork_flag = 0;
	count = 0;
}
float err_get_f(float A, float B) //前排横竖电感加权
{
		uint16 temp_add1=0,temp_add2=0,temp_sub1=0,temp_sub2=0;
	
    temp_sub1=AD[2]-AD[4];
    temp_sub2=AD[0]-AD[6];
    temp_add1=AD[4]+AD[2];
    temp_add2=AD[0]+AD[6];

    return (float)(A*temp_sub1+B*temp_sub2)/(float)(A*temp_add1+B*temp_add2);
}
float err_get_b(float A, float B) //后排普通循迹 两个横 前排两个横
{
	  int16 temp_add1=0,temp_add2=0,temp_sub1=0,temp_sub2=0;
    temp_sub1=AD[9]-AD[8];
    temp_sub2=AD[2]-AD[4];
    temp_add1=AD[8]+AD[9];
    temp_add2=AD[2]+AD[4];
    return (float)(A*temp_sub1+B*temp_sub2)/(float)(A*temp_add1+B*temp_add2);
}
float err_get()
{
	return (float)(sqrt((float)AD[2])-sqrt((float)AD[4]))/(AD[2]+AD[4]);
}
float err_get_cross()
{
	return (float)(sqrt((float)AD[9])-sqrt((float)AD[8]))/(AD[8]+AD[9]);
}
float err_in_cross()
{
	return (float)((AD[1]-AD[5]))/(float)(AD[1]+AD[5]);
}
void Normal_handle(void)//正常巡线处理
{
	//static float last_input = 0;
	if(Compensate_flag)
	{
		input1 = ADC_Compensate + err_get_f(Normal_H,1-Normal_H); //ADC_Compensate 0~10 补偿量
		ServoDuty = Servo_Middle - pid_duoji_normal_sqrt(input1);
	}
	else
	{
		//input1 = 10 * err_get(); //差比和（差）求偏离程度
		input1 = err_get_f(Normal_H,1-Normal_H);
		ServoDuty = Servo_Middle - pid_duoji_normal_sqrt(input1);
//		last_input = input1;
	}
	 if(cross_cnt==2&&cnt>=5000&&cnt_star)//第二个十字出来一段路程再开三岔
	{
		fork_flag  = 1;
		cross_cnt  = 0;
		cnt_star = 0;
		cnt = 0;
	}
}

void Round_run(void)//圆环巡线处理
{
	//static float last_input = 0;
	if(Compensate_flag)
	{
		input1 = ADC_Compensate + 10* err_get_cross(); //ADC_Compensate 0~10 补偿量
		ServoDuty = Servo_Middle - pid_duoji_cross(input1);
	}
	else
	{
			input1 = 10 * err_get_cross(); //差比和（差）求偏离程度
			ServoDuty = Servo_Middle - pid_duoji_cross(input1);
//		last_input = input1;
	}
}

void Round_handle(void) //环岛处理
{
	static float dig= 0;
	static float ADC_Compensate_temp = 0.0;
	switch(Round_Process)
	{			
		case Round_Ining://入环前 判断左环或右环
		{
			if(cnt_star)
			{
				if(cnt>=circle_before_cnt)
				{
					if(AD[0]>=circle_in_max_right) 
					{
						Round_Process = Round_IningR;
						Compensate_flag = 1;
						cnt_left = 1;
						cnt = 0;
						if(ADC_Compensate<0) ADC_Compensate = 0-ADC_Compensate;
					}
//					if(AD[6]>=circle_in_max_left)
//					{
//						Round_Process = Round_IningL;
//						Compensate_flag = 1;
//						cnt_left = 0;
//						cnt = 0;
//						if(ADC_Compensate>0) ADC_Compensate = 0-ADC_Compensate;
//					}
				}
			}
			Normal_handle();
			break;
		}
		case Round_IningL:
		{
			if(cnt_star)
			{
				if(cnt>=circle_in_cnt)
				{
					Round_Process=Round_InoverL; //判断入环结束
					Compensate_flag = 0;
					ADC_Compensate_temp = ADC_Compensate;
					ADC_Compensate  = -0.08 ;
					cnt_star = 1;
					cnt_left = 0;
					cnt = 0;
				}
			}
			input1 = ADC_Compensate + 10* (err_get()>0?0:err_get()); //ADC_Compensate 0~10 补偿量
			ServoDuty = Servo_Middle - pid_duoji_normal_sqrt(input1);
			break;
		}
		case Round_InoverL:
		{
			
			input1 = 10*(float)(sqrt((float)AD[0])-sqrt((float)AD[6]))/(AD[0]+AD[6]);
			ServoDuty = Servo_Middle - circle_run_p*(input1);
			if(cnt>=circle_run_max_cnt)
			{
				if(AD[0]>=circle_out_max) //判断预出环 
				{
					Round_Process = Round_OutL ; 
					Compensate_flag = 1;
					ADC_Compensate = ADC_Compensate_temp;
				}
			}
			break;
		}
		case Round_OutL: // 出左环
		{
			input1 = ADC_Compensate + 10* (err_get()>0?0:err_get()); //ADC_Compensate 0~10 补偿量
			ServoDuty = Servo_Middle - pid_duoji_normal_sqrt(input1);
			if(AD[3]>=90)
			{
				Compensate_flag = 0;
				Round_Process = Round_Outover;
			}
			break;
		}
		//右环   
		case Round_IningR:
		{
				if(cnt_star)
				{
					if(cnt>=circle_in_cnt)
					{
						Round_Process=Round_InoverR; //判断入环结束
//						Compensate_flag = 0;
//						ADC_Compensate_temp = ADC_Compensate;
						//ADC_Compensate  = 0.08 ;
						cnt_star = 1;
						cnt = 0;
					}
				}
		  //input1 = 10*(float)(sqrt((float)AD[0])-0)/(AD[0]+AD[6]);
			input1 = (float)(AD[0]-0)/(AD[0]+AD[6]);
			ServoDuty = Servo_Middle - circle_run_p*(input1);
//			input1 = ADC_Compensate + 10* (err_get()>0?err_get():0); //ADC_Compensate 0~10 补偿量
//			ServoDuty = Servo_Middle - pid_duoji_normal_sqrt(input1);
			//if(AD[4]<=10 &&AD[5]<=10 && AD[6]<=10)  Round_Process=Round_InoverR; //判断入环结束
			break;
		}
		case Round_InoverR://
		{
			input1 = err_get_f(Circle_H,1-Circle_H);//0.3
			ServoDuty = Servo_Middle - pid_duoji_normal_sqrt(input1);
//			input1 = 10*(float)(sqrt((float)AD[0])-sqrt((float)AD[6]))/(AD[0]+AD[6]);
//			ServoDuty = Servo_Middle - circle_run_p*(input1);
//			Normal_handle();
			if(cnt>=circle_run_max_cnt)
			{
				if(AD[6]>=circle_out_max) //判断预出环
				{
					Round_Process = Round_OutR ; 
					Compensate_flag = 1;
					//ADC_Compensate = ADC_Compensate_temp;
					cnt_star = 1;
					cnt = 0;
				}
			}
			break;
		}
		case Round_OutR:
		{
			input1 = ADC_Compensate + 10*abs_float(err_get()); //ADC_Compensate 0~10 补偿量
			ServoDuty = Servo_Middle - pid_duoji_normal_sqrt(input1);
			if(cnt>=circle_out_max_cnt)
			{
					if(AD[3]>=90)
					{
						Round_Process = Round_Outover; 
						Compensate_flag = 0;
						//cnt = 0;
						cnt_star = 0;
						//cnt_star = 1;
						break;
					}
			}
			break;
		}
		case Round_Outover:
		{
//			if(cnt>=2000)
//			{
//				cnt = 0;
//				cnt_star = 0;
//				Position = Normal; //回到正常赛道
//				Normal_H = 1.0;
//			}
			input1 = 10 * err_get_cross(); //差比和（差）求偏离程度
			ServoDuty = Servo_Middle - 150*(input1);
//			Normal_handle();
			if(AD[6]<=60 && AD[0]<=60 )  
			{
				Position = Normal; //回到正常赛道
				Normal_H = 1.0;
			}
			
			
//			flag_star=0;
			break;
		}
	}
}

void Fork_Rhandle(void)//右三岔处理
{
	switch(Fork_Process)
	{
		case Fork_Ining:
		{
			input1 = 10*(float)(sqrt((float)AD[9])-0)/(AD[9]+AD[8]);
			ServoDuty = Servo_Middle - fork_turn*(input1);
			if(cnt>=fork_cnt_max)                //(AD[0]<=60 && AD[6]<=20 && AD[3]>=70) AD[8]+AD[9]<=150
			{
				cnt_star = 0;
				cnt = 0;
				Fork_Process = Forking;
				GoalPulseLL = Speed_target;
				GoalPulseRR = Speed_target;
			}
			break;
		}
		case Forking:
		{
			//右岔路循迹
			//Normal_handle();
			input1 = 10 * err_get(); //差比和（差）求偏离程度
			ServoDuty = Servo_Middle - pid_duoji_normal_sqrt(input1);
			if(AD[0]>=73) //判断出口
			{
				Fork_Process = Forkouting;
				count=0;
			}
			break;
		}
		case Forkouting:
		{
//			ServoDuty = Servo_Middle -FORK_OutServo;
			input1 = 10*(float)(sqrt((float)AD[9])-0)/(AD[9]+AD[8]);
			ServoDuty = Servo_Middle - fork_turn*(input1);
			if(AD[3]>=85) {
				GoalPulseLL = Speed_target;
				GoalPulseRR = Speed_target;
				Fork_Process = Forkoutover; //判断出岔路
				Position = Normal;
			}
			break;
		}
		case Forkoutover:
		{
			Position = Normal;
			Normal_handle();
			break;
		}
	}
}
void Fork_Lhandle(void)//左三岔处理
{
	switch(Fork_Process)
	{
		case Fork_Ining:
		{
			input1 = 10*(float)(0-sqrt((float)AD[8]))/(AD[9]+AD[8]);
			ServoDuty = Servo_Middle - fork_turn*(input1);
			if(cnt>=fork_cnt_max)                //(AD[0]<=60 && AD[6]<=20 && AD[3]>=70)   AD[8]+AD[9]<=150
			{
					cnt_star = 0;
					cnt = 0;
					Fork_Process = Forking;
					GoalPulseLL = Speed_target;
					GoalPulseRR = Speed_target;
					if(match==1)
					{
						cnt_star = 1;
						cnt = 0;
					}
			}
			break;
		}
		case Forking:
		{
//			input1 = 10 * (float)(sqrt((float)AD[7])-sqrt((float)AD[8]))/(AD[7]+AD[8]);
//			ServoDuty = Servo_Middle - pid_duoji_fork(input1);
			if(match==1)  
			{
				if(cnt>=6000) 
				{
					//flag_star = 0;
					GoalPulseLL = 0;
					GoalPulseRR = 0;
					cnt = 0;
					cnt_star = 0;
				}
			}				
			Normal_handle();
			if(AD[6]>=75)  //判断出口
			{  
				Fork_Process = Forkouting;
				count = 1;
			}
			break;
		}
		case Forkouting:
		{
//		ServoDuty = Servo_Middle +FORK_OutServo;
			input1 = 10*(float)(0-sqrt((float)AD[8]))/(AD[9]+AD[8]);
			ServoDuty = Servo_Middle - fork_turn*(input1);
			if(AD[3]>=85) {
				Fork_Process = Forkoutover; //判断出岔路
				GoalPulseLL = Speed_target;
			  GoalPulseRR = Speed_target;
				Position = Normal;
			}
			break;
		}
		case Forkoutover:
		{
			Normal_handle();
			Position = Normal;
			break;
		}
	}
}

void Cross_handle(void)//十字处理
{
	static int out_state;//丢线状态位
	switch(Cross_Process)
	{
		case Crossing:
		{
			//直走
			input1 = 10 * err_get_cross(); //差比和（差）求偏离程度
			//input1 = err_get_b(0.8,0.2);
			ServoDuty = Servo_Middle - 150*(input1);
			//Normal_handle();
			if(AD[3]<cross1_in_max&&cross_cnt==1)
			{
				Cross_Process = Cross_In;
			}
			else if(AD[3]<cross2_in_max&&cross_cnt==2)
			{
				Cross_Process = Cross_In;
			}
			break;
		}
		case Cross_In:
		{
			if(cnt>=cross_cnt_max)
			{
				  //走一段路程再进行出十字判断
					if((AD[0]+AD[3]+AD[6])>=Cross_sum-20)
					{
						Cross_Process = Crossover;
						cnt_star = 1;
						cnt = 0;
					}
					input1 = err_in_cross();
					ServoDuty = Servo_Middle - cross_run_p*(input1);
			}
			else//正常循迹
			{
				//input1 = err_get_f(Cross_H,1-Cross_H);
				input1 = err_in_cross();
				ServoDuty = Servo_Middle - cross_run_p*(input1);
//				if(AD[3]<=20)//丢线处理
//				{
//					ServoDuty = Servo_Middle - 2*cross_run_p*((float)(AD[9]-AD[8])/(float)(AD[8]+AD[9]));
//				}
			}
			//十字内丢线处理
			if(AD[9]>=30&&AD[3]<=15&&AD[2]<=15&&AD[4]<=15)//前瞻往左偏出赛道
			{
				out_state = 1;
				Cross_Process = Cross_duixian;
			}
			if(AD[8]>=30&&AD[3]<=15&&AD[2]<=15&&AD[4]<=15)//前瞻往右偏出赛道
			{
				out_state = 2;
				Cross_Process = Cross_duixian;
			}
			
			
			break;
		}
		case Cross_duixian:
		{

			if(out_state==1)
			{
				ServoDuty = Servo_Left_Max;
				if(AD[3]>=30&&AD[2]>=30&&AD[4]>=30)
				Cross_Process = Cross_In;
			}
			else if(out_state==2)
			{
				ServoDuty = Servo_Right_Min;
				if(AD[3]>=30&&AD[2]>=30&&AD[4]>=30)
				Cross_Process = Cross_In;
			}
//			if((AD[0]+AD[6]+AD[3])>=Cross_sum-20)
//			{
//				Cross_Process = Crossover;
//				cnt_star = 1;
//				cnt = 0;
//			}
				break;
			
	}
		case Crossover:
		{
			if(cnt>=7000)
			{
				cnt_star = 0;
				cnt = 0;
				Position = Normal;
				Normal_H    = 0.8;
				if(cross_cnt==2) 
				{
					cnt_star = 1;
//					GoalPulseLL = 0;
//					GoalPulseRR = 0;
					
				}
			}
			input1 = 10 * err_get_cross(); //差比和（差）求偏离程度
			ServoDuty = Servo_Middle - 200*(input1);
			
			break;
		}
	}
}
void Descent_handle(void)//坡道处理
{
	switch(Descent_Process)
	{
		case Descenting:
		{
			GoalPulseLL = Speed_target + Descent_Speedadd;
			GoalPulseRR = Speed_target + Descent_Speedadd;
			Normal_handle();
			if(AD[3]<=92&&AD[3]>85)
			{
				Descent_Process = Descentovering;
//				GoalPulseLL = Speed_target;
//				GoalPulseRR = Speed_target;
			}
			if(AD[3]<=80) 
			{
				Position = Normal;
				GoalPulseLL = Speed_target;
				GoalPulseRR = Speed_target;
			}
			break;
		}
		case Descentovering:
		{

			Normal_handle();
			if(AD[3]>96)
			{
				Descent_Process = Descentover;
				GoalPulseLL = Speed_target;
				GoalPulseRR = Speed_target;
			}
			if(AD[3]<=80) 
			{
				Position = Normal;
				GoalPulseLL = Speed_target;
				GoalPulseRR = Speed_target;
			}
			break;
		}
		case Descentover:
		{
			Normal_handle();
			if(AD[3]<=92)  Position = Normal;
			break;
		}
	}
}


uint8 key_read()
{
	if(!P32) return 1;
	else if(!P33) return 2;
	else if(!P45) return 3;
	else if(!P46) return 4;
	else if(!P53) return 5;
	else return 0;
}
char txt[40];
uint32 clkk = 0;
void view_0_handle(void)
{
	static uint16 ch =0;
	
	sprintf(txt,"Target:%d               ",Speed_target);
	lcd_showstr_simspi(0,0,(char*)txt);
	sprintf(txt,"Match:%1d               ",match);
	lcd_showstr_simspi(0,1,(char*)txt);
	sprintf(txt,"normal_kd:%d            ",duojikd_normal_sqrt);
	lcd_showstr_simspi(0,2,(char*)txt);
	sprintf(txt,"fork_flag:%d           ",fork_flag);
	lcd_showstr_simspi(0,3,(char*)txt);
	sprintf(txt,"ADC_get:%1d    %d       ",adc_get,beishu);
	lcd_showstr_simspi(0,4,(char*)txt);
	sprintf(txt,"chashu_flag:%1d              ",chashu_flag);
	lcd_showstr_simspi(0,5,(char*)txt);
	sprintf(txt,"chashu:%f              ",chashu);
	lcd_showstr_simspi(0,6,(char*)txt);
	sprintf(txt,"cin_max_left:%d            ",circle_in_max_left);
	lcd_showstr_simspi(0,7,(char*)txt);
	lcd_showchar_simspi(150,(ch)*16,'<');
	switch(key_read())
	{
		case 1 :
		{
			view++;
			//保存信息
			at24c02_write_byte(0,adc_get);
			at24c02_write_byte(1,match);
			at24c02_write_uint16(24,Speed_target);
			at24c02_write_uint16(54,circle_in_max_left);
			at24c02_write_uint16(28,duojikd_normal_sqrt);
			at24c02_write_byte(46,chashu_flag);
			at24c02_write_byte(60,chashu*10);
			at24c02_write_byte(51,fork_flag);
			lcd_clear_simspi(BLACK);
			break;
		}
		case 2 :
		{
			ch++;
			if(ch== 8) ch = 0;
			break;
		}
		case 3 :
		{
			//增加
			switch(ch)
			{
				case 0:
				{
					Speed_target+=beishu;
					break;
				}
				case 1:
				{
					match++;
					if(match>1) match=1;
					break;
				}
				case 2:
				{
					duojikd_normal_sqrt+=beishu;
					break;
				}
				case 3:
				{
					fork_flag++;
					if(fork_flag>1) fork_flag = 1;
					break;
				}
				case 4:
				{
					adc_get++;
					if(adc_get>1) adc_get=1;
					break; 
				}
				case 5:
				{
					chashu_flag++;
					if(chashu_flag>1) chashu_flag=1;
					break;
				}
				case 6:
				{
					chashu+=0.1;
					break;
				}
				case 7:
				{
					circle_in_max_left+=beishu;
					break;
				}
			}
			break;
		}
		case 4 :
		{
			//减少
			switch(ch)
			{
				case 0:
				{
					Speed_target-=beishu;
					break;
				}
				case 1:
				{
					match--;
					if(match<0) match =0;
					break;
				}
				case 2:
				{
					duojikd_normal_sqrt-=beishu;
					break;
				}
				case 3:
				{
					fork_flag--;
					if(fork_flag<0) fork_flag = 0;
					break;
				}
				case 4:
				{
					adc_get--;
					if(adc_get<0) adc_get = 0;
					break;
				}
				case 5:
				{
					chashu_flag--;
					if(chashu_flag<0) chashu_flag=0;
					break;
				}
				case 6:
				{
					chashu -= 0.1;
					break;
				}
				case 7:
				{
					circle_in_max_left-=beishu;
					break;
				}
			}
			break;
		}
		
		case 5 :
		{
			beishu *= 10;
			if(beishu==1000) beishu = 1;
			break;
		}
		
	}
}
void view_1_handle(void)
{

	static uint16 ch =0;
	int16 flag = 0;
	int16 i = 0;
		
	sprintf(txt,"duojikp_normal:%d        ",duojikp_normal_sqrt);
	lcd_showstr_simspi(0,0,(char*)txt);
	sprintf(txt,"fork_turn:%d         ",fork_turn);
	lcd_showstr_simspi(0,1,(char*)txt);
	sprintf(txt,"cross1_in_max:%d         ",cross1_in_max);
	lcd_showstr_simspi(0,2,(char*)txt);
	sprintf(txt,"cross2_in_max:%d         ",cross2_in_max);
	lcd_showstr_simspi(0,3,(char*)txt);
	sprintf(txt,"cross_run_p:%d           ",cross_run_p);
	lcd_showstr_simspi(0,4,(char*)txt);
	sprintf(txt,"cir_in_max_right:%d         ",circle_in_max_right);
	lcd_showstr_simspi(0,5,(char*)txt);
	sprintf(txt,"circle_run_p:%d  %d       ",circle_run_p,beishu);
	lcd_showstr_simspi(0,6,(char*)txt);
	sprintf(txt,"ADC_Compensate:%.2f        ",ADC_Compensate);
	lcd_showstr_simspi(0,7,(char*)txt);
	lcd_showchar_simspi(150,(ch)*16,'<');
	switch(key_read())
	{
		case 1 :
		{
			view++;
			for(i = 0;i<8;i++)
			{
				if(Speed_target==120+5*i)
				{
					 at24c02_write_uint16(79+2*i,duojikp_normal_sqrt);
					 at24c02_write_uint16(95+2*i,fork_turn);
					 at24c02_write_uint16(111+2*i,cross_run_p);
					 at24c02_write_uint16(127+2*i,cross1_in_max);
					 at24c02_write_uint16(143+2*i,cross2_in_max);
					 at24c02_write_uint16(159+2*i,circle_in_max_right);
					 at24c02_write_uint16(175+2*i,(int)(ADC_Compensate*100));
					 at24c02_write_uint16(191+2*i,circle_run_p);
					 flag = 1;
					 break;
				}
				
				//at24c02_write_uint16(175+2*i,(int)(0*100));
			}
			if(flag == 0)
			{
				at24c02_write_uint16(26,duojikp_normal_sqrt);
				at24c02_write_uint16(44,fork_turn);
				at24c02_write_uint16(34,cross_run_p);
				at24c02_write_uint16(32,cross1_in_max);
				at24c02_write_uint16(34,cross2_in_max);
				at24c02_write_uint16(56,circle_in_max_right);
				at24c02_write_uint16(61,ADC_Compensate*100);
				at24c02_write_uint16(52,circle_run_p);
			}
			//保存信息
			lcd_clear_simspi(BLACK);
			break;
		}
		case 2 :
		{
			ch++;
			if(ch== 8) ch = 0;
			break;
		}
		case 3 :
		{
			//增加
			switch(ch)
			{
				case 0:
				{
					duojikp_normal_sqrt += beishu;    //(beishu/100.0);
					break;
				}
				case 1:
				{
					fork_turn += (beishu);	
					break;
				}
				case 2:
				{
					cross1_in_max += beishu;
					break;
				}
				case 3:
				{
					cross2_in_max += beishu;
					break;
				}
				case 4:
				{
					cross_run_p += beishu;
					break;
				}
				case 5:
				{
					circle_in_max_right += beishu;
					break;
				}
				case 6:
				{
					circle_run_p += (beishu);
					break;
				}
				case 7:
				{
					ADC_Compensate += (beishu/100.0);
					break;
				}
			}
			break;
		}
		case 4 :
		{
			//减少
						switch(ch)
			{
				case 0:
				{
					duojikp_normal_sqrt -= beishu;    //(beishu/100.0);
					break;
				}
				case 1:
				{
					fork_turn -= (beishu);	
					break;
				}
				case 2:
				{
					cross1_in_max -= beishu;
					break;
				}
				case 3:
				{
					cross2_in_max -= beishu;
					break;
				}
				case 4:
				{
					cross_run_p -= beishu;
					break;
				}
				case 5:
				{
					circle_in_max_right -= beishu;
					break;
				}
				case 6:
				{
					circle_run_p -= (beishu);
					break;
				}
				case 7:
				{
					ADC_Compensate -= (beishu/100.0);
					break;
				}
			}
			break;
		}
		case 5 :
		{
			beishu *= 10;
			if(beishu==1000) beishu = 1;
			break;
		}
		
	}
}
void view_2_handle(void)
{
	static uint16 ch =0;
	sprintf(txt,"Circle_H:%.2f        ",Circle_H);
	lcd_showstr_simspi(0,0,(char*)txt);
	sprintf(txt,"Cross_H:%.2f         ",Cross_H);
	lcd_showstr_simspi(0,1,(char*)txt);
	sprintf(txt,"circle_out_max:%d           ",circle_out_max);
	lcd_showstr_simspi(0,2,(char*)txt);
	sprintf(txt,"fork_mid_max:%d            ",fork_mid_max);
	lcd_showstr_simspi(0,3,(char*)txt);
	sprintf(txt,"fork_mid_min:%d            ",fork_mid_min);
	lcd_showstr_simspi(0,4,(char*)txt);
	sprintf(txt,"fork_side_max:%d           ",fork_side_max);
	lcd_showstr_simspi(0,5,(char*)txt);
	sprintf(txt,"fork_side_min:%d           ",fork_side_min);
	lcd_showstr_simspi(0,6,(char*)txt);
	sprintf(txt,"cross2_out:%.2f        ",cross2_out);//阈值
	lcd_showstr_simspi(0,7,(char*)txt);
	lcd_showchar_simspi(150,(ch)*16,'<');
	
	switch(key_read())
	{
		case 1 :
		{
			view++;
			//保存信息	
			at24c02_write_uint16(36,Circle_H*100);
			at24c02_write_uint16(30,Cross_H*100);
			at24c02_write_uint16(58,circle_out_max);
			at24c02_write_uint16(65,fork_mid_max);
			at24c02_write_uint16(67,fork_mid_min);
			at24c02_write_uint16(69,fork_side_max);
			at24c02_write_uint16(71,fork_side_min);
			at24c02_write_uint16(42,cross2_out*100);
			lcd_clear_simspi(BLACK);
			break;
		}
		case 2 :
		{
			ch++;
			if(ch==8) ch = 0;
			break;
		}
		case 3 :
		{
			//增加
			switch(ch)
			{
				case 0:
				{
					Circle_H += (beishu/100.0);
					break;
				}
				case 1:
				{
					Cross_H += (beishu/100.0);
					break;
				}
				case 2:
				{
					circle_out_max += beishu;
					break;
				}
				case 3:
				{
					fork_mid_max += beishu;
					break;
				}
				case 4:
				{
					fork_mid_min += beishu;
					break;
				}
				case 5:
				{
					fork_side_max += beishu;
					break;
				}
				case 6:
				{
					fork_side_min += beishu;
					break;
				}
				case 7:
				{
					cross2_out += (beishu/100.0);
					break;
				}
			}
			break;
		}
		case 4 :
		{
			//减少
			switch(ch)
			{
				case 0:
				{
					Circle_H -= (beishu/100.0);
					break;
				}
				case 1:
				{
					Cross_H -= (beishu/100.0);
					break;
				}
				case 2:
				{
					circle_out_max -= beishu;
					break;
				}
				case 3:
				{
					fork_mid_max -= beishu;
					break;
				}
				case 4:
				{
					fork_mid_min -= beishu;
					break;
				}
				case 5:
				{
					fork_side_max -= beishu;
					break;
				}
				case 6:
				{
					fork_side_min -= beishu;
					break;
				}
				case 7:
				{
					cross2_out -= (beishu/100.0);
					break;
				}
			}
			break;
		}
		case 5 :
		{
			beishu *= 10;
			if(beishu==1000) beishu = 1;
			break;
		}
		
	}
}
void view_3_handle(void)
{
	static uint16 ch =0;
	sprintf(txt,"Cross_sum:%d               ",Cross_sum);
	lcd_showstr_simspi(0,0,(char*)txt);
	sprintf(txt,"cir_bef_cnt:%d               ",circle_before_cnt);
	lcd_showstr_simspi(0,1,(char*)txt);
	sprintf(txt,"cir_in_cnt:%d               ",circle_in_cnt);
	lcd_showstr_simspi(0,2,(char*)txt);
	sprintf(txt,"fork_cnt_max:%d           ",fork_cnt_max);
	lcd_showstr_simspi(0,3,(char*)txt);
	sprintf(txt,"cross_cnt_max:%d         ",cross_cnt_max);
	lcd_showstr_simspi(0,4,(char*)txt);
	sprintf(txt,"cir_run_cnt:%d           ",circle_run_max_cnt);
	lcd_showstr_simspi(0,5,(char*)txt);
	sprintf(txt,"cir_out_cnt:%d           ",circle_out_max_cnt);
	lcd_showstr_simspi(0,6,(char*)txt);
	sprintf(txt,"circle_max:%d %d          ",circle_max,beishu);
	lcd_showstr_simspi(0,7,(char*)txt);
	lcd_showchar_simspi(150,(ch)*16,'<');
	switch(key_read())
	{
		case 1 :
		{
			view++;
			//保存信息		
			at24c02_write_uint16(63,Cross_sum);
			at24c02_write_uint16(47,circle_before_cnt);
			at24c02_write_uint16(49,circle_in_cnt);
			at24c02_write_uint16(38,fork_cnt_max);
			at24c02_write_uint16(40,cross_cnt_max);
			at24c02_write_uint16(73,circle_run_max_cnt);
			at24c02_write_uint16(75,circle_out_max_cnt);
			at24c02_write_uint16(77,circle_max);
			lcd_clear_simspi(BLACK);
			break;
		}
		case 2 :
		{
			ch++;
			if(ch== 8) ch = 0;
			break;
		}
		case 3 :
		{
			//增加
			switch(ch)
			{
				case 0:
				{
					Cross_sum+=beishu;
					break;
				}
				case 1:
				{
					circle_before_cnt+=beishu;
					break;
				}
				case 2:
				{
					circle_in_cnt+=beishu;
					break;
				}
				case 3:
				{
					fork_cnt_max+=beishu;
					break;
				}
				case 4:
				{
					cross_cnt_max+=beishu;
					break;
				}
				case 5:
				{
					circle_run_max_cnt+=beishu;
					break;
				}
				case 6:
				{
					circle_out_max_cnt+=beishu;
					break;
				}
				case 7:
				{
					circle_max+=beishu;
				}
			}
			
			break;
		}
		case 4 :
		{
			//减少
			switch(ch)
			{
				case 0:
				{
					Cross_sum-=beishu;
					break;
				}
				case 1:
				{
					circle_before_cnt-=beishu;
					break;
				}
				case 2:
				{
					circle_in_cnt-=beishu;
					break;
				}
				case 3:
				{
					fork_cnt_max-=beishu;
					break;
				}
				case 4:
				{
					cross_cnt_max-=beishu;
					break;
				}
				case 5:
				{
					circle_run_max_cnt-=beishu;
					break;
				}
				case 6:
				{
					circle_out_max_cnt-=beishu;
					break;
				}
				case 7:
				{
					circle_max-=beishu;
				}
			}
			break;
		}
		case 5 :
		{
			beishu *= 10;
			if(beishu==10000) beishu = 1;
			break;
		}
		
	}
}
void TFTLCD_show(void)//显示数据
{
	
	switch(view)
	{
		case 0:
		{
			view_0_handle();
			break;
		}
		case 1:
		{
			view_1_handle();
			break;
		}
		case 2:
		{
			view_2_handle();
			break;
		}
		case 3:
		{
			view_3_handle();
			break;
		}
		case 4:
		{
			//开始
		  lcd_clear_simspi(BLACK);
			delay_ms(100);
			flag_star = 1;
			GoalPulseLL = Speed_target ;
			GoalPulseRR = Speed_target ;
			if(match==1) Star_handle();
			EnableGlobalIRQ();
			break;
		}
	}
}
void show_dirve()
{
	char txt[40];
	if(flag_star == 0)
	{
		
			sprintf(txt," %d %d %d %d     ",AD_V[7],AD_V[8],AD_V[9],AD_V[10]);	// 左   到 右
		
			lcd_showstr_simspi(0,0,(char*)txt);
			
			switch(Position)
			{
				case Normal:
				{
					sprintf(txt,"Position: Normal ");
					break; 
				}
				case Round:
				{
					sprintf(txt,"Position: Round ");		//赛道元素
					break;
				}
				case Fork_L:
				{
					sprintf(txt,"Position: Fork_L ");	
					break;
				}
				case Fork_R:
				{
					sprintf(txt,"Position: Fork_R ");	
					break;
				}
				case Cross:
				{
					sprintf(txt,"Position: Cross ");	
					break;
				}
				case Descent:
				{
					sprintf(txt,"Position: Descent ");	
					break;
				}
			}
			//sprintf(txt,"P:%.3f I:%.3f    ",Velocity_KP,Velocity_KI);	//显示kpki的值  asin(-2 * Q_info.q1 * Q_info.q3 + 2 * Q_info.q0 * Q_info.q2) * 180 / PI
			lcd_showstr_simspi(0,1,(char*)txt);
			sprintf(txt,"DIS:%d %d  ",Cross_Process,cnt);//显示编码器的值
			lcd_showstr_simspi(0,2,(char*)txt);
			sprintf(txt,"%02d %02d S:%.2f      ",AD[7],AD[10],input1);//显示电机pwm的值
			lcd_showstr_simspi(0,3,(char*)txt);
			sprintf(txt," %d %d %d %d %d        ",AD_V[6],AD_V[5],AD_V[4],AD_V[3],AD_V[2]);	// 显示舵机的角度
			lcd_showstr_simspi(0,4,(char*)txt);
			sprintf((char*)txt,"%d %d  %02d %02d       ",AD_V[1],AD_V[0],AD[8],AD[9]);	// 显示电感的值
			lcd_showstr_simspi(0,5,(char*)txt);
			sprintf(txt,"%02d %02d %02d %02d %02d %02d %02d      ",AD[6],AD[5],AD[4],AD[3],AD[2],AD[1],AD[0]);	// 显示电感归一化的值
			lcd_showstr_simspi(0,6,(char*)txt);
			sprintf((char*)txt,"%d             ",1233);	// 显示电感的值
			lcd_showstr_simspi(0,7,(char*)txt);
			if(key_read() == 1)
			{
				  
				lcd_clear_simspi(BLACK);
				delay_ms(100);
				flag_star = 1;
				GoalPulseLL = Speed_target ;
				GoalPulseRR = Speed_target ;
				//EnableGlobalIRQ();
			}
		}
}
void sss()
{
			char str[10];
			sprintf(str,"%d",duojikp_normal_sqrt);	// 显示电感归一化的值
			lcd_showstr_simspi(0,0,(char*)str);
}
void Send_Message(void)//发送信息
{
	switch(Position)
	{
			case Round:
			{
				bluetooth_ch9141_send_buff("Round\r\n",6);
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
void shibie()//识别元素
{
	if(AD[3]>=circle_max)//判断上坡和圆环
	{
//			if(AD[3]>=99)
//			{
//				Position=Descent;
//				Descent_Process = Descenting;
//				timeout = 0;
//			}
//			else 
//		  if(AD[0]+AD[6]>=120)
//			{
				Position=Round;
				Round_Process=Round_Ining;	
				cnt_star = 1;
				cnt = 0;
				timeout = 0;
//			}
	}
	else if(AD[3]<93 && (AD[3]+AD[6]+AD[0])>=Cross_sum)//十字
	{
		Position=Cross;
		Cross_Process = Crossing;
		timeout = 0;
		cross_cnt++;
		cnt_star = 1;
		cnt = 0;
		if(Speed_target>140)
		{
			GoalPulseLL = 140;
			GoalPulseRR = 140;  
		}

	}
	else if( fork_flag==1&&ad3_grow_flag==0&&AD[3]<=fork_mid_max &&AD[3]>=fork_mid_min && AD[0]>=fork_side_min && AD[0]<=fork_side_max && AD[6]>=fork_side_min &&AD[6]<=fork_side_max) // 三岔识别 (AD[3]<=fork_mid_max &&AD[3]>=fork_mid_min && AD[0]>=fork_side_min && AD[0]<=fork_side_max && AD[6]>=fork_side_min &&AD[6]<=fork_side_max
	//	&& AD[9]>=60 &&AD[9]<90&& AD[8]>=60&&AD[8]<90)||    (AD[7]>=fork_back_side_max&&AD[10]>=fork_back_side_max&&AD[3]<=fork_back_mid_max )
	{
		fork_flag = 0;
		if(count==0)  
		{
			Position=Fork_L;
			GoalPulseLL = Speed_target*0.7;
			GoalPulseRR = Speed_target*0.7;
		}
		else 
		{
			Position=Fork_R;
			GoalPulseLL = Speed_target*0.8;
			GoalPulseRR = Speed_target*0.6;
		}
		Fork_Process =Fork_Ining;
		timeout = 0;
		cnt_star = 1;
		cnt = 0;
	}
	else 
	{
		Position = Normal;
		Normal_Process =Normal_ing; 
	}
}

