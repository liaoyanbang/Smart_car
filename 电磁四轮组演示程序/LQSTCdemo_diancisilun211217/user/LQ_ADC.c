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
����  �ܡ���ȡADCת������
������ֵ��ADC_Channel_x , xΪ0~11
������ֵ��ADC������ֵ��Ĭ����12λ
�������¡�2021��1��22�� 
������汾��V1.0
��ʹ�þ�����x=ADC_Read(8); //��ȡADC8ͨ��ֵ
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
u16 ADC_Read(uint8 ADC_Channel_x)
{ 
  unsigned short data temp;
	
  ADC_RES = 0;
  ADC_RESL = 0;
  
  ADC_CONTR &= 0xF0;
  ADC_CONTR = ADC_CONTR | 0x40 | ADC_Channel_x;    //���� AD ת��
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  
  while((ADC_CONTR & 0x20) == 0)  ;   //wait for ADC finish
  ADC_CONTR &= ~0x20;     			//���ADC������־
  temp = ADC_RES & 0x0F;
  temp = (temp<<8) | ADC_RESL;
  return temp;						//Ĭ����12λ
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������u16 ADC_Read_Average(uint8 ADC_Channel_x,uint8 times)
����  �ܡ���ȡ����ADC1����ֵ��ƽ��ֵ
������ֵ��ADC_Channel_x , xΪ0~11
������ֵ��ADC������ֵ
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
��ʹ�þ�����x=ADC_Read(8,10); //��ȡADC8ͨ��ֵ,10�ξ�ֵ
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
u16 ADC_Read_Average(uint8 ADC_Channel_x,uint8 times)
{
  u32 temp_val=0;
  u8 t;
  u8 delay;
  for(t=0;t<times;t++)
  {
    temp_val+=ADC_Read(ADC_Channel_x);
    for(delay=0;delay<100;delay++);
  }
  return temp_val/times;
} 

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void ADC_Init (void)
����  �ܡ��ĸ����ADC��ʼ��������
������ֵ����
������ֵ���� 
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void ADC_Init (void)
{
  P_SW2 |= 0x80;
  ADCTIM = 0x3f;					//���� ADC �ڲ�ʱ��ADC����ʱ�佨�������ֵ
  P_SW2 &= 0x7f;
  ADCCFG |= 0x2f;					//���� ADC ʱ��Ϊϵͳʱ��/2/16/16
  ADC_CONTR = 0x80; 			//ʹ�� ADC ģ��
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void Test_ADC(void)
����  �ܡ����Գ�����Ļ��ʾADC�ɼ��ĵ��ֵ
������ֵ����
������ֵ���� 
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
��ʹ�þ�����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_ADC(void)
{
  char txt[30]; 
  unsigned int data diangan[4];
	
  OLED_Init();    // OLED��ʼ��
  OLED_CLS();  	// ����		
  OLED_P14x16Str(0,2,(unsigned char*)"�����������ܿƼ�");		// �ַ�����ʾ
  OLED_P6x8Str(0,4,"Long Qiu i.s.t.");						// �ַ�����ʾ
  delayms(1000);  // ��ʱ�ȴ�
  OLED_CLS();     // ����		
  ADC_Init();		// ADC��ʼ��
  while(1)
  {
    diangan[0]=ADC_Read(8);    		// ��ȡADC8ͨ��ֵ
    diangan[1]=ADC_Read(9);			// ��ȡADC9ͨ��ֵ
    diangan[2]=ADC_Read(10); 		// ��ȡADC10ͨ��ֵ
    diangan[3]=ADC_Read(11);		// ��ȡADC11ͨ��ֵ	
    sprintf(txt,"AD0:%05d ",diangan[0]);	// ��ʾADC8��ֵ
    OLED_P8x16Str(0,0,(u8*)txt);
    sprintf(txt,"AD1:%05d ",diangan[1]);	// ��ʾADC9��ֵ
    OLED_P8x16Str(0,2,(u8*)txt);
    sprintf(txt,"AD2:%05d ",diangan[2]);	// ��ʾADC10��ֵ
    OLED_P8x16Str(0,4,(u8*)txt);
    sprintf(txt,"AD3:%05d ",diangan[3]);	// ��ʾADC11��ֵ
    OLED_P8x16Str(0,6,(u8*)txt);
  }
}











