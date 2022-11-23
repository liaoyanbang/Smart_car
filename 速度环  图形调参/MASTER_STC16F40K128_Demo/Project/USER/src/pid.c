#include "pid.h"

float Velocity_KP_L = 6.0,Velocity_KI_L = 2.5 ,Velocity_KD_L = 0;

float Velocity_KP_R = 8.3,Velocity_KI_R = 2.2 ,Velocity_KD_R = 0;

int abs_int(int p)
{
	if(p>=0) return p;
	else return -p;
}

int Incremental_PI_L (int16 Encoder,int16 Target)
{ 	
	 static int16 Bias=0,Last_bias=0,Last_last_bias=0;
	 static int16 Pwm = 0;
	 Bias = Target-Encoder; //Calculate the deviation //计算偏差
	 Pwm += Velocity_KP_L * (Bias-Last_bias) + Velocity_KI_L* Bias + Velocity_KD_L*(Bias-2* Last_bias + Last_last_bias); 
	 if(Pwm>7200)Pwm= 7200;
	 if(Pwm<-7200)Pwm= -7200;
	 Last_last_bias= Last_bias;
	 Last_bias=Bias; //Save the last deviation //保存上一次偏差
	 return Pwm;    
}
int Incremental_PI_R (int16 Encoder,int16 Target)
{  
	 static int16 Bias=0,Last_bias=0,Last_last_bias=0;
	 static int16 Pwm = 0;
	 Bias = Target-Encoder; //Calculate the deviation //计算偏差
	 Pwm += Velocity_KP_R * (Bias-Last_bias) + Velocity_KI_R * Bias + Velocity_KD_R*(Bias- 2* Last_bias + Last_last_bias);  
	 if(Pwm>7200)Pwm= 7200;
	 if(Pwm<-7200)Pwm= -7200;
	 Last_last_bias= Last_bias;
	 Last_bias=Bias; //Save the last deviation //保存上一次偏差 
	 return Pwm;
}
