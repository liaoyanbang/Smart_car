#include "pid.h"

float Velocity_KP_L = 80.0,Velocity_KI_L = 5.4 ,Velocity_KD_L = 46;

float Velocity_KP_R = 60.0,Velocity_KI_R = 5.0 ,Velocity_KD_R = 20;
//float Velocity_KP_L = 16,Velocity_KI_L = 3.6 ,Velocity_KD_L = 0;

//float Velocity_KP_R = 12.2,Velocity_KI_R = 4.3 ,Velocity_KD_R = 0;

//float Velocity_KP_L = 6.0,Velocity_KI_L = 2.5 ,Velocity_KD_L = 0;

//float Velocity_KP_R = 8.3,Velocity_KI_R = 2.2 ,Velocity_KD_R = 0;


	
int16 duojikp_normal_sqrt = 0;
int16 duojikd_normal_sqrt = 0;
	
int16 duojikp_fork = 0;
int16 duojikd_fork = 0;

int16 duojikp_round = 0;//123
int16 duojikd_round = 0;



int abs_int(int p)
{
	if(p>=0) return p;
	else return -p;
}
float abs_float(float p)
{
	if(p>=0.0) return p;
	else return -p;
}

int Incremental_PI_L (int16 Encoder,int16 Target)
{ 	
	 static int16 Bias=0,Last_bias=0,Last_last_bias=0;
	 static int16 Pwm = 0;
	 Bias = Target-Encoder; //Calculate the deviation //计算偏差
//	 if(Bias>=80)
//	 {
//		 Pwm = 7200;
//	 }
//	 else
//	 {
		 Pwm += Velocity_KP_L * (Bias-Last_bias) + Velocity_KI_L* Bias + Velocity_KD_L*(Bias-2* Last_bias + Last_last_bias); 
		 if(Pwm>7200)Pwm= 7200;
		 if(Pwm<-7200)Pwm= -7200;
//	 }
	 Last_last_bias= Last_bias;
	 Last_bias=Bias; //Save the last deviation //保存上一次偏差
	 return Pwm;    
}
int Incremental_PI_R (int16 Encoder,int16 Target)
{  
	 static int16 Bias=0,Last_bias=0,Last_last_bias=0;
	 static int16 Pwm = 0;
	 Bias = Target-Encoder; //Calculate the deviation //计算偏差
//	 if(Bias>=80)
//	 {
//		 Pwm = 7200;
//	 }
//	 else
//	 {
		 Pwm += Velocity_KP_L * (Bias-Last_bias) + Velocity_KI_L* Bias + Velocity_KD_L*(Bias-2* Last_bias + Last_last_bias); 
		 if(Pwm>7200)Pwm= 7200;
		 if(Pwm<-7200)Pwm= -7200;
//	 }
	 Last_last_bias= Last_bias;
	 Last_bias=Bias; //Save the last deviation //保存上一次偏差 
	 return Pwm;
}

int16 pid_duoji_normal_sqrt(float dig)//差比和  dig 0~1
{
	static float error = 0;
	static float last_error = 0;
	static int16 servoDuty = 0;
	error = dig;
	//duojikp_normal_sqrt = Fuzzy_sp(&dig4,&error_dig4);//Fuzzy_Update(&Duoji_P,dig4,error_dig4);
	servoDuty = duojikp_normal_sqrt * error + duojikd_normal_sqrt * (error-last_error);
	last_error = error;
	return servoDuty;
}


int16 pid_duoji_fork(float dig)//差比和  dig 0~1
{
	static float error = 0;
	static float last_error = 0;
	static int16 servoDuty = 0;
	error = dig;
	//duojikp_fork += Fuzzy_Update(&Duoji_P,dig4,error_dig4);
	servoDuty =  duojikp_fork* error  +duojikd_fork* (error-last_error);
	last_error = error;
	return servoDuty;
}


int16 pid_duoji_round(float dig)//差比和  dig 0~1
{
	static float error = 0;
	static float last_error = 0;
	static int16 servoDuty = 0;
	error = dig;
	//duojikp = Fuzzy_Update(&Duoji_P,error,last_error-error);
	servoDuty =  duojikp_round* error  +duojikd_round*(error-last_error);
	last_error = error;
	return servoDuty;
}
int16 pid_duoji_cross(float dig)//差比和  dig 0~1
{
	static float error = 0;
	static float last_error = 0;
	static int16 servoDuty = 0;
	error = dig;
	//duojikp = Fuzzy_Update(&Duoji_P,error,last_error-error);
	servoDuty =  200* error  +200*(error-last_error);
	last_error = error;
	return servoDuty;
}