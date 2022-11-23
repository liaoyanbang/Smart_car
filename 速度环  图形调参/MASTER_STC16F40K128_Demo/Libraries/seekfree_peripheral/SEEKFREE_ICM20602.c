/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		ICM20602
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 * @note		
					���߶��壺
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
//  ���º�����ʹ�����IICͨ�ţ���Ƚ�Ӳ��IIC�����IIC���Ÿ���������ʹ��������ͨIO
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602�Լ캯��
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				���øú���ǰ�����ȵ���ģ��IIC�ĳ�ʼ��
//-------------------------------------------------------------------------------------------------------------------
void icm20602_self1_check(void)
{
    uint8 dat;
	dat = simiic_read_reg(ICM20602_DEV_ADDR,ICM20602_WHO_AM_I,SIMIIC);
    while(0x12 != dat)   //��ȡICM20602 ID
    {
        dat = simiic_read_reg(ICM20602_DEV_ADDR,ICM20602_WHO_AM_I,SIMIIC);
        delay_ms(10);
        //��������ԭ�������¼���
        //1 ICM20602���ˣ�������µ������ĸ��ʼ���
        //2 ���ߴ������û�нӺ�
        //3 ��������Ҫ����������裬������3.3V
    }
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���IIC����ʼ��ICM20602
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				���øú���ǰ�����ȵ���ģ��IIC�ĳ�ʼ��
//-------------------------------------------------------------------------------------------------------------------
void icm20602_init_simiic(void)
{
    delay_ms(10);  //�ϵ���ʱ

    //���
    icm20602_self1_check();

    //��λ
    simiic_write_reg(ICM20602_DEV_ADDR,ICM20602_PWR_MGMT_1,0x80);               //��λ�豸
    delay_ms(2);                                                        //��ʱ
    while(0x80 & simiic_read_reg(ICM20602_DEV_ADDR,ICM20602_PWR_MGMT_1,SIMIIC));//�ȴ���λ���

    //���ò���
    simiic_write_reg(ICM20602_DEV_ADDR,ICM20602_PWR_MGMT_1,0x01);               //ʱ������
    simiic_write_reg(ICM20602_DEV_ADDR,ICM20602_PWR_MGMT_2,0x00);               //���������Ǻͼ��ٶȼ�
    simiic_write_reg(ICM20602_DEV_ADDR,ICM20602_CONFIG,0x01);                   //176HZ 1KHZ
    simiic_write_reg(ICM20602_DEV_ADDR,ICM20602_SMPLRT_DIV,0x07);               //�������� SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
    simiic_write_reg(ICM20602_DEV_ADDR,ICM20602_GYRO_CONFIG,0x18);              //��2000 dps
    simiic_write_reg(ICM20602_DEV_ADDR,ICM20602_ACCEL_CONFIG,0x10);             //��8g
    simiic_write_reg(ICM20602_DEV_ADDR,ICM20602_ACCEL_CONFIG_2,0x03);           //Average 4 samples   44.8HZ   //0x23 Average 16 samples
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡICM20602���ٶȼ�����
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
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
//  @brief      ��ȡICM20602����������
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
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
//  ���º�����ʹ�����SPIͨ�ţ���Ƚ�Ӳ��SPI�����SPI���Ÿ���������ʹ��������ͨIO
//-------------------------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ͨ��SPIдһ��byte,ͬʱ��ȡһ��byte
//  @param      byte        ���͵�����    
//  @return     uint8       return ����status״̬
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
//  @brief      ��valд��cmd��Ӧ�ļĴ�����ַ,ͬʱ����status�ֽ�
//  @param      cmd         ������
//  @param      val         ��д��Ĵ�������ֵ
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
//  @brief      ��ȡcmd����Ӧ�ļĴ�����ַ
//  @param      cmd         ������
//  @param      *val        �洢��ȡ�����ݵ�ַ
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
//  @brief      ��ȡcmd����Ӧ�ļĴ�����ַ
//  @param      cmd         ������
//  @param      *val        �洢��ȡ�����ݵ�ַ
//  @param      num         ��ȡ������
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
//  @brief      ICM20602�Լ캯��
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
		
        //��������ԭ�������¼���
        //1 ICM20602���ˣ�������µ������ĸ��ʼ���
        //2 ���ߴ������û�нӺ�
        //3 ��������Ҫ����������裬������3.3V
    }while(0x12 != val);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���SPI����ʼ��ICM20602
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void icm20602_init_simspi(void)
{
    uint8 val;

    icm20602_self4_check();//���
    
    icm_simspi_w_reg_byte(ICM20602_PWR_MGMT_1,0x80);//��λ�豸
    delay_ms(2);
    do
    {//�ȴ���λ�ɹ�
        icm_simspi_r_reg_byte(ICM20602_PWR_MGMT_1,&val);
    }while(0x41 != val);
    
    icm_simspi_w_reg_byte(ICM20602_PWR_MGMT_1,     0x01);            //ʱ������
    icm_simspi_w_reg_byte(ICM20602_PWR_MGMT_2,     0x00);            //���������Ǻͼ��ٶȼ�
    icm_simspi_w_reg_byte(ICM20602_CONFIG,         0x01);            //176HZ 1KHZ
    icm_simspi_w_reg_byte(ICM20602_SMPLRT_DIV,     0x07);            //�������� SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
    icm_simspi_w_reg_byte(ICM20602_GYRO_CONFIG,    0x18);            //��2000 dps
    icm_simspi_w_reg_byte(ICM20602_ACCEL_CONFIG,   0x10);            //��8g
    icm_simspi_w_reg_byte(ICM20602_ACCEL_CONFIG_2, 0x03);            //Average 8 samples   44.8HZ
    
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡICM20602���ٶȼ�����
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
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
//  @brief      ��ȡICM20602����������
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
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
//***************������Y��Ƕ�*************************//
/*
 * ��������ICM20689_ACC_angle_X_Z
 * ����  �����ٶȼƷ����нǶ�
 * ����  ����
 * ���  ���Ƕ�ֵ
 * ����  ���ڲ�����
 */

float ICM20602_ACC_angle_X_Z(void)                       
{
  get_icm20602_accdata_simspi();
  return (atan2((float)icm_acc_z,(float)icm_acc_x)*(180.0/PI));   //�����м��ٶȼƵõ��Ƕ�ֵ
}

//**************����Y��Ƕ�*************************//
/*
 * ��������ICM20689_GYRO_Y_Ang(void)
 * ����  ������Y��Ƕ�
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
 
void ICM20602_GYRO_Y_Ang()
{
  get_icm20602_gyro_simspi();   //��ȡY��������ֵ
  GYRO_Ang_Y +=((float)icm_gyro_y)*0.00032;  //���ֽǶ�
}

//***************���ٶȼƺ������������ں�*****************************//

void ICM20602_GYRO_ACCEL_Ang_Y(void)
{	
	static float temp_p = 0.0;
	float ACCEL_Ang_Y=0.0;
  ICM20602_GYRO_Y_Ang();
  ACCEL_Ang_Y=ICM20602_ACC_angle_X_Z();
  temp_p=(ACCEL_Ang_Y-GYRO_Ang_Y)*0.025;//�ں�ϵ��  //0.025
  GYRO_Ang_Y=GYRO_Ang_Y+temp_p;  
  ACC_GYRO_Ang_Y=GYRO_Ang_Y;
}

int16 GyroOffset_x,GyroOffset_y,GyroOffset_z;// ������У׼ֵ 
/**
 * @brief ��������Ư��ʼ��
 * ͨ���ɼ�һ���������ֵ�������������ƫ��ֵ��
 * ���� �����Ƕ�ȡ������ - ��Ʈֵ������ȥ�����ƫ������
 */
void gyroOffsetInit(void)     
{
		int16 i =0;
    GyroOffset_x = 0;
    GyroOffset_y = 0;
    GyroOffset_z = 0;
    for ( i = 0; i < 10; ++i) 
		{
        get_icm20602_gyro_simspi();    // ��ȡ�����ǽ��ٶ�
        GyroOffset_x += icm_gyro_x;
        GyroOffset_y += icm_gyro_y;
        GyroOffset_z += icm_gyro_z;
        delay_ms(5);    // ��� 1Khz
    }
    GyroOffset_x /= 100;
    GyroOffset_y /= 100;
    GyroOffset_z /= 100;
}

/**
 * @brief ���ɼ�����ֵת��Ϊʵ������ֵ, ���������ǽ���ȥ��Ư����
 * ���ٶȼƳ�ʼ������ -> ������Χ: ��8g        ��Ӧ������: 4096 LSB/g
 * �����ǳ�ʼ������   -> ������Χ: ��2000 dps  ��Ӧ������: 16.4 LSB/dps   (degree per second)
 * @tips: gyro = (gyro_val / 16.4) ��/s = ((gyro_val / 16.4) * PI / 180) rad/s
 */

//void icmGetValues(void) 
//{
//    float alpha = 0.451;//Ҫ��

//    //һ�׵�ͨ�˲�����λg
//    icm_data.acc_x = (((float) icm_acc_x) * alpha) / 4096 + icm_data.acc_x * (1 - alpha);
//    icm_data.acc_y = (((float) icm_acc_y) * alpha) / 4096 + icm_data.acc_y * (1 - alpha);
//    icm_data.acc_z = (((float) icm_acc_z) * alpha) / 4096 + icm_data.acc_z * (1 - alpha);

//    //! �����ǽ��ٶȱ���ת��Ϊ�����ƽ��ٶ�: deg/s -> rad/s
//    icm_data.gyro_x = ((float) icm_gyro_x - GyroOffset_x) * PI / 180 / 16.4f; 
//    icm_data.gyro_y = ((float) icm_gyro_y - GyroOffset_y) * PI / 180 / 16.4f;
//    icm_data.gyro_z = ((float) icm_gyro_z - GyroOffset_z) * PI / 180 / 16.4f;
//}

//#define delta_T     0.001f  // ��������1ms ��Ƶ��1KHZ

//float I_ex, I_ey, I_ez;  // ������
//quater_param_t Q_info = {1, 0, 0, 0};  // ��Ԫ����ʼ��
//euler_param_t eulerAngle;              // ŷ����
//Icm_data icm_data;               // ICM20602�ɼ���������ֵ

//float icm_kp= 0.17;    // ���ٶȼƵ��������ʱ�������
//float icm_ki= 0.004;   // �������������ʵĻ�������

///**
// * @brief �û����˲��㷨������������̬(�����ü��ٶȼ����������ǵĻ������)
// * ���ٶȼƶ���֮��������Ƚ����У��������ݼ��������̬���ţ������Ƕ������������У��������ݿ��ţ�������ʹ�û����������(�ڲ������㷨�Ŵ�̬���)��
// * ���ʹ����̬�����˲����������������ǣ��������ż��ٶȼơ�
// * @tips: n - ��������ϵ�� b - ��������ϵ
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
//	float q0 = Q_info.q0;  //��Ԫ��
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