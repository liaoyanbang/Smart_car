#ifndef __LQINCLUDES_H
#define __LQINCLUDES_H	  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "STC16f.h"
#include "intrins.h"

typedef 	unsigned char	u8;
typedef 	unsigned char	uint8;
typedef 	unsigned char	uint8_t;
typedef 	unsigned int	u16;
typedef 	unsigned int	uint16;
typedef 	unsigned int	uint16_t;
typedef 	unsigned long	u32;
typedef 	unsigned long	uint32;
typedef 	unsigned long	uint32_t;
typedef 	signed char	int8;
typedef 	signed char	int8_t;
typedef 	signed int	int16;
typedef 	signed int	int16_t;
typedef 	signed long	int32;
typedef 	signed long	int32_t;

#include <LQ_GPIO_KEY.h>
#include <LQ_GPIO_LED.h>
#include <LQ_GPIO_EXIT.h>
#include <LQ_OLED096.h>
#include <LQ_MotorServo.h>
#include "LQ_ICM20689.h"
#include <LQ_UART.h>
#include <LQ_ADC.h>
#include "LQ_PWM.h"
#include <LQ_I2C.h>
#include <LQ_TIMER.h>
#include <LQ_SPI.h>
#include <LQ_Encoder.h>
#include <LQ_UART.h>
#include <LQ_SPI.h>
#include "math.h" 
#include "LQ_dianci.h"
#include <ANO_DT.h>

#include "LQ_SOFTI2C.h"

#define EnableInterrupts (EA = 1)                 //开总中断
#define DisableInterrupts (EA = 0)                //关总中断

#define MAIN_Fosc 30000000UL
//#define MAIN_Fosc 24000000UL
//#define MAIN_Fosc 33177600UL

void sysinit(void);

#endif











