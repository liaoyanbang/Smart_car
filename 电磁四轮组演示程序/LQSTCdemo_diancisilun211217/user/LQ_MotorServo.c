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

#define USEDRV8701     //ʹ�ò�ͬ�����������岻ͬ�ĺ궨��
//#define USE7843OR7971//ʹ�ò�ͬ�����������岻ͬ�ĺ궨��

#define PWM1_0      0x00	//P:P1.0  N:P1.1
#define PWM1_1      0x01	//P:P2.0  N:P2.1 
#define PWM1_2      0x02	//P:P6.0  N:P6.1

#define PWM2_0      0x00	//P:P1.2/P5.4  N:P1.3
#define PWM2_1      0x04	//P:P2.2  N:P2.3
#define PWM2_2      0x08	//P:P6.2  N:P6.3

#define PWM3_0      0x00	//P:P1.4  N:P1.5
#define PWM3_1      0x10	//P:P2.4  N:P2.5
#define PWM3_2      0x20	//P:P6.4  N:P6.5

#define PWM4_0      0x00	//P:P1.6  N:P1.7
#define PWM4_1      0x40	//P:P2.6  N:P2.7
#define PWM4_2      0x80	//P:P6.6  N:P6.7
#define PWM4_3      0xC0	//P:P3.4  N:P3.3

#define PWM5_0      0x00	//P2.0
#define PWM5_1      0x01	//P1.7
#define PWM5_2      0x02	//P0.0
#define PWM5_3      0x03	//P7.4

#define PWM6_0      0x00	//P2.1
#define PWM6_1      0x04	//P5.4
#define PWM6_2      0x08	//P0.1
#define PWM6_3      0x0C	//P7.5

#define PWM7_0      0x00	//P2.2
#define PWM7_1      0x10	//P3.3
#define PWM7_2      0x20	//P0.2
#define PWM7_3      0x30	//P7.6

#define PWM8_0      0x00	//P2.3
#define PWM8_1      0x40	//P3.4
#define PWM8_2      0x80	//P0.3
#define PWM8_3      0xC0	//P7.7

#define ENO5P       0x01
#define ENO6P       0x04
#define ENO7P       0x10
#define ENO8P       0x40
#define ENO1P       0x01
#define ENO1N       0x02
#define ENO2P       0x04
#define ENO2N       0x08
#define ENO3P       0x10
#define ENO3N       0x20
#define ENO4P       0x40
#define ENO4N       0x80


u16 PWM1_Duty;
u16 PWM2_Duty;
u16 PWM3_Duty;
u16 PWM4_Duty;
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void MotorInit(u16 freq)
����  �ܡ����PWM��ʼ��
������ֵ��int Encoder ���������ɼ��ĳ���
������ֵ��int Target  ����������
������ֵ�����PWM 
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
�����þ������������  TIM4_PWM_Init(1000-1);  //1KHZ����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifdef USE7843OR7971
void MotorInit(u16 freq)
{
  P_SW2 |= 0x80;
  PWM2_PSCRL = (MAIN_Fosc/PWM2_CLOCK)-1;   	//��ƵΪ22M,PWM��Ƶ��10M��Χ 
  PWM2_CCER1 = 0x00;												//д CCMRx ǰ���������� CCxE �ر�ͨ��
  PWM2_CCER2 = 0x00;
  PWM2_CCMR1 = 0x60; 												//ͨ��ģʽ����
  PWM2_CCMR2 = 0x60;
  PWM2_CCMR3 = 0x60;
  PWM2_CCMR4 = 0x60;
  PWM2_CCER1 = 0x33; 												//����ͨ�����ʹ�ܺͼ���
  PWM2_CCER2 = 0x33;
  
  PWM2_ARRH  = (u8)((PWM2_CLOCK/freq)>>8);  //��������ʱ�䣬��0�������趨��ֵ
  PWM2_ARRL  = (u8)(PWM2_CLOCK/freq); //  
  
  PWM2_CCR1H = 0;
  PWM2_CCR1L = 20; 	//����ռ�ձ�ʱ��
  PWM2_CCR2H = 0;
  PWM2_CCR2L = 20; 	//����ռ�ձ�ʱ��	
  PWM2_CCR3H = 0;
  PWM2_CCR3L = 20; 	//����ռ�ձ�ʱ��
  PWM2_CCR4H = 0;	
  PWM2_CCR4L = 20; 	//����ռ�ձ�ʱ��
  
  PWM2_ENO = 0x00;
  PWM2_ENO |= ENO5P; //ʹ�����
  PWM2_ENO |= ENO6P; //ʹ����� 
  PWM2_ENO |= ENO7P; //ʹ�����
  PWM2_ENO |= ENO8P; //ʹ����� 
  PWM2_PS = 0x00;    //�߼� PWM ͨ�������ѡ��λ
  PWM2_PS |= PWM5_3; //ѡ�� PWM5_3 ͨ��
  PWM2_PS |= PWM6_3; //ѡ�� PWM6_3 ͨ�� //�߼� PWM ͨ�� 4P �����ѡ��λ, 0x00:P1.6, 0x40:P2.6, 0x80:P6.6, 0xC0:P3.4
  PWM2_PS |= PWM7_3;
  PWM2_PS |= PWM8_3;
  PWM2_BKR = 0x80;   //ʹ�������
  PWM2_CR1 |= 0x01;  //��ʼ��ʱ
  P_SW2 &= 0x7f;
}
#elif defined USEDRV8701
void MotorInit(u16 freq)
{
  P_SW2 |= 0x80;
  PWM2_PSCRL = (MAIN_Fosc/PWM2_CLOCK)-1;   //��ƵΪ22M,PWM��Ƶ��10M��Χ 
  PWM2_CCER1 = 0x00; //д CCMRx ǰ���������� CCxE �ر�ͨ��
  PWM2_CCER2 = 0x00;
  PWM2_CCMR1 = 0x60; //ͨ��ģʽ����
  PWM2_CCMR2 = 0x60;
  PWM2_CCMR3 = 0x60;
  PWM2_CCMR4 = 0x60;
  PWM2_CCER1 = 0x55; //����ͨ�����ʹ�ܺͼ���
  PWM2_CCER2 = 0x55;
  
  PWM2_ARRH  = (u8)((PWM2_CLOCK/freq)>>8);  //��������ʱ�䣬��0�������趨��ֵ
  PWM2_ARRL  = (u8)(PWM2_CLOCK/freq); //  
  
  PWM2_CCR1H = 100;
  PWM2_CCR1L = 200; 	//����ռ�ձ�ʱ��
  PWM2_CCR3H = 100;
  PWM2_CCR3L = 200; 	//����ռ�ձ�ʱ��	
  
  PWM2_ENO = 0x00;
  PWM2_ENO |= ENO5P; 	//ʹ�����
  //PWM2_ENO |= ENO6P;//ʹ����� 
  PWM2_ENO |= ENO7P; 	//ʹ�����
  //PWM2_ENO |= ENO8P;//ʹ����� 
  PWM2_PS = 0x00;   	//�߼� PWM ͨ�������ѡ��λ
  PWM2_PS |= PWM5_3;	//ѡ�� PWM5_3 ͨ��
  //PWM2_PS |= PWM6_3;//ѡ�� PWM6_3 ͨ�� //�߼� PWM ͨ�� 4P �����ѡ��λ, 0x00:P1.6, 0x40:P2.6, 0x80:P6.6, 0xC0:P3.4
  PWM2_PS |= PWM7_3;
  //PWM2_PS |= PWM8_3;
  PWM2_BKR = 0x80;			//ʹ�������
  PWM2_CR1 |= 0x01;  		//��ʼ��ʱ
  
  P7M1 = 0;   
  P7M0 = 0xA0;				//P75,P77����Ϊ���������ǿ������Ҫ����������
  P7PU |= 0xA0;
  
  P_SW2 &= 0x7f;
}
#endif
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void MotorCtrl(float motor1, float motor2)
����  �ܡ�������ƺ���
������ֵ��motor1   �� ���1ռ�ձ�
������ֵ��motor2   �� ���2ռ�ձ�
������ֵ�����PWM 
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
�����þ����� MotorCtrl (1000,-2000) //�������һ��һ��ת��
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifdef  USE7843OR7971
void MotorCtrl (int16_t motor1,int16_t motor2)
{
  if (motor1 < 0)
  {	 
		motor1=0-motor1;
    P_SW2 |= 0x80;
    PWM2_CCR1H = 0; //����ռ�ձ�ʱ��
    PWM2_CCR1L = 0;
    PWM2_CCR2H = (u8)(motor1 >> 8); //����ռ�ձ�ʱ��
    PWM2_CCR2L = (u8)(motor1);
    P_SW2 &= 0x7f;
  }
  else
  {
    P_SW2 |= 0x80;
    PWM2_CCR1H = (u8)(motor1 >> 8); //����ռ�ձ�ʱ��
    PWM2_CCR1L = (u8)(motor1);
    PWM2_CCR2H = 0; //����ռ�ձ�ʱ��
    PWM2_CCR2L = 0;
    P_SW2 &= 0x7f;
  }  
  if (motor2 < 0)
  {	 
		motor2=0-motor2;
    P_SW2 |= 0x80;
    PWM2_CCR3H = 0; //����ռ�ձ�ʱ��
    PWM2_CCR3L = 0;
    PWM2_CCR4H = (u8)(motor2 >> 8); //����ռ�ձ�ʱ��
    PWM2_CCR4L = (u8)(motor2);
    P_SW2 &= 0x7f;
  }
  else
  {
    P_SW2 |= 0x80;
    PWM2_CCR3H = (u8)(motor2 >> 8); //����ռ�ձ�ʱ��
    PWM2_CCR3L = (u8)(motor2);
    PWM2_CCR4H = 0; //����ռ�ձ�ʱ��
    PWM2_CCR4L = 0;
    P_SW2 &= 0x7f;
  }  
}
#elif defined USEDRV8701
void MotorCtrl (int16_t motor1,int16_t motor2)
{
  if (motor1 < 0)
  {	 
		motor1=0-motor1;
    P_SW2 |= 0x80;
    PWM2_CCR1H = (u8)(motor1 >> 8); //����ռ�ձ�ʱ��
    PWM2_CCR1L = (u8)(motor1);
    P75=1;
    
    P_SW2 &= 0x7f;
  }
  else
  {
    P_SW2 |= 0x80;
    PWM2_CCR1H = (u8)(motor1 >> 8); //����ռ�ձ�ʱ��
    PWM2_CCR1L = (u8)(motor1);
    P75=0;
    P_SW2 &= 0x7f;
  }
  
  if (motor2 < 0)
  {	 
		motor2=0-motor2;
    P_SW2 |= 0x80;
    PWM2_CCR3H = (u8)(motor2 >> 8); //����ռ�ձ�ʱ��
    PWM2_CCR3L = (u8)(motor2);
    P77=1;
    P_SW2 &= 0x7f;
  }
  else
  {
    P_SW2 |= 0x80;
    PWM2_CCR3H = (u8)(motor2 >> 8); //����ռ�ձ�ʱ��
    PWM2_CCR3L = (u8)(motor2);
    P77=0;
    P_SW2 &= 0x7f;
  }  
}
#endif
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������TestMotor(void)
����  �ܡ����Ա궨���PWM���Ƶ��
������ֵ����
������ֵ����
������ֵ����
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
�����þ�����
��ע�����ע�⣬һ��Ҫ�Ե�������������
ʹ������ĸ����Ե�����������̣�
1.��ʹ�����ñ������ص�ѹ����ر�֤��ص�ѹ��7V���ϣ�������������Ӧ��
2.�Ӻ�ĸ�嵽��������ź��߼���Դ�ߣ�
3.�Ӻ������嵽����ĵ��ߣ�
4.��д�������У�ȷ�����������ת���󣬿����������Դ���أ�
5.����K0/K1ȷ�����ת���ٶȼ�����
6.������ַ�ת������K1�����ص���ģʽ������ֱ�ӹر��������Դ��
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TestMotor (void)
{
  short duty = 20;
  char txt[16];
  OLED_Init(); 
  OLED_CLS();   						// ����
  OLED_P6x8Str(2, 0, "LQ Motor Test");
  MotorInit(MOTOR_FREQUENCY);    		// �����ʼ��
  while (1)
  {
    if (KEY_Read(KEY0) == 0)      	// ����KEY0����ռ�ձȼ�С
    {
      if (duty > -(PWM2_CLOCK/MOTOR_FREQUENCY))	// ��ռ�ձ�Ϊ10M/12.5k=800
        duty -= 30;
    }
    if (KEY_Read(KEY2) == 0)      	// ����KEY2����ռ�ձȼӴ�
    {
      if (duty < PWM2_CLOCK/MOTOR_FREQUENCY)  // ��ռ�ձ�Ϊ10M/12.5k=800
        duty += 40;
    }
    if (KEY_Read(KEY1) == 0)      	// ����KEY1����ռ�ձ���ֵ
    {
      duty = 20;
    }
    
    MotorCtrl(duty,duty);
    
    sprintf(txt, "PWM: %05d;", duty);
    OLED_P6x8Str(0, 5, txt);  		// �ַ�����ʾ
    UART1_PutStr(txt);
    
    LED_Ctrl(LED0, RVS);       		// ��ƽ��ת,LED��˸
    delayms(200);              		// ��ʱ�ȴ�
  }
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void ServoInit(u16 freq)
����  �ܡ����PWM��ʼ��
������ֵ��u16 freq,����Ƶ�ʣ���Χ0--10000
������ֵ����
������ֵ����
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
�����þ���������1����� ServoInit(100);  //100HZ����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void ServoInit(u16 freq)
{
  P_SW2 |= 0x80;	
  //PWM1_PSCRL = 16; //��Ƶ��Χ  fcn_cnt=fck_psc/(PSCR[15:0]+1)
  PWM1_PSCRL = MAIN_Fosc/PWM1_CLOCK-1;      //��ƵΪ30M,PWM��Ƶ��1M��Χ  
  PWM1_CCER2 = 0x00; //д CCMRx ǰ���������� CCxE �ر�ͨ��
  PWM1_CCMR4 = 0x60; //���� PWM4 ģʽ1 ���
  PWM1_CCER2 = 0x55; //ʹ�� CC4E ͨ��
  
  PWM1_ARRH  = (u8)((PWM1_CLOCK/freq)>>8);  //��������ʱ�䣬��0�������趨��ֵ
  PWM1_ARRL  = (u8)(PWM1_CLOCK/freq);       //
  
  PWM1_CCR4H = (u8)((Servo_Center)>>8);     //�����ֵ
  PWM1_CCR4L = (u8)(Servo_Center);          //
  
  PWM1_ENO |= 0x80; //ʹ�� PWM4N ���
  PWM1_PS  |= 0x00; //�߼� PWM ͨ�� 4N �����ѡ��λ, 0x00:P1.7, 0x40:P2.7, 0x80:P6.7, 0xC0:P3.3
  
  PWM1_BKR = 0x80;  //ʹ�������
  PWM1_CR1 |= 0x01; //��ʼ��ʱ
  P_SW2 &= 0x7f;		
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void ServoCtrl (uint16_t duty)
����  �ܡ����ת�Ǻ���������С�����˷�Χ���ƣ���С
������ֵ��short duty��ռ�ձȣ���Χ0--10000��
������ֵ��int Target  ����������
������ֵ�����PWM 
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
�����þ�����ServoCtrl (1500);//�����ֵ
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void ServoCtrl(uint16_t duty)
{  
	/*
  if (duty >= Servo_Left_Max)                  //���Ʒ�ֵ
  duty = Servo_Left_Max;
    else if (duty < Servo_Right_Min)          //���Ʒ�ֵ
  duty = Servo_Right_Min;
  */
  P_SW2 |= 0x80;
  PWM1_CCR4H = (u8)(duty >> 8); //����ռ�ձ�ʱ��
  PWM1_CCR4L = (u8)(duty);
  P_SW2 &= 0x7f;
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������Test_Servo(void)
����  �ܡ����PWM��ʼ�������Ա궨���PWM����SD5/S3010���
������ֵ����
������ֵ����
������ֵ����
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
�����þ�����
��ע�����ע�⣬һ��Ҫ�Զ����ǽ�������
ʹ������ĸ����Զ�����̣�
1.��ʹ�����ñ������ص�ѹ����ر�֤��ص�ѹ��7V���ϣ�������������Ӧ��
2.Ȼ��ȷ����������ѹ��SD5�����5V���磬S3010��6-7V���磬SD012�����5V���磡����
3.�Ѷ���Ķ���ȥ�����ö����������ת����
4.��д�������У��ö��ת������ֵ���������û��Ӧ�ظ�1-2�������ߵ��������PWMƵ�ʼ�ռ�ձȣ����ܿ�Ϊ׼��
5.����ܿغ�������ת�������֨֨�죬�Կ�ת������ʱ����װ�϶��̣�
6.����K0/K1ȷ�����������ת�����ޣ�������������Ϊ�����޷���ֹ�����ת�ջ٣�
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void TestServo (void)
{
  char txt[16] = "X:";
  u16 duty = Servo_Center;
  
  OLED_CLS();   				// ����
  OLED_P6x8Str(2, 0, "LQ Servo Test");
  ServoInit(100);				// �����ʼ����Ƶ��Ϊ100Hz
  ServoCtrl(Servo_Center); 	// �����ֵ
  while (1)
  {
    if (!KEY_Read(KEY0))
    {
      if (duty > 100)  	// ��ֹduty��
      {
        duty -= 10;     // �궨��ʱ�򣬿��԰Ѳ�����С�㣬����10
      }
    }
    if (!KEY_Read(KEY1))
    {
      duty = Servo_Center;
    }
    if (!KEY_Read(KEY2))
    {
      duty += 10;
    }
    ServoCtrl(duty); 
    sprintf(txt, "Servo:%05d ", duty);
    OLED_P6x8Str(1, 2, txt); 	// ��ʾ
    LED_Ctrl(LED0, RVS);   		// LED��˸;
    delayms(100);        
  }
}


//
