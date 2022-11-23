/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�STC16���İ�
����    д��chiusir
��E-mail  ��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2021��1��23��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��IDE��STC16
��Target �� C251Keil5.6������
��SYS PLL�� 30MHzʹ���ڲ�����
=================================================================
STC16���������Ƶ��
����Ƽ�Bվ��ַ��https://space.bilibili.com/95313236
STC16�������زο���Ƶ�� https://www.bilibili.com/video/BV1gy4y1p7T1/
STC16һ����ӽ�����Ƶ�� https://www.bilibili.com/video/BV1Jy4y1e7R4/
=================================================================
����ʱ, ѡ��ʱ�� 30MHZ (�û��������޸�Ƶ��).
STC16F��������:����IRCBND=0��ISP��������Ϊ24M��
Ȼ��IRCBND=0������Ƶ��Ϊ30M��
����IRCBND=1������Ƶ��Ϊ30M������Ϊ׼
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������ADC1_SingleChannel_Get(uint8_t ADC_Channel_x) 
����  �ܡ���λʱ���ȡ����������
������ֵ��u8 encno ������ѡ��
������ֵ����������ע���ȡ֮�������ڣ���Ҫ����16λ���ֵ
�������¡�2021��1��22�� 
������汾��V1.0
��ʹ�þ�����x=ADC_Read(8); //��ȡADC8ͨ��ֵ
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
short Read_Encoder(u8 encno)
{
  short data tm=0;
  if(encno==1)
  {
    if(P07)//������1����A:P06  DIR:P07 	
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
    if(P05)//������������A:P04  DIR:P05 	
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
����������void Timer4EncInit(void)
����  �ܡ���������ʼ������
������ֵ����
������ֵ����
�������¡�2021��1��22�� 
������汾��V1.0
��ʹ�þ�����2��������
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Timer34EncInit (void)
{   
  /*
  T4T3M &= ~0xf0;     	//ֹͣ����, ��ʱģʽ, 12Tģʽ, �����ʱ��  
  T4T3M &= ~0x01;    		//�����ʱ��
  T4T3M |=  (1<<2);    	//12T mode ;P0.4����
  T3H = 0;
  T3L = 0;
  
  T4T3M |=  (1<<3);     //��ʼ����
  //ET3 = 1;   					//�����ж�
  
  T4T3M &= ~0x10;    	  //�����ʱ��
  T4T3M |=  (1<<6);     //12T mode ;P0.6����
  T4H = 0;
  T4L = 0; 
  T4T3M |=  (1<<7);     //��ʼ����
  //ET4 = 1;   					//�����ж�	  
  */
  T3L = 0; 
  T3H = 0;
  T4T3M |= 0x0c; 
  
  T4L = 0;
  T4H = 0;
  T4T3M |= 0xc0; 
}


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void TestEncoder(void)
����  �ܡ����Գ���TFT1.8��ʾ
������ֵ����
������ֵ����
�������¡�2021��1��22�� 
������汾��V1.0
��ʹ�þ�����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TestEncoder(void)
{
  char txt[16];
  short duty = 20;
  short enc1,enc2;
  
//  OLED_CLS();   	                         	// ����
//  OLED_P6x8Str(0, 0, "Test Encoder");       	// �ַ�����ʾ
//  Timer34EncInit();                          	// �������ӿڳ�ʼ��
  MotorInit(MOTOR_FREQUENCY);    				// �����ʼ��
  while (1)
  {
    if (KEY_Read(KEY0) == 0)      // ����KEY0����ռ�ձȼ�С
    {
      if (duty > -PWM2_CLOCK/MOTOR_FREQUENCY)	// ��ռ�ձ�Ϊ10M/12.5k=800
        duty -= 50;
    }
    if (KEY_Read(KEY2) == 0)      // ����KEY2����ռ�ձȼӴ�
    {
      if (duty < PWM2_CLOCK/MOTOR_FREQUENCY)  // ��ռ�ձ�Ϊ10M/12.5k=800
        duty += 50;
    }
    if (KEY_Read(KEY1) == 0)      // ����KEY1����ռ�ձ���ֵ
    {
      duty = 20;
    }
    
    MotorCtrl(duty,duty);
    
    /* ��ȡ������ֵ */
    enc1 = Read_Encoder(1); 			        // ���� ĸ���ϱ�����1��С��ǰ��Ϊ��ֵ
    enc2 = Read_Encoder(2); 			        // �ҵ�� ĸ���ϱ�����2��С������Ϊ��ֵ
    sprintf(txt, "Enc1: %04d  ", enc1);
    OLED_P6x8Str(0, 3, txt);			        // �ַ�����ʾ
    sprintf(txt, "PWM: %05d;", duty);
    OLED_P6x8Str(0, 4, txt);   					// �ַ�����ʾ
    sprintf(txt, "Enc2: %04d  ", enc2);
    OLED_P6x8Str(0, 5, txt);			        // �ַ�����ʾ
    
    LED_Ctrl(LED0, RVS);       		        	// ��ƽ��ת,LED��˸
    delayms(50);							 	// ��ʱ�ȴ�
  }
}
//