#include "headfile.h"

#define LED P52
#define CHAO P47


uint8 str[40];
int main(void)
{
    DisableGlobalIRQ();
	
    board_init();   //��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���
	
		lcd_init_simspi();//��ʼ��TFT��
		
		pit_timer_ms(TIM_4,5);//��ʼ����ʱ��1 50ms 
		pit_timer_ms(TIM_2,6);//��ʼ����ʱ��1 50ms 

		//icm20602_init_simspi();//ICM20602��ʼ�� 
	
		pwm_init(PWMB_CH1_P74,50,Servo_Middle);//��ʼ�����  
	
		pwm_init(PWMA_CH2P_P62,10000,0);//��ʼ������
		pwm_init(PWMA_CH1P_P60,10000,0);//��ʼ���ҵ��
		
		adc_init(ADC_P00,ADC_SYSclk_DIV_2); //ADC��ʼ��
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
		
		ctimer_count_init(CTIM0_P34);//������1��ʼ��
		ctimer_count_init(CTIM3_P04);//������2��ʼ��
		
		Position = Normal;
		
		wireless_type = WIRELESS_CH9141;
    //�������RTS���ţ���RTS����Ĭ��Ϊ�ߵ�ƽ��������Ҫ��������Ϊ�͵�ƽ��
		
    BLUETOOTH_CH9141_RTS_PIN = 0;
		
    //��ʼ������
    //uart_init (BLUETOOTH_CH9141_UART, BLUETOOTH_CH9141_UART_RX, BLUETOOTH_CH9141_UART_TX, BLUETOOTH_CH9141_UART_BAUD, BLUETOOTH_CH9141_TIMER_N);	//��ʼ������   
		
		gpio_mode(P32,GPIO);
		gpio_mode(P33,GPIO);
		
		//uart_init(UART_3, UART3_RX_P50, UART3_TX_P51, 115200, TIM_2);  //����������
		
		ADV_Init();
		
		delay_ms(300);  //��ʱ300ms���ȴ��������������ϵ�ɹ�
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
