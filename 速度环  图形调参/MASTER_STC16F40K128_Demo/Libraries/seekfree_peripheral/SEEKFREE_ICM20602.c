/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		ICM20602
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 * @note		
					接线定义：
					------------------------------------ 

					------------------------------------ 
 ********************************************************************************************************************/


#include "SEEKFREE_ICM20602.h"
#include "SEEKFREE_IIC.h"
#include "zf_delay.h"
#include "zf_spi.h"

int16 icm_gyro_x,icm_gyro_y,icm_gyro_z;
int16 icm_acc_x,icm_acc_y,icm_acc_z;



//-------------------------------------------------------------------------------------------------------------------
//  以下函数是使用软件IIC通信，相比较硬件IIC，软件IIC引脚更加灵活，可以使用任意普通IO
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602自检函数
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				调用该函数前，请先调用模拟IIC的初始化
//-------------------------------------------------------------------------------------------------------------------
void icm20602_self1_check(void)
{
    uint8 dat;
	dat = simiic_read_reg(ICM20602_DEV_ADDR,ICM20602_WHO_AM_I,SIMIIC);
    while(0x12 != dat)   //读取ICM20602 ID
    {
        dat = simiic_read_reg(ICM20602_DEV_ADDR,ICM20602_WHO_AM_I,SIMIIC);
        delay_ms(10);
        //卡在这里原因有以下几点
        //1 ICM20602坏了，如果是新的这样的概率极低
        //2 接线错误或者没有接好
        //3 可能你需要外接上拉电阻，上拉到3.3V
    }
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      软件IIC，初始化ICM20602
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				调用该函数前，请先调用模拟IIC的初始化
//-------------------------------------------------------------------------------------------------------------------
void icm20602_init_simiic(void)
{
    delay_ms(10);  //上电延时

    //检测
    icm20602_self1_check();

    //复位
    simiic_write_reg(ICM20602_DEV_ADDR,ICM20602_PWR_MGMT_1,0x80);               //复位设备
    delay_ms(2);                                                        //延时
    while(0x80 & simiic_read_reg(ICM20602_DEV_ADDR,ICM20602_PWR_MGMT_1,SIMIIC));//等待复位完成

    //配置参数
    simiic_write_reg(ICM20602_DEV_ADDR,ICM20602_PWR_MGMT_1,0x01);               //时钟设置
    simiic_write_reg(ICM20602_DEV_ADDR,ICM20602_PWR_MGMT_2,0x00);               //开启陀螺仪和加速度计
    simiic_write_reg(ICM20602_DEV_ADDR,ICM20602_CONFIG,0x01);                   //176HZ 1KHZ
    simiic_write_reg(ICM20602_DEV_ADDR,ICM20602_SMPLRT_DIV,0x07);               //采样速率 SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
    simiic_write_reg(ICM20602_DEV_ADDR,ICM20602_GYRO_CONFIG,0x18);              //±2000 dps
    simiic_write_reg(ICM20602_DEV_ADDR,ICM20602_ACCEL_CONFIG,0x10);             //±8g
    simiic_write_reg(ICM20602_DEV_ADDR,ICM20602_ACCEL_CONFIG_2,0x03);           //Average 4 samples   44.8HZ   //0x23 Average 16 samples
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取ICM20602加速度计数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void get_icm20602_accdata_simiic(void)
{
    uint8 dat[6];

    simiic_read_regs(ICM20602_DEV_ADDR, ICM20602_ACCEL_XOUT_H, dat, 6, SIMIIC);
    icm_acc_x = (int16)(((uint16)dat[0]<<8 | dat[1]));
    icm_acc_y = (int16)(((uint16)dat[2]<<8 | dat[3]));
    icm_acc_z = (int16)(((uint16)dat[4]<<8 | dat[5]));
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取ICM20602陀螺仪数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void get_icm20602_gyro_simiic(void)
{
    uint8 dat[6];

    simiic_read_regs(ICM20602_DEV_ADDR, ICM20602_GYRO_XOUT_H, dat, 6, SIMIIC);
    icm_gyro_x = (int16)(((uint16)dat[0]<<8 | dat[1]));
    icm_gyro_y = (int16)(((uint16)dat[2]<<8 | dat[3]));
    icm_gyro_z = (int16)(((uint16)dat[4]<<8 | dat[5]));
}




//-------------------------------------------------------------------------------------------------------------------
//  以下函数是使用软件SPI通信，相比较硬件SPI，软件SPI引脚更加灵活，可以使用任意普通IO
//-------------------------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------------------------
//  @brief      通过SPI写一个byte,同时读取一个byte
//  @param      byte        发送的数据    
//  @return     uint8       return 返回status状态
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 icm_simspi_wr_byte(uint8 byte)
{
    uint8 i;
	
    for(i=0; i<8; i++)
    {
        ICM20602_MOSI(byte&0x80);
        byte <<= 1;
		ICM20602_SCK (0);
		
        ICM20602_SCK (1);
		byte |= ICM20602_MISO; 
    }	
    return(byte);                                      		
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      将val写入cmd对应的寄存器地址,同时返回status字节
//  @param      cmd         命令字
//  @param      val         待写入寄存器的数值
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm_simspi_w_reg_byte(uint8 cmd, uint8 val)
{
    ICM20602_CSN (0);
    cmd |= ICM20602_SPI_W;
    icm_simspi_wr_byte(cmd);                      	
    icm_simspi_wr_byte(val);                               	
    ICM20602_CSN (1);                                    	
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取cmd所对应的寄存器地址
//  @param      cmd         命令字
//  @param      *val        存储读取的数据地址
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm_simspi_r_reg_byte(uint8 cmd, uint8 *val)
{
    ICM20602_CSN (0);
    cmd |= ICM20602_SPI_R;
    icm_simspi_wr_byte(cmd);                               	
    *val = icm_simspi_wr_byte(0);                           	
    ICM20602_CSN (1);                                    	
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取cmd所对应的寄存器地址
//  @param      cmd         命令字
//  @param      *val        存储读取的数据地址
//  @param      num         读取的数量
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm_simspi_r_reg_bytes(uint8 cmd, uint8 *val, uint8 num)
{
    uint16 i;
    ICM20602_CSN (0);
    cmd |= ICM20602_SPI_R;
    icm_simspi_wr_byte(cmd);
	
	for(i=0; i<num; i++)	
		val[i] = icm_simspi_wr_byte(0);    

	
    ICM20602_CSN (1);                                    		
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602自检函数
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void icm20602_self4_check(void)
{
    uint8 val;
    do
    {
        icm_simspi_r_reg_byte(ICM20602_WHO_AM_I,&val);
		
        //卡在这里原因有以下几点
        //1 ICM20602坏了，如果是新的这样的概率极低
        //2 接线错误或者没有接好
        //3 可能你需要外接上拉电阻，上拉到3.3V
    }while(0x12 != val);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      软件SPI，初始化ICM20602
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void icm20602_init_simspi(void)
{
    uint8 val;

    icm20602_self4_check();//检测
    
    icm_simspi_w_reg_byte(ICM20602_PWR_MGMT_1,0x80);//复位设备
    delay_ms(2);
    do
    {//等待复位成功
        icm_simspi_r_reg_byte(ICM20602_PWR_MGMT_1,&val);
    }while(0x41 != val);
    
    icm_simspi_w_reg_byte(ICM20602_PWR_MGMT_1,     0x01);            //时钟设置
    icm_simspi_w_reg_byte(ICM20602_PWR_MGMT_2,     0x00);            //开启陀螺仪和加速度计
    icm_simspi_w_reg_byte(ICM20602_CONFIG,         0x01);            //176HZ 1KHZ
    icm_simspi_w_reg_byte(ICM20602_SMPLRT_DIV,     0x07);            //采样速率 SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
    icm_simspi_w_reg_byte(ICM20602_GYRO_CONFIG,    0x18);            //±2000 dps
    icm_simspi_w_reg_byte(ICM20602_ACCEL_CONFIG,   0x10);            //±8g
    icm_simspi_w_reg_byte(ICM20602_ACCEL_CONFIG_2, 0x03);            //Average 8 samples   44.8HZ
    
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取ICM20602加速度计数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void get_icm20602_accdata_simspi(void)
{
    uint8 dat[6];

    
    icm_simspi_r_reg_bytes(ICM20602_ACCEL_XOUT_H, dat, 6);
    icm_acc_x = (int16)(((uint16)dat[0]<<8 | dat[1]));
    icm_acc_y = (int16)(((uint16)dat[2]<<8 | dat[3]));
    icm_acc_z = (int16)(((uint16)dat[4]<<8 | dat[5]));
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取ICM20602陀螺仪数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void get_icm20602_gyro_simspi(void)
{
    uint8 dat[6];
    
    icm_simspi_r_reg_bytes(ICM20602_GYRO_XOUT_H, dat, 6);

    icm_gyro_x = (int16)(((uint16)dat[0]<<8 | dat[1]));
    icm_gyro_y = (int16)(((uint16)dat[2]<<8 | dat[3]));
    icm_gyro_z = (int16)(((uint16)dat[4]<<8 | dat[5]));
}


float GYRO_Ang_Y=0.0;
float ACC_GYRO_Ang_Y=0.0;
//***************反正切Y轴角度*************************//
/*
 * 函数名：ICM20689_ACC_angle_X_Z
 * 描述  ：加速度计反正切角度
 * 输入  ：无
 * 输出  ：角度值
 * 调用  ：内部调用
 */

float ICM20602_ACC_angle_X_Z(void)                       
{
  get_icm20602_accdata_simspi();
  return (atan2((float)icm_acc_z,(float)icm_acc_x)*(180.0/PI));   //反正切加速度计得到角度值
}

//**************积分Y轴角度*************************//
/*
 * 函数名：ICM20689_GYRO_Y_Ang(void)
 * 描述  ：积分Y轴角度
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
 
void ICM20602_GYRO_Y_Ang()
{
  get_icm20602_gyro_simspi();   //获取Y轴陀螺仪值
  GYRO_Ang_Y +=((float)icm_gyro_y)*0.00032;  //积分角度
}

//***************加速度计和陀螺仪数据融合*****************************//

void ICM20602_GYRO_ACCEL_Ang_Y(void)
{	
	static float temp_p = 0.0;
	float ACCEL_Ang_Y=0.0;
  ICM20602_GYRO_Y_Ang();
  ACCEL_Ang_Y=ICM20602_ACC_angle_X_Z();
  temp_p=(ACCEL_Ang_Y-GYRO_Ang_Y)*0.025;//融合系数  //0.025
  GYRO_Ang_Y=GYRO_Ang_Y+temp_p;  
  ACC_GYRO_Ang_Y=GYRO_Ang_Y;
}

int16 GyroOffset_x,GyroOffset_y,GyroOffset_z;// 陀螺仪校准值 
/**
 * @brief 陀螺仪零漂初始化
 * 通过采集一定数据求均值计算陀螺仪零点偏移值。
 * 后续 陀螺仪读取的数据 - 零飘值，即可去除零点偏移量。
 */
void gyroOffsetInit(void)     
{
		int16 i =0;
    GyroOffset_x = 0;
    GyroOffset_y = 0;
    GyroOffset_z = 0;
    for ( i = 0; i < 10; ++i) 
		{
        get_icm20602_gyro_simspi();    // 获取陀螺仪角速度
        GyroOffset_x += icm_gyro_x;
        GyroOffset_y += icm_gyro_y;
        GyroOffset_z += icm_gyro_z;
        delay_ms(5);    // 最大 1Khz
    }
    GyroOffset_x /= 100;
    GyroOffset_y /= 100;
    GyroOffset_z /= 100;
}

/**
 * @brief 将采集的数值转化为实际物理值, 并对陀螺仪进行去零漂处理
 * 加速度计初始化配置 -> 测量范围: ±8g        对应灵敏度: 4096 LSB/g
 * 陀螺仪初始化配置   -> 测量范围: ±2000 dps  对应灵敏度: 16.4 LSB/dps   (degree per second)
 * @tips: gyro = (gyro_val / 16.4) °/s = ((gyro_val / 16.4) * PI / 180) rad/s
 */

//void icmGetValues(void) 
//{
//    float alpha = 0.451;//要改

//    //一阶低通滤波，单位g
//    icm_data.acc_x = (((float) icm_acc_x) * alpha) / 4096 + icm_data.acc_x * (1 - alpha);
//    icm_data.acc_y = (((float) icm_acc_y) * alpha) / 4096 + icm_data.acc_y * (1 - alpha);
//    icm_data.acc_z = (((float) icm_acc_z) * alpha) / 4096 + icm_data.acc_z * (1 - alpha);

//    //! 陀螺仪角速度必须转换为弧度制角速度: deg/s -> rad/s
//    icm_data.gyro_x = ((float) icm_gyro_x - GyroOffset_x) * PI / 180 / 16.4f; 
//    icm_data.gyro_y = ((float) icm_gyro_y - GyroOffset_y) * PI / 180 / 16.4f;
//    icm_data.gyro_z = ((float) icm_gyro_z - GyroOffset_z) * PI / 180 / 16.4f;
//}

//#define delta_T     0.001f  // 采样周期1ms 即频率1KHZ

//float I_ex, I_ey, I_ez;  // 误差积分
//quater_param_t Q_info = {1, 0, 0, 0};  // 四元数初始化
//euler_param_t eulerAngle;              // 欧拉角
//Icm_data icm_data;               // ICM20602采集的六轴数值

//float icm_kp= 0.17;    // 加速度计的收敛速率比例增益
//float icm_ki= 0.004;   // 陀螺仪收敛速率的积分增益

///**
// * @brief 用互补滤波算法解算陀螺仪姿态(即利用加速度计修正陀螺仪的积分误差)
// * 加速度计对振动之类的噪声比较敏感，长期数据计算出的姿态可信；陀螺仪对振动噪声不敏感，短期数据可信，但长期使用积分误差严重(内部积分算法放大静态误差)。
// * 因此使用姿态互补滤波，短期相信陀螺仪，长期相信加速度计。
// * @tips: n - 导航坐标系； b - 载体坐标系
// */
//#define sampleFreq	512.0f		// sample frequency in Hz
//#define betaDef		0.1f		// 2 * proportional gain

//---------------------------------------------------------------------------------------------------
// Variable definitions

//volatile float beta = betaDef;	
//void icmAHRSupdate(Icm_data* icm) 
//{
//	float recipNorm;
//	float s0, s1, s2, s3;
//	float qDot1, qDot2, qDot3, qDot4;
//	float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3;
//	float q0 = Q_info.q0;  //四元数
//  float q1 = Q_info.q1;
//  float q2 = Q_info.q2;
//  float q3 = Q_info.q3;
//	// Rate of change of quaternion from gyroscope
//	qDot1 = 0.5f * (-q1 * icm->gyro_x - q2 * icm->gyro_y - q3 * icm->gyro_z);
//	qDot2 = 0.5f * (q0 * icm->gyro_x + q2 * icm->gyro_z - q3 * icm->gyro_y);
//	qDot3 = 0.5f * (q0 * icm->gyro_y - q1 * icm->gyro_z + q3 * icm->gyro_x);
//	qDot4 = 0.5f * (q0 * icm->gyro_z + q1 * icm->gyro_y - q2 * icm->gyro_x);

//	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
//	if(!((icm->acc_x == 0.0f) && (icm->acc_y == 0.0f) && (icm->acc_z == 0.0f))) {

//		// Normalise accelerometer measurement
//		recipNorm = invSqrt(icm->acc_x * icm->acc_x + icm->acc_y * icm->acc_y + icm->acc_z * icm->acc_z);
//		icm->acc_x *= recipNorm;
//		icm->acc_y *= recipNorm;
//		icm->acc_z *= recipNorm;   

//		// Auxiliary variables to avoid repeated arithmetic
//		_2q0 = 2.0f * q0;
//		_2q1 = 2.0f * q1;
//		_2q2 = 2.0f * q2;
//		_2q3 = 2.0f * q3;
//		_4q0 = 4.0f * q0;
//		_4q1 = 4.0f * q1;
//		_4q2 = 4.0f * q2;
//		_8q1 = 8.0f * q1;
//		_8q2 = 8.0f * q2;
//		q0q0 = q0 * q0;
//		q1q1 = q1 * q1;
//		q2q2 = q2 * q2;
//		q3q3 = q3 * q3;

//		// Gradient decent algorithm corrective step
//		s0 = _4q0 * q2q2 + _2q2 * icm->acc_x + _4q0 * q1q1 - _2q1 * icm->acc_y;
//		s1 = _4q1 * q3q3 - _2q3 * icm->acc_x + 4.0f * q0q0 * q1 - _2q0 * icm->acc_y - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * icm->acc_z;
//		s2 = 4.0f * q0q0 * q2 + _2q0 * icm->acc_x + _4q2 * q3q3 - _2q3 * icm->acc_y - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * icm->acc_z;
//		s3 = 4.0f * q1q1 * q3 - _2q1 * icm->acc_x + 4.0f * q2q2 * q3 - _2q2 * icm->acc_y;
//		recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
//		s0 *= recipNorm;
//		s1 *= recipNorm;
//		s2 *= recipNorm;
//		s3 *= recipNorm;

//		// Apply feedback step
//		qDot1 -= beta * s0;
//		qDot2 -= beta * s1;
//		qDot3 -= beta * s2;
//		qDot4 -= beta * s3;
//	}

//	// Integrate rate of change of quaternion to yield quaternion
//	q0 += qDot1 * (1.0f / sampleFreq);
//	q1 += qDot2 * (1.0f / sampleFreq);
//	q2 += qDot3 * (1.0f / sampleFreq);
//	q3 += qDot4 * (1.0f / sampleFreq);

//	// Normalise quaternion
//	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
//	q0 *= recipNorm;
//	q1 *= recipNorm;
//	q2 *= recipNorm;
//	q3 *= recipNorm;
//}
//float invSqrt(float x) 
//{
//	float halfx = 0.5f * x;
//	float y = x;
//	long i = *(long*)&y;
//	i = 0x5f3759df - (i>>1);
//	y = *(float*)&i;
//	y = y * (1.5f - (halfx * y * y));
//	return y;
//}