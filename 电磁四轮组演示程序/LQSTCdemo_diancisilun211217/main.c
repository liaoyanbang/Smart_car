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
оƬ���һ��2038...Ȼ��IRCBND=0������Ƶ��Ϊ30M��
оƬ���һ��2010...����IRCBND=1������Ƶ��Ϊ30M��
�ٷ��������β�ͬ�����в��죬����Ϊ׼������
=================================================================
 ��ע�����ע�⣬һ��Ҫ�Զ����ǽ�������
 ʹ������ĸ����Զ�����̣�
 1.��ʹ�����ñ������ص�ѹ����ر�֤��ص�ѹ��7V���ϣ�������������Ӧ��
 2.Ȼ��ȷ����������ѹ��SD5/SD012�����5V���磬S3010��6-7V���磡����
 3.�Ѷ���Ķ���ȥ�����ö����������ת����
 4.��д�������У��ö��ת������ֵ���������û��Ӧ�ظ�1-2�������ߵ��������PWMƵ�ʼ�ռ�ձȣ����ܿ�Ϊ׼��
 5.����ܿغ�������ת�������֨֨�죬�Կ�ת������ʱ����װ�϶��̣�
 6.����K0/K1ȷ�����������ת�����ޣ�������������Ϊ�����޷���ֹ�����ת�ջ٣�
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void main(void)  
����  �ܡ�������
������ֵ����
������ֵ���� 
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void main(void)  
{	 	
  sysinit(); 	
 // UART1_config(1);						// ʹ�ö�ʱ��2������4�Ĳ���������ʱ��
  OLED_Init();                // OLED��ʼ��
 // OLED_CLS();   							// ����		
  OLED_P14x16Str(0,2,(unsigned char*)"�����������ܿƼ�");		// �ַ�����ʾ
  //OLED_P6x8Str(0,4,"Long Qiu i.s.t.");	// �ַ�����ʾ
  delayms(200);
  //Test_GPIO_LED();
  GPIO_KEY_Init();            // ������ʼ��
	MotorInit(12500);
	ServoInit(100);
	ADC_Init();		// ADC��ʼ��
	Timer34EncInit();                          	// �������ӿڳ�ʼ��
	Timer0_init();
	Timer1_init();
  OLED_CLS();     // ����		
  
	EnableInterrupts;					           // �����ж�
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
����������void sysinit(void)  
����  �ܡ�����ϵͳ���ܳ�ʼ��
������ֵ����
������ֵ���� 
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void sysinit(void)  
{	
  WTST = 0; 
  P_SW2 |= 0x80;	
	// ����Ƭ��������һ�е�ǰ5λ��2010A��30M��Ҫ IRCBAND=1; IRCBAND=0; Ƶ�ʸı�
	// ����Ƭ��������һ�е�ǰ5λ��2038A��30M��Ҫ IRCBAND=0; 
	IRCBAND=1;       	 	    // 1��35MƵ�Σ�0��24MƵ��
  //IRTRIM =0;            // Ƶ��У������ͨ��ISPУ��
  //LIRTRIM=0;            // Ƶ��У������ͨ��ISPУ��
  CLKDIV = 0;				      // ��Ƶ����
  P_SW2 &= 0x7F;
  RSTCFG|=1<<4;   		    // ʹ��P5.4��λ�ܽ�  
  P0M1 =0x40; P0M0 = 0;   // ����Ϊ׼˫���,P0.6  0.7Ϊ����ڣ���������
  P0PU =0x40;             // P0.6  0.7ʹ���ڲ�4.1K��������
  P1M1 = 0;   P1M0 = 0;   // ����Ϊ׼˫���
  P3M1 = 0;   P3M0 = 0;   // ����Ϊ׼˫���
  P4M1 = 0;   P4M0 = 0;   // ����Ϊ׼˫���
  P5M1 = 0;   P5M0 = 0;   // ����Ϊ׼˫���
  //P6M1 = 0;   P6M0 = 0; // ����Ϊ׼˫��ڣ�OLED��
  //P7M1 =0xFF; P7M0 = 0; // ����Ϊ׼˫��ڣ�����Ͱ���ʹ��
  P7PU =0xFF;	            // ����Ϊ׼˫���,P7.0123Ϊ����ڽӰ���,P7.4567Ϊ����ڽӵ������
  P40 = 0;				        // ��LED��Դ 
  EA=0;                   // �ر����ж�
}
