#include "headfile.h"

#define LED P52
#define CHAO P47


uint8 str[40];
int main(void)
{
    DisableGlobalIRQ();
	
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口
	
		lcd_init_simspi();//初始化TFT屏
		
		pit_timer_ms(TIM_4,5);//初始化定时器1 50ms 
		pit_timer_ms(TIM_2,6);//初始化定时器1 50ms 

		//icm20602_init_simspi();//ICM20602初始化 
	
		pwm_init(PWMB_CH1_P74,50,Servo_Middle);//初始化舵机  
	
		pwm_init(PWMA_CH2P_P62,10000,0);//初始化左电机
		pwm_init(PWMA_CH1P_P60,10000,0);//初始化右电机
		
		adc_init(ADC_P00,ADC_SYSclk_DIV_2); //ADC初始化
		adc_init(ADC_P01,ADC_SYSclk_DIV_2);
		adc_init(ADC_P05,ADC_SYSclk_DIV_2);
		adc_init(ADC_P06,ADC_SYSclk_DIV_2);
		adc_init(ADC_P13,ADC_SYSclk_DIV_2);
		adc_init(ADC_P14,ADC_SYSclk_DIV_2);
		adc_init(ADC_P15,ADC_SYSclk_DIV_2);
		adc_init(ADC_P10,ADC_SYSclk_DIV_2);
		adc_init(ADC_P11,ADC_SYSclk_DIV_2);
		adc_init(ADC_P02,ADC_SYSclk_DIV_2);
		adc_init(ADC_P03,ADC_SYSclk_DIV_2);
		
		ctimer_count_init(CTIM0_P34);//编码器1初始化
		ctimer_count_init(CTIM3_P04);//编码器2初始化
		
		Position = Normal;
		
		wireless_type = WIRELESS_CH9141;
    //如果不接RTS引脚，则RTS引脚默认为高电平，这里需要将其设置为低电平。
		
    BLUETOOTH_CH9141_RTS_PIN = 0;
		
    //初始化串口
    //uart_init (BLUETOOTH_CH9141_UART, BLUETOOTH_CH9141_UART_RX, BLUETOOTH_CH9141_UART_TX, BLUETOOTH_CH9141_UART_BAUD, BLUETOOTH_CH9141_TIMER_N);	//初始化串口   
		
		gpio_mode(P32,GPIO);
		gpio_mode(P33,GPIO);
		
		//uart_init(UART_3, UART3_RX_P50, UART3_TX_P51, 115200, TIM_2);  //超声波配置
		
		ADV_Init();
		
		delay_ms(300);  //延时300ms，等待主板其他外设上电成功
		while(1)
		{
			TFTLCD_show();
//			if(res == 'G')
//			{
//				Star_handle();
//			}
			if(flag_star ==1 )break;

		}
		//if(init==0)  gyroOffset_init();
    EnableGlobalIRQ();
    while(1)
    {
			
//			 Turn_control(); 
			 show_dirve();
//			 CHAO= 1;
//			 if(dis_flag)
//			 {
//					
//					dis_flag=0;
//			 }
//			 lcd_showuint16_simspi(0,0,time_5ms);
//			 delay_ms(100);
		}
}
