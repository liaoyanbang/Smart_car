/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		adc
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC16F40K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/


#include "zf_adc.h"
#include "intrins.h"


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC初始化
//  @param      adcn            选择ADC通道
//  @param      speed      		ADC时钟频率
//  @return     void
//  Sample usage:               adc_init(ADC_P10,ADC_SYSclk_DIV_2);//初始化P1.0为ADC功能,ADC时钟频率：SYSclk/2
//-------------------------------------------------------------------------------------------------------------------
void adc_init(ADCN_enum adcn,ADC_SPEED_enum speed)
{
	ADC_CONTR |= 1<<7;				//1 ：打开 ADC 电源
	
	ADC_CONTR &= (0xF0);			//清除ADC_CHS[3:0] ： ADC 模拟通道选择位
	ADC_CONTR |= adcn;
	
	if((adcn >> 3) == 1) //P0.0
	{
		//IO口需要设置为高阻输入
		P0M0 &= ~(1 << (adcn & 0x07));
		P0M1 |= (1 << (adcn & 0x07));
	}
	else if((adcn >> 3) == 0) //P1.0	
	{
		//IO口需要设置为高阻输入
		P1M0 &= ~(1 << (adcn & 0x07));
	    P1M1 |= (1 << (adcn & 0x07));
	}

	ADCCFG |= speed&0x0F;			//ADC时钟频率SYSclk/2/speed&0x0F;
	
	ADCCFG |= 1<<5;					//转换结果右对齐。 ADC_RES 保存结果的高 2 位， ADC_RESL 保存结果的低 8 位。
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC转换一次
//  @param      adcn            选择ADC通道
//  @param      resolution      分辨率
//  @return     void
//  Sample usage:               adc_convert(ADC_P10, ADC_10BIT);
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_once(ADCN_enum adcn,ADCRES_enum resolution)
{
	uint16 adc_value;
	
	ADC_CONTR &= (0xF0);			//清除ADC_CHS[3:0] ： ADC 模拟通道选择位
	ADC_CONTR |= adcn;
	
	ADC_CONTR |= 0x40;  			// 启动 AD 转换
	while (!(ADC_CONTR & 0x20));  	// 查询 ADC 完成标志
	ADC_CONTR &= ~0x20;  			// 清完成标志
	
	adc_value = ADC_RES;  			//存储 ADC 的 12 位结果的高 4 位
	adc_value <<= 8;
	adc_value |= ADC_RESL;  		//存储 ADC 的 12 位结果的低 8 位
	
	ADC_RES = 0;
	ADC_RESL = 0;
	
	adc_value >>= resolution;		//取多少位
	

	return adc_value;
}

uint16 ADC_Read(ADCN_enum ADC_Channel_x)
{ 
  unsigned short data temp;
	
  ADC_RES = 0;
  ADC_RESL = 0;
  
  ADC_CONTR &= 0xF0;
  ADC_CONTR = ADC_CONTR | 0x40 | ADC_Channel_x;    //启动 AD 转换
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  
  while((ADC_CONTR & 0x20) == 0)  ;   //wait for ADC finish
  ADC_CONTR &= ~0x20;     			//清除ADC结束标志
  temp = ADC_RES & 0x0F;
  temp = (temp<<8) | ADC_RESL;
  return temp;						//默认是12位
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】u16 ADC_Read_Average(uint8 ADC_Channel_x,uint8 times)
【功  能】获取几次ADC1采样值的平均值
【参数值】ADC_Channel_x , x为0~11
【返回值】ADC读到的值
【作  者】chiusir
【最后更新】2021年1月22日 
【软件版本】V1.0
【使用举例】x=ADC_Read(8,10); //读取ADC8通道值,10次均值
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

