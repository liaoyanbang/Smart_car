/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		adc
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC16F40K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/


#include "zf_adc.h"
#include "intrins.h"


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC��ʼ��
//  @param      adcn            ѡ��ADCͨ��
//  @param      speed      		ADCʱ��Ƶ��
//  @return     void
//  Sample usage:               adc_init(ADC_P10,ADC_SYSclk_DIV_2);//��ʼ��P1.0ΪADC����,ADCʱ��Ƶ�ʣ�SYSclk/2
//-------------------------------------------------------------------------------------------------------------------
void adc_init(ADCN_enum adcn,ADC_SPEED_enum speed)
{
	ADC_CONTR |= 1<<7;				//1 ���� ADC ��Դ
	
	ADC_CONTR &= (0xF0);			//���ADC_CHS[3:0] �� ADC ģ��ͨ��ѡ��λ
	ADC_CONTR |= adcn;
	
	if((adcn >> 3) == 1) //P0.0
	{
		//IO����Ҫ����Ϊ��������
		P0M0 &= ~(1 << (adcn & 0x07));
		P0M1 |= (1 << (adcn & 0x07));
	}
	else if((adcn >> 3) == 0) //P1.0	
	{
		//IO����Ҫ����Ϊ��������
		P1M0 &= ~(1 << (adcn & 0x07));
	    P1M1 |= (1 << (adcn & 0x07));
	}

	ADCCFG |= speed&0x0F;			//ADCʱ��Ƶ��SYSclk/2/speed&0x0F;
	
	ADCCFG |= 1<<5;					//ת������Ҷ��롣 ADC_RES �������ĸ� 2 λ�� ADC_RESL �������ĵ� 8 λ��
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADCת��һ��
//  @param      adcn            ѡ��ADCͨ��
//  @param      resolution      �ֱ���
//  @return     void
//  Sample usage:               adc_convert(ADC_P10, ADC_10BIT);
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_once(ADCN_enum adcn,ADCRES_enum resolution)
{
	uint16 adc_value;
	
	ADC_CONTR &= (0xF0);			//���ADC_CHS[3:0] �� ADC ģ��ͨ��ѡ��λ
	ADC_CONTR |= adcn;
	
	ADC_CONTR |= 0x40;  			// ���� AD ת��
	while (!(ADC_CONTR & 0x20));  	// ��ѯ ADC ��ɱ�־
	ADC_CONTR &= ~0x20;  			// ����ɱ�־
	
	adc_value = ADC_RES;  			//�洢 ADC �� 12 λ����ĸ� 4 λ
	adc_value <<= 8;
	adc_value |= ADC_RESL;  		//�洢 ADC �� 12 λ����ĵ� 8 λ
	
	ADC_RES = 0;
	ADC_RESL = 0;
	
	adc_value >>= resolution;		//ȡ����λ
	

	return adc_value;
}

uint16 ADC_Read(ADCN_enum ADC_Channel_x)
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
uint16 ADC_Read_Average(uint8 ADC_Channel_x,uint8 times)
{
  int32 temp_val=0;
  uint8 t;
  uint8 delay;
  for(t=0;t<times;t++)
  {
    temp_val+=ADC_Read(ADC_Channel_x);
    for(delay=0;delay<100;delay++);
  }
  return temp_val/times;
} 

