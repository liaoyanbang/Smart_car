/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�STC16���İ�
����    �ġ�chiusir
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

u8 Uart4_Receive;
#define Baudrate1   (65536 - MAIN_Fosc / 115200 / 4)
#define Baudrate4   (65536 - MAIN_Fosc / 115200 / 4)

//========================================================================
// ����: void UART1_int (void) interrupt UART1_VECTOR
// ����: UART1�жϺ�����
// ����: nine.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
void UART1_int (void) interrupt 4
{
  if(RI)
  {
    RI = 0;    
  }
  
  if(TI)
  {
    TI = 0;    
  }
}
//========================================================================
// ����: void UART4_int (void) interrupt UART4_VECTOR
// ����: UART4�жϺ���������ң�ء�
// ����: nine.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
extern short MotorDuty1, MotorDuty2,ServoDuty;				  // ������PMW����
extern unsigned char Flag_Stop;
extern short Velocity;

void UART4_int (void) interrupt 18
{  
  static int uart_receive=0;//����������ر���
  static u8 Flag_PID,i,j,Receive[50];                      
  static float Data;                                       
  
  if((S4CON & 0x01) != 0)                                  
    {                                                        
		S4CON &= ~0x01;                               //Clear Rx flag
                                                                 
		uart_receive = S4BUF;                                      
		Uart4_Receive = uart_receive;                              
	                                           
	
	}  											 

    if((S4CON & 0x02) != 0)
    {
        S4CON &= ~0x02;                           //Clear Tx flag        
    } 
}
//========================================================================
// ����: SetTimer2Baudraye(u32 dat)
// ����: ����Timer2�������ʷ�������
// ����: dat: Timer2����װֵ.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
void SetTimer2Baudraye(u32 dat)
{
  AUXR &= ~(1<<4);                                         //Timer stop
  AUXR &= ~(1<<3);                                         //Timer2 set As Timer
  AUXR |=  (1<<2);                                         //Timer2 set as 1T mode
  T2H = (u8)(dat / 256);
  T2L = (u8)(dat % 256);
  IE2  &= ~(1<<2);                                         //��ֹ�ж�
  AUXR |=  (1<<4);                                         //Timer run enable
}
//========================================================================
// ����: void   UART1_config(u16 tmr)
// ����: UART1��ʼ��������
// ����: tmr: ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer1��������.
// ����: none.
// �汾: VER1.0
// ����: 2014-11-28
// ��ע: 
//========================================================================
void UART1_config(u16 tmr)
{  
  /*********** ������ʹ�ö�ʱ��2 *****************/
  if(tmr == 2)
  {
    AUXR |= 0x01;                       //S1 tmr Use Timer2;
    SetTimer2Baudraye(Baudrate1);
  }
  /*********** ������ʹ�ö�ʱ��1 *****************/
  else
  {
    TR1 = 0;
    AUXR &= ~0x01;                       //S1 tmr Use Timer1;
    AUXR |=  (1<<6);                     //Timer1 set as 1T mode
    TMOD &= ~(1<<6);                     //Timer1 set As Timer
    TMOD &= ~0x30;                       //Timer1_16bitAutoReload;
    TH1 = (u8)(Baudrate1 / 256);
    TL1 = (u8)(Baudrate1 % 256);
    ET1 = 0;                             //��ֹ�ж�
    INTCLKO &= ~0x02;                                         //�����ʱ��
    TR1  = 1;
  }
  /*************************************************/  
  SCON = (SCON & 0x3f) | 0x40;                                //UART1ģʽ, 0x00: ͬ����λ���, 0x40: 8λ����,�ɱ䲨����, 0x80: 9λ����,�̶�������, 0xc0: 9λ����,�ɱ䲨����
  //  PS  = 1;                                                    //�����ȼ��ж�
  ES  = 1;                                                    //�����ж�
  REN = 1;                                                    //�������
  P_SW1 &= 0x3f;
  //  P_SW1 |= 0x00;                                          //UART1 switch to, 0x00: P3.0 P3.1, 0x40: P3.6 P3.7, 0x80: P1.6 P1.7, 0xC0: P4.3 P4.4
}

void UART4_config(u16 tmr)                                    // ѡ������, 2: ʹ��Timer2��������, ����ֵ: ʹ��Timer4��������.
{
  if(tmr == 2)
  {
    SetTimer2Baudraye(Baudrate4);
    S4CON = 0x10;                                            //8λ����, ʹ��Timer2�������ʷ�����, �������
  }
  else
  {
    S4CON = 0x50;                                            //8λ����, ʹ��Timer4�������ʷ�����, �������
    T4H = (Baudrate4) / 256;
    T4L = (Baudrate4) % 256;
    T4T3M = 0xA0;
  }
  IE2 |= 0x10;                                               //����UART4�ж�
  //    P_SW2 &= ~0x04;                                      //UART4 switch bit2 to: 0: P0.2 P0.3
  P_SW2 |= 0x04;                                             //UART4 switch bit2 to: 1: P5.2 P5.3
}

/*************************************************************************
*  �������ƣ�void UART_PutChar(UART_t  uratn, char ch)
*  ����˵����UART�����ֽں���,ʹ��ǰ���ȳ�ʼ����Ӧ����
*  ����˵����uratn �� UART0 - UART3/ ch    �� Ҫ��ӡ���ַ�
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��UART_PutChar(UART0, 'a');  //��ӡ�ַ�a
*************************************************************************/
void UART1_PutChar(char ch)
{
  SBUF = ch;    
}
void UART4_PutChar(char ch)
{
  S4BUF = ch;    
}

/*************************************************************************
*  �������ƣ�void UART_PutStr(UART_t  uratn, char *st)
*  ����˵����UART�����ַ�������(�� NULL ֹͣ����),ʹ��ǰ���ȳ�ʼ����Ӧ����
*  ����˵����uratn �� UART0 - UART3/ str   �� Ҫ��ӡ���ַ�����ַ
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��UART_PutStr(UART3, "123456789"); //����9���ֽ�
*************************************************************************/
void UART1_PutStr(char *st)
{
  while(*st)
  {
    UART1_PutChar(*st++);
  }
}
void UART4_PutStr(char *st)
{
  while(*st)
  {
    UART4_PutChar(*st++);
  }
}

/*************************************************************************
*  �������ƣ�void UART4_PutBuff(unsigned char *buff, unsigned long len)
*  ����˵����UART�����ֽں���,ʹ��ǰ���ȳ�ʼ����Ӧ����
*  ����˵����
* @param    buff  �� Ҫ��ӡ���ַ�����ַ
* @param    len   �� Ҫ��ӡ�ĳ���
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��UART4_PutBuff("123456789",5);//ʵ�ʷ���5���ֽڡ�1����2����3����4����5��
*************************************************************************/
void UART4_PutBuff(unsigned char *buff, unsigned long len)
{
  while(len--)
  {
    UART4_PutChar(*buff);
    buff++;
  }
}

/*************************************************************************
*  �������ƣ�char UART_GetChar(UART_t  uratn)
*  ����˵����UART��ȡ�ֽ� ʹ��ǰ���ȳ�ʼ����Ӧ���� ����ǰ��ȷ���н��յ����� �����ȴ����ݵ���
*  ����˵����uratn �� UART0 - UART3
*  �������أ���ȡ�ֽ�
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��UART_GetChar(UART3); //����һ���ַ�
*************************************************************************/
char UART1_GetChar(void)
{  
  return (SBUF);
}
char UART4_GetChar(void)
{  
  return (S4BUF);
}

//
