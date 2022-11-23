#ifndef _Pid_H 
#define _Pid_H
#include "headfile.h"
\
int abs_int(int p);


int Incremental_PI_L (int16 Encoder,int16 Target);
int Incremental_PI_R (int16 Encoder,int16 Target);

extern float Velocity_KP_L ,Velocity_KI_L ,Velocity_KD_L;

extern float Velocity_KP_R ,Velocity_KI_R ,Velocity_KD_R;


#endif