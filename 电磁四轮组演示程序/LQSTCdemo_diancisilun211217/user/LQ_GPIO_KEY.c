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
����������void GPIO_KEY_Init(void)
����  �ܡ�GPIO��ʼ������
������ֵ����
������ֵ���� 
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void GPIO_KEY_Init(void)
{
  P7M1 &= 0xF0;
  P7M1 |= 0x00;
  P7M0 &= 0xF0;
  P7M0 |= 0x00;   //����Ϊ�������  
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������unsigned char KEY_Read(KEYn_e KEYno)
����  �ܡ���ȡ����״̬
������ֵ��KEYn_e KEYno�������
������ֵ������״̬��0/1
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char KEY_Read(KEYn_e KEYno)
{
  switch(KEYno)
  {
  case KEY0:
    if(P70)
      return 1;				//ĸ���ϰ���0
    else
    {
      while(P70==0);  //�ȴ��ɿ�����
      return 0;	
    }
    break;
    
  case KEY1:
    if(P71)
      return 1;				//ĸ���ϰ���0
    else
    {
      while(P71==0);  //�ȴ��ɿ�����
      return 0;	
    }
    break;
    
  case KEY2:
     if(P72)
      return 1;				//ĸ���ϰ���0
    else
    {
      while(P72==0);  //�ȴ��ɿ�����
      return 0;	
    }
    break;
    
  case KEY3:
    if(P73)
      return 1;				//ĸ���ϰ���3
    else
    {
      while(P73==0);  //�ȴ��ɿ�����
      return 0;	
    }
    break;
    
  case DSW0:
    return P32;				//ĸ���ϲ��뿪��0
    break;
    
  case DSW1:
    return P33;				//ĸ���ϲ��뿪��1
    break;
  default:
    return 0XFF;
  }
  return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������unsigned char KEY_Read_Comb(void)
����  �ܡ���ȡȫ������״̬,��ȡ��������״̬��������ϼ�ʹ��
������ֵ����
������ֵ���������״̬��0--7����״̬ 
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char KEY_Read_Comb(void)
{
  unsigned char tm=0;
  
  tm = P70 | (P7^1 << 1) | (P7^2 << 2);//��ȡ��������״̬������	
  if(tm==0x07)
  {
    return 0;
  }
  return  (~tm)&0X07;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void Test_GPIO_KEY(void)
����  �ܡ����Գ���,����KEY0/1/2����� LEDȫ��
������ֵ����
������ֵ���� 
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_GPIO_KEY(void)
{
  unsigned char k=0xFF;
  GPIO_LED_Init();
  GPIO_KEY_Init();
  while(1)
  {
    k= KEY_Read(KEY0)& KEY_Read(KEY1)& KEY_Read(KEY2)& KEY_Read(KEY3);
    
    if(k==0) LED_Ctrl(LEDALL, ON); //����KEY0/1/2����� LEDȫ��
    else     LED_Ctrl(LEDALL, OFF);//�ɿ�KEY0/1/2����� LEDȫ��
    
    delayms(100);                	//��ʱ�ȴ�
  }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void Test_ComKEY_Tft(void)
����  �ܡ�������ϰ������ڲ�����ʾ,����״̬������ʾ
������ֵ����
������ֵ���� 
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_ComKEY_Tft(void)
{
  unsigned char k=0xFF;
  OLED_Init();                                              	// OLED��ʼ��
  OLED_CLS();                                               	// ����		
  OLED_P14x16Str(0,0,(unsigned char*)"�����������ܿƼ�");		// �ַ�����ʾ
  OLED_P6x8Str(0,2,"Long Qiu i.s.t.");	                  	// �ַ�����ʾ
  delayms(1000);                                            	// ��ʱ�ȴ�
  OLED_CLS();                                             	// ����		
  OLED_P6x8Str(0,1,"KEY Pressed:  ");	                    	// �ַ�����ʾ
  GPIO_KEY_Init();
  while(1)
  {
    k= KEY_Read_Comb();
    switch(k)
    {
    case NOKEYDOWN:
      //OLED_P6x8Str(35,3,"NO key!  ");			// �ַ�����ʾ
      break;
    case KEY0DOWN:
      OLED_P6x8Str(35,3,"KEY0!    ");		  	// �ַ�����ʾ
      break;
    case KEY1DOWN:
      OLED_P6x8Str(35,3,"KEY1!    ");			// �ַ�����ʾ
      break;
    case KEY2DOWN:
      OLED_P6x8Str(35,3,"KEY2!    ");		  	// �ַ�����ʾ
      break;	
    case KEY01DOWN:
      OLED_P6x8Str(35,3,"KEY0&1!  ");		  	// �ַ�����ʾ
      break;
    case KEY02DOWN:
      OLED_P6x8Str(35,3,"KEY0&2!  ");		  	// �ַ�����ʾ
      break;
    case KEY12DOWN:
      OLED_P6x8Str(35,3,"KEY1&2!  ");		  	// �ַ�����ʾ
      break;
    case KEY012DOWN:
      OLED_P6x8Str(35,3,"KEY0&1&2!");	  		// �ַ�����ʾ
      break;
    default:
      break;
    }
    LED_Ctrl(LED0,RVS);                     		// �ĸ�LEDͬʱ��˸
    delayms(100);                       			// ��ʱ�ȴ�
  }
}



