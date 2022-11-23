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
#include "LQ_dianci.h"
#include "include.h"
short leftP=0,leftV=0,rightV=0,rightP=0;
short TempAngle = 0;   
short MotorDuty1=0; 				  // ��������ȫ�ֱ���
short MotorDuty2=0;						// �����ҵ��ȫ�ֱ���

short ECPULSE1 = 0;          // �����������ȫ�ֱ��� 
short ECPULSE2 = 0;          // �����ұ�����ȫ�ֱ���
short MtTargetDuty =0; 	   // ���PWM
short Kp=10;
short ServoDuty =0;



void timer0_int (void) interrupt 1
{
		ECPULSE1 = Read_Encoder(1); 			        // ���� ĸ���ϱ�����1��С��ǰ��Ϊ��ֵ
		ECPULSE2 = Read_Encoder(2); 			        // �ҵ�� ĸ���ϱ�����2��С������Ϊ��ֵ
		Control();
		ServoCtrl(Servo_Center-TempAngle);
	  MotorCtrl(MotorDuty1,MotorDuty2);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void InductorNormal (void)
����  �ܡ���ų���ʾ����
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned int data diangan[4];
void InductorNormal (void)
{
    diangan[0]=ADC_Read(8);    	// ��ȡADC8ͨ��ֵ   P00
    diangan[1]=ADC_Read(9);			// ��ȡADC9ͨ��ֵ	  P01
    diangan[2]=ADC_Read(10); 		// ��ȡADC10ͨ��ֵ	P02
    diangan[3]=ADC_Read(11);		// ��ȡADC11ͨ��ֵ	P03
		
		leftP =  (float)(diangan[0] - 100.0) / (2300.0 - 100.0) * 100.0;		// ��й�һ��
		leftV =  (float)(diangan[1] - 100.0) / (2300.0 - 100.0) * 100.0;		// ��й�һ��
		rightV = (float)(diangan[2] - 100.0) / (2300.0 - 100.0) * 100.0;		// ��й�һ��
		rightP = (float)(diangan[3] - 100.0) / (2300.0 - 100.0) * 100.0;		// ��й�һ��			
		
	  leftP = (leftP < 0) ? 0 : leftP;            //��һ���������������
		leftP = (leftP > 100) ? 100 : leftP;				//��һ���������������
		rightV = (rightV < 0) ? 0 : rightV;					//��һ���������������
		rightV = (rightV > 100) ? 100 : rightV;			//��һ���������������
	  leftV = (leftV < 0) ? 0 : leftV;						//��һ���������������
		leftV = (leftV > 100) ? 100 : leftV;				//��һ���������������
		rightP = (rightP < 0) ? 0 : rightP;					//��һ���������������
		rightP = (rightP > 100) ? 100 : rightP;			//��һ���������������			
}


int16_t Last_ServoDuty = 0; // ������
int16_t M_error=0,Last_Target=0;      // �����ٶ�
int time = 0;

int count = 1;             // �ɺ��ܴ�������
int Fork = 0,Forking=0,Forkout=0;              // ����·��־
int Round=0,Round_In=0,Rounding=0,Round_Out=0;
int Detection_L=0,Fork_R=0,Detection_R=0,Fork_L=0;

int RAllPulse=0;
void Control(void)
{
	InductorNormal();
	    // Բ������
   if(Round==1)
    {
      // Target_Speed =30;
     //  Target_Speed = 50;
        Discern();
			

    }
    // ����·�ڴ��� ��һȦ�Ҳ� �ڶ�Ȧ���
   ////////////�Ҳ�·//////////
    else if(Fork == 1&&count==0&&Round==0&&Fork_R==0)  // �Ҳ�·
    {
        TempAngle = -150;
       // Target_Speed = 40;
        Fork_R = 1;
			MtTargetDuty =150;

    }
    else if(rightP<5&&leftP<5&&Fork_R==1&&Forking==0)  // �ж��ڲ�·��
   {
        Forking=1;
   }

    else if(Forking==1&&Forkout==0&&Fork_R==1)				// ��·��ѭ��
    {
        TempAngle=(leftV-rightV)*220/(leftV+rightV);
       // Target_Speed = 40;
			  MtTargetDuty =150;

           if(leftP>18&&leftV>18)											// �жϲ�·����
            {
                Forkout=1;
                RAllPulse = 0;
							MtTargetDuty =180;
            }
    }
    else if(Forking==1&&Forkout==1&&Fork_R==1) 	//����· �����־λ
    {
        TempAngle=(leftV-rightV)*220/(leftV+rightV);
			  MtTargetDuty =180;

            Fork = 0;
            Fork_R=0;
            Forking=0;
            Forkout=0;

    }

       ///////////// ���·///////
    else if(Fork == 1&&count==1&&Round==0&&Fork_L==0)  // ���·
    {
        TempAngle = 180;
       // Target_Speed = 20;
        Fork_L = 1;
    }

    else if(rightP<5&&leftP<5&&Fork_L==1&&Forking==0)	// �ж��ڲ�·��
      {
           Forking=1;
      }

    else if(Forking==1&&Forkout==0&&Fork_L==1)// ��·��ѭ��
      {
        TempAngle=(leftV-rightV)*200/(leftV+rightV);
        //  Target_Speed = 20;
				MtTargetDuty =180;
           if(rightV>18&&rightP>18)		// �жϲ�·����
              {
                  Forkout=1;
                  RAllPulse = 0;
              }

      }
       else if(Forking==1&&Forkout==1&&Fork_L==1) //����· �����־λ
       {
           TempAngle=(leftV-rightV)*200/(leftV+rightV);
				   MtTargetDuty =180;
               Fork = 0;
               Fork_R=0;
               Fork_L=0;
               Forking=0;
               Forkout=0;
               count=0;
       }

     //����ѭ��
   else
   {
            // Բ��ʶ��
       if((Detection_L==0)&&(leftV>90)&&(rightV>80)&&(leftP<25))    //���ж�
       {

           Detection_L=1;
				   
         //  Target_Speed = 0;
       }
       else if(leftV>92&&rightV>70&&Round==0&&Detection_L==1&&(leftP>45)) //�󻷽�
       {
           Round = 1;
           Round_In = 1;
			    	

       }
            // ����·��ʶ��
          //  
      else if(leftP>15&&leftV<50&&rightV<50&&rightP>15&&Round==0&&Fork==0&&Forkout==0) //ʶ������·��
            {
                Fork = 1;
                MtTargetDuty =100;
            }
      else if(Round == 0&&Fork == 0)			// ����ѭ��
            {
							if((leftV<5)&&(rightV<5))
							{
								MtTargetDuty = 0;
							}
							else if((leftV>15)&&(rightV>15))
							{
								TempAngle=(leftV-rightV)*140/(leftV+rightV);
								MtTargetDuty = 250;
							}
							else 
							{
								TempAngle=(leftV-rightV)*330/(leftV+rightV);
								MtTargetDuty =200;
							}
            }
    }

    // ����ջ�����

	if(TempAngle>150)TempAngle=150;
	if(TempAngle<-150)TempAngle=-150;
	if(TempAngle<0)
	{
		MotorDuty1 = MtTargetDuty +TempAngle/2;//���� 
		MotorDuty2 = MtTargetDuty ;	
	}
		if(TempAngle>0)
	{
		MotorDuty1 = MtTargetDuty ;
		MotorDuty2 = MtTargetDuty-TempAngle/2 ;	
	}
}


void Discern(void) // ʶ��Բ�� ��Բ��ֻд���� �һ��Լ��ο���д
{
    if(Round_In==1)  // ��Բ��
    {
        TempAngle=(leftP-rightP)*400/(leftP+rightP);	//�л���ֱ���ѭ��
		  	MtTargetDuty =0;
    }
    if((rightP<20)&&(rightV<30)&&Round_In==1)					// �ж���Բ����
    {

        Rounding=1;
        Round_In=0;
    }
    else if(Rounding==1)												// Բ����ѭ�� �л�ˮƽ���ѭ��
    {
        TempAngle=(leftV-rightV)*200/(leftV+rightV);
			  MtTargetDuty =200;
    }
    if(Rounding==1&&((leftP>80&&leftV>80)))
    {
        Round_Out = 1;
        RAllPulse = 0;
    }
    if(Round_Out == 1)
    {

        TempAngle=(leftV-rightV)*280/(leftV+rightV);
			  MtTargetDuty =220;
        if(leftP<60&&leftV<60)           // ��Բ�� �����־λ
        {
         
            Round = 0;
            Rounding=0;
            Round_Out = 0;
            RAllPulse = 0;
            Detection_R=0;
        }
    }
}







/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void OLED_show (void)
����  �ܡ�OLED��ʾ����
����  �ߡ�chiusir
�������¡�2021��12��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void OLED_show(void)
{
	  char txt[30];
		sprintf(txt,"LP:%03d RP:%03d",leftP,rightP);		// ��ʾADC8��ֵ
    OLED_P6x8Str(0,0,(u8*)txt);
    sprintf(txt,"LV:%03d RV:%03d",leftV,rightV);		// ��ʾADC9��ֵ
    OLED_P6x8Str(0,1,(u8*)txt);
		sprintf(txt,"E:%04d %04d",ECPULSE1,ECPULSE2);		// ��ʾADC11��ֵ
    OLED_P6x8Str(0,2,(u8*)txt);
		sprintf(txt,"M:%04d Mt:%04d",MotorDuty1,MotorDuty2);	// ��ʾADC11��ֵ
    OLED_P6x8Str(0,3,(u8*)txt);
		sprintf(txt,"T:%04d",TempAngle);	// ��ʾADC11��ֵ
    OLED_P6x8Str(0,4,(u8*)txt);
}
