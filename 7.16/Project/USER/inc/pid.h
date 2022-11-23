#ifndef _Pid_H 
#define _Pid_H
#include "headfile.h"

int abs_int(int p);
float abs_float(float p);
int Incremental_PI_L (int16 Encoder,int16 Target);
int Incremental_PI_R (int16 Encoder,int16 Target);


int16 pid_duoji_fork(float dig);//差比和  dig 0~1
int16 pid_duoji_round(float dig);//差比和  dig 0~1
int16 pid_duoji_normal_sqrt(float dig);//差比和  dig 0~1
int16 pid_duoji_cross(float dig);//差比和  dig 0~1


extern float Velocity_KP_L;
extern float Velocity_KI_L;
extern float Velocity_KP_R;
extern float Velocity_KI_R;
extern float Velocity_KD_L;
extern float Velocity_KD_R;


extern int16 duojikp_round;
extern int16 duojikd_round;
extern int16 duojikp_fork ;   
extern int16 duojikd_fork ;
extern int16 duojikp_normal_sqrt;
extern int16 duojikd_normal_sqrt;
#endif