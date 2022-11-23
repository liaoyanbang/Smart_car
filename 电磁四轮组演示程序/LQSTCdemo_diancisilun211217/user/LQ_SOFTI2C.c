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
#include "LQ_SOFTI2C.h"
#include "include.h"
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������
����  �ܡ���ʱ����
������ֵ��ms /30M
������ֵ���� 
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void  delayms(unsigned int ms)
{
  unsigned int i;
  do{    
    i=MAIN_Fosc/6000;//��Լ1����
    while(--i);
  }while(--ms);
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void delayus(unsigned int us)
����  �ܡ���ʱ����
������ֵ��us /30M
������ֵ���� 
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void  delayus(unsigned int us)
{
  unsigned int i;
  do{    
    i=MAIN_Fosc/6000001;//��Լ1us
    while(--i);
  }while(--us);
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void SOFT_IIC_Init(void)
����  �ܡ���ʼ��I2C��Ӧ�Ľӿ����š�
������ֵ����
������ֵ���� 
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void SOFT_IIC_Init(void)
{			
  P2M1 &= 0xCF;
  P2M1 |= 0x00;
  P2M0 &= 0xCF;
  P2M0 |= 0x20;   //SCL����Ϊ���������SDA��Ϊ׼˫���
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������void SOFT_IIC_Start(void)
����  �ܡ�����IIC��ʼ�ź�
������ֵ����
������ֵ���� 
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int SOFT_IIC_Start(void)
{
  //SDA_OUT();     //sda�����
  SOFT_IIC_SDA=1;
  if(!READ_SDA)return 0;	
  SOFT_IIC_SCL=1;
  delayus(1);
  SOFT_IIC_SDA=0;	//START:when CLK is high,DATA change form high to low 
  delayus(2);	//T(hold,start) > 0.6us
  if(READ_SDA)return 0;	//��Ҫ�ŵ���ʱ����
  SOFT_IIC_SCL=0;	//ǯסI2C���ߣ�׼�����ͻ�������� 
  return 1;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void SOFT_IIC_Stop(void)
*��������:	    //����IICֹͣ�ź�
*******************************************************************************/	  
void SOFT_IIC_Stop(void)
{
  //SDA_OUT();//sda�����
  SOFT_IIC_SCL=0;
  delayus(1);
  SOFT_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
  delayus(1);
  SOFT_IIC_SCL=1; 
  delayus(1);
  SOFT_IIC_SDA=1;//����I2C���߽����ź�
  delayus(1);							   	
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char SOFT_IIC_Wait_Ack(void)
*��������:	    �ȴ�Ӧ���źŵ��� 
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
*******************************************************************************/
int SOFT_IIC_Wait_Ack(void)
{
  unsigned char ucErrTime=0;
  //SDA_IN();      //SDA����Ϊ����  
  //	delay_us(1);
  SOFT_IIC_SDA=1;		// ���ͷ�SDA��
  delayus(2);	   
  SOFT_IIC_SCL=1;
  delayus(1);	 
  while(READ_SDA)
  {
    ucErrTime++; 
    if(ucErrTime>50)
    {
      SOFT_IIC_Stop();
      return 0;
    }
    delayus(1);
  }
  SOFT_IIC_SCL=0;//ʱ�����0 	   
  return 1;  
} 

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void SOFT_IIC_Ack(void)
*��������:	    ����ACKӦ��
*******************************************************************************/
void SOFT_IIC_Ack(void)
{
  SOFT_IIC_SCL=0;
  //SDA_OUT();
  SOFT_IIC_SDA=0;
  delayus(1);
  SOFT_IIC_SCL=1;
  delayus(1);
  SOFT_IIC_SCL=0;
  SOFT_IIC_SDA=1;		//�ͷ�SDA��
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void SOFT_IIC_NAck(void)
*��������:	    ����NACKӦ��
*******************************************************************************/	    
void SOFT_IIC_NAck(void)
{
  SOFT_IIC_SCL=0;
  //SDA_OUT();
  delayus(1);
  SOFT_IIC_SDA=1;
  delayus(1);
  SOFT_IIC_SCL=1;
  delayus(2);
  SOFT_IIC_SCL=0;
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:		void SOFT_IIC_Send_Byte(unsigned char txd)
*��������:	    IIC����һ���ֽ�
*******************************************************************************/		  
void SOFT_IIC_Send_Byte(unsigned char txd)
{                        
  unsigned char t;   
  //SDA_OUT(); 	    
  SOFT_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
  delayus(1);
  for(t=0;t<8;t++)
  {            
    SOFT_IIC_SDA=(txd&0x80)>>7;
    delayus(1);   
    SOFT_IIC_SCL=1;
    txd<<=1; 	  
    delayus(2); 
    SOFT_IIC_SCL=0;	
    delayus(1);  
  }
  SOFT_IIC_SDA = 1;  // �����������ͷ�SDA
  delayus(1);
} 	 
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������bool i2cWrite(unsigned char addr, unsigned char reg, unsigned char dat)
����  �ܡ���addr�豸��reg��ַдlen�ֽ�����*dat��������DMP��ӿڣ�����һ��
������ֵ��unsigned char addr  �豸��ַ
������ֵ��unsigned char reg   Ҫд��ļĴ���
������ֵ��unsigned int  len   ���ݳ���
������ֵ��unsigned char *dat  ����ָ��
������ֵ��0���ɹ�   1��ʧ�� 
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int LQ_I2C_Write(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *dat)
{
  unsigned int data i = 0;		// �����ҲҪ��data
  unsigned int data length = len;
  if (!SOFT_IIC_Start())
    return 1;
  SOFT_IIC_Send_Byte(addr << 1);
  if (!SOFT_IIC_Wait_Ack()) 
  {
    SOFT_IIC_Stop();
    return 2;
  }
  SOFT_IIC_Send_Byte(reg);
  SOFT_IIC_Wait_Ack();
  for (i = 0; i < length; i++) 
  {
    SOFT_IIC_Send_Byte(dat[i]);
    if (!SOFT_IIC_Wait_Ack()) {
      SOFT_IIC_Stop();
      return 3;
    }
    else
      ;
  }
  SOFT_IIC_Stop();
  return 0;
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
����������int LQ_I2C_Read(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *buf)
����  �ܡ���addr�豸��reg��ַȡlen�ֽ����ݴ����*buf��������DMP��ӿڣ�����һ��
������ֵ��unsigned char addr  �豸��ַ
������ֵ��unsigned char reg   Ҫд��ļĴ���
������ֵ��unsigned int  len   ���ݳ���
������ֵ��unsigned char *buf  ����ָ��
������ֵ��0���ɹ�   1��ʧ��  
����  �ߡ�chiusir
�������¡�2021��1��22�� 
������汾��V1.0
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
int LQ_I2C_Read(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *buf)
{
  unsigned int data length = len;
  if (SOFT_IIC_Start() == 0)
    return 1;
  SOFT_IIC_Send_Byte((unsigned char)(addr << 1));
  if (SOFT_IIC_Wait_Ack() == 0) 
  {
    SOFT_IIC_Stop();
    return 2;
  }
  SOFT_IIC_Send_Byte(reg);
  SOFT_IIC_Wait_Ack();
  delayus(2);					//Ӧ����Ҫһ����ʱ��
  while(SOFT_IIC_Start() == 0);
  SOFT_IIC_Send_Byte((unsigned char)((addr << 1)+1));
  SOFT_IIC_Wait_Ack();
  while (length) 
  {
    if (length == 1)
      *buf = SOFT_IIC_Read_Byte(0);
    else
      *buf = SOFT_IIC_Read_Byte(1);
    buf++;
    length--;
  }
  SOFT_IIC_Stop();
  return 0;
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char SOFT_IIC_Read_Byte(unsigned char ack)
*��������:	    //��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK 
*******************************************************************************/  
unsigned char SOFT_IIC_Read_Byte(unsigned char ack)
{
  unsigned char i,receive=0;
  //SDA_IN();//SDA����Ϊ����
  for(i=0;i<8;i++ )
  {
    SOFT_IIC_SCL=0; 
    delayus(2);
    SOFT_IIC_SCL=1;
    receive<<=1;
    if(READ_SDA)receive++;   
    delayus(2); 
  }					 
  if(ack)
    SOFT_IIC_Ack(); //����ACK 
  else
    SOFT_IIC_NAck();//����nACK  
  return receive;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
*��������:	    ��ȡָ���豸 ָ���Ĵ�����һ��ֵ
����	I2C_Addr  Ŀ���豸��ַ
addr	   �Ĵ�����ַ
����   ��������ֵ
*******************************************************************************/ 
unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
{
  unsigned char res=0;
  
  SOFT_IIC_Start();	
  SOFT_IIC_Send_Byte(I2C_Addr);	   //����д����
  res++;
  SOFT_IIC_Wait_Ack();
  SOFT_IIC_Send_Byte(addr); res++;  //���͵�ַ
  SOFT_IIC_Wait_Ack();	  
  //SOFT_IIC_Stop();//����һ��ֹͣ����	
  SOFT_IIC_Start();
  SOFT_IIC_Send_Byte((unsigned char)(I2C_Addr+1)); res++;          //�������ģʽ			   
  SOFT_IIC_Wait_Ack();
  res=SOFT_IIC_Read_Byte(0);	   
  SOFT_IIC_Stop();//����һ��ֹͣ����
  
  return res;
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char IICreadBytes(unsigned char dev, unsigned char reg, unsigned char length, unsigned char *dat)
*��������:	    ��ȡָ���豸 ָ���Ĵ����� length��ֵ
����	dev  Ŀ���豸��ַ
reg	  �Ĵ�����ַ
length Ҫ�����ֽ���
*dat  ���������ݽ�Ҫ��ŵ�ָ��
����   ���������ֽ�����
*******************************************************************************/ 
unsigned char IICreadBytes(unsigned char dev, unsigned char reg, unsigned char length, unsigned char *dat)
{
  unsigned char count = 0;
  
  SOFT_IIC_Start();
  SOFT_IIC_Send_Byte(dev);	   //����д����
  SOFT_IIC_Wait_Ack();
  SOFT_IIC_Send_Byte(reg);   //���͵�ַ
  SOFT_IIC_Wait_Ack();	  
  SOFT_IIC_Start();
  SOFT_IIC_Send_Byte((unsigned char)(dev+1));  //�������ģʽ	
  SOFT_IIC_Wait_Ack();
  
  for(count=0;count<length;count++){
    
    if(count!=length-1)dat[count]=SOFT_IIC_Read_Byte(1);  //��ACK�Ķ�����
    else  dat[count]=SOFT_IIC_Read_Byte(0);	 //���һ���ֽ�NACK
  }
  SOFT_IIC_Stop();//����һ��ֹͣ����
  return count;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char IICwriteBytes(unsigned char dev, unsigned char reg, unsigned char length, unsigned char* dat)
*��������:	    ������ֽ�д��ָ���豸 ָ���Ĵ���
����	dev  Ŀ���豸��ַ
reg	  �Ĵ�����ַ
length Ҫд���ֽ���
*dat  ��Ҫд�����ݵ��׵�ַ
����   �����Ƿ�ɹ�
*******************************************************************************/ 
unsigned char IICwriteBytes(unsigned char dev, unsigned char reg, unsigned char length, unsigned char* dat){
  
  unsigned char count = 0;
  SOFT_IIC_Start();
  SOFT_IIC_Send_Byte(dev);	   //����д����
  SOFT_IIC_Wait_Ack();
  SOFT_IIC_Send_Byte(reg);   //���͵�ַ
  SOFT_IIC_Wait_Ack();	  
  for(count=0;count<length;count++){
    SOFT_IIC_Send_Byte(dat[count]); 
    SOFT_IIC_Wait_Ack(); 
  }
  SOFT_IIC_Stop();//����һ��ֹͣ����
  
  return 1; //status == 0;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char IICreadByte(unsigned char dev, unsigned char reg, unsigned char *dat)
*��������:	    ��ȡָ���豸 ָ���Ĵ�����һ��ֵ
����	dev  Ŀ���豸��ַ
reg	   �Ĵ�����ַ
*dat  ���������ݽ�Ҫ��ŵĵ�ַ
����   1
*******************************************************************************/ 
unsigned char IICreadByte(unsigned char dev, unsigned char reg, unsigned char *dat)
{
  *dat=I2C_ReadOneByte(dev, reg);
  return 1;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char dat)
*��������:	    д��ָ���豸 ָ���Ĵ���һ���ֽ�
����	dev  Ŀ���豸��ַ
reg	   �Ĵ�����ַ
dat  ��Ҫд����ֽ�
����   1
*******************************************************************************/ 
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char dat)
{
  return IICwriteBytes(dev, reg, 1, &dat);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char IICwriteBits(unsigned char dev,unsigned char reg,unsigned char bitStart,unsigned char length,unsigned char dat)
*��������:	    �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �еĶ��λ
����	dev  Ŀ���豸��ַ
reg	   �Ĵ�����ַ
bitStart  Ŀ���ֽڵ���ʼλ
length   λ����
dat    ��Ÿı�Ŀ���ֽ�λ��ֵ
����   �ɹ� Ϊ1 
ʧ��Ϊ0
*******************************************************************************/ 
unsigned char IICwriteBits(unsigned char dev,unsigned char reg,unsigned char bitStart,unsigned char length,unsigned char dat)
{
  
  unsigned char b;
  if (IICreadByte(dev, reg, &b) != 0) {
    unsigned char mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
    dat <<= (8 - length);
    dat >>= (7 - bitStart);
    b &= mask;
    b |= dat;
    return IICwriteByte(dev, reg, b);
  } else {
    return 0;
  }
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char IICwriteBit(unsigned char dev, unsigned char reg, unsigned char bitNum, unsigned char dat)
*��������:	    �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �е�1��λ
����	dev  Ŀ���豸��ַ
reg	   �Ĵ�����ַ
bitNum  Ҫ�޸�Ŀ���ֽڵ�bitNumλ
dat  Ϊ0 ʱ��Ŀ��λ������0 ���򽫱���λ
����   �ɹ� Ϊ1 
ʧ��Ϊ0
*******************************************************************************/ 
unsigned char IICwriteBit(unsigned char dev, unsigned char reg, unsigned char bitNum, unsigned char dat)
{
  unsigned char b;
  IICreadByte(dev, reg, &b);
  b = (dat != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
  return IICwriteByte(dev, reg, b);
}

//------------------End of File----------------------------
