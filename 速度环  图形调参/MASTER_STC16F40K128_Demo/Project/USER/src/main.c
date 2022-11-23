/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC16F40K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/

#include "headfile.h"


/*
 *关于内核频率的设定，可以查看board.h文件
 *在board_init中,已经将P54引脚设置为复位
 *如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可
 */


//=======蓝牙灯光说明=========
//1，蓝灯闪烁表示表示当前为从机模式并且未连接
//2，蓝灯不亮表示当前为主机模式并且未连接
//3，蓝灯常亮表示已连接
//4，如果蓝牙连接成功了，进行透传的时候蓝灯会闪烁

//=========主机程序==========
//需要与从机程序配套测试
//需要在初始化函数参数中写入对应地址，如：bluetooth_ch9141_init(MASTER_MODE，"80:90:2C:E4:C2:84,000000");
//查看模块地址方式
//1.使用在线调试	（主机或从机方式都能查看）
//	初始化中bluetooth_ch9141_get_mac_address()函数下一语句加个断点，运行到断点后查看mac_address数组，得到的17位字符即为地址
//2. 使用手机	 （下载从机代码后使用）
//	上电运行后，打开手机BLE调试软件，能搜索到模块的名字，名字下方便是地址
//	模块的名字为初始化中bluetooth_ch9141_set_name函数的参数
//	如果使用手机查看蓝牙的mac地址，则输入的时候请将mac倒置一下
//	例如手机查看到的mac地址为61:62:63:64:65:67，则使用本函数连接的时候应该写
//bluetooth_ch9141_init(MASTER_MODE，"67:65:64:63:62:61,000000");
//地址填写完成后运行代码，如果从机运行中，主机会自动连接从机

//！！运行效果：
//主机持续发送数据，从机接收到数据后会将数据发送回来，主机接收到从机返回的数据后将数据显示到屏幕上
uint8 test1[] = "1111111111\r\n";
uint8 test2[] = "222222222222\r\n";
uint8 send_value = 'a';
uint8 str[30];
void OLED_show(void);
#define LED P52
int main(void)
{
    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口
	
		lcd_init_simspi();//初始化TFT屏
		
		pit_timer_ms(TIM_1,20);//初始化定时器1 5ms  Control 速度环
//		pit_timer_ms(TIM_2,100);//初始化定时器1 5ms  Control 速度环
//		
//		exit_init(INT0_P32, FALLING_EDGE);	  //下降沿触发INT0_P32中断  KEY1
//		exit_init(INT1_P33, FALLING_EDGE);	  //下降沿触发INT1_P33中断  KEY2
//		exit_init(INT2_P36, FALLING_EDGE);	  //下降沿触发INT2_P36中断  KEY3
//		exit_init(INT3_P37, FALLING_EDGE);	  //下降沿触发INT3_P37中断  KEY4
	
		pwm_init(PWMA_CH2P_P62,10000,0);//初始化左电机
		pwm_init(PWMA_CH1P_P60,10000,0);//初始化右电机
		
		ctimer_count_init(CTIM0_P34);//编码器1初始化
		ctimer_count_init(CTIM3_P04);//编码器2初始化
		
	
	
	
		//上位机配置
		uart_init(UART_1, UART1_RX_P30, UART1_TX_P31, 115200, TIM_2); 
		delay_ms(300);  //延时300ms，等待主板其他外设上电成功
    EnableGlobalIRQ();
		flag_star = 1;
    while(1)
    {
			ANO_DT_SendtoUSER(PulseL,GoalPulseL,PulseR,GoalPulseR);
			//delay_ms(20);
			TFTLCD_show();
			if(P32 == 0)
			{
				delay_ms(10);
				if(P32 == 0)
				{
					while(P32);
					Velocity_KI_L +=0.1;
				}
			}
			else if(P33 == 0)
			{
				delay_ms(10);
				if(P33 == 0)
				{
					while(P33);
					LED = !LED;	//反转LED
					//k=at24c02_read_byte(AT24C02_DEV_ADDR);
					Velocity_KI_L -=0.1;
				}
			}
			else if(P45 == 0)
			{
				delay_ms(10);
				if(P45 == 0)
				{
					while(P45);
					LED = !LED;	//反转LED
					Velocity_KP_L +=0.1;
				}
			}
			else if(P46 == 0)
			{
				delay_ms(10);
				if(P46 == 0)
				{
					while(P46);
					LED = !LED;	//反转LED
					Velocity_KP_L -=0.1;
				}
			}
			
			
    }
}

