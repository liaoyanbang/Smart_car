/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技STC16核心板
【编    写】chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2021年1月23日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【IDE】STC16
【Target 】 C251Keil5.6及以上
【SYS PLL】 30MHz使用内部晶振
=================================================================
STC16相关配套视频：
龙邱科技B站网址：https://space.bilibili.com/95313236
STC16环境下载参考视频： https://www.bilibili.com/video/BV1gy4y1p7T1/
STC16一体板子介绍视频： https://www.bilibili.com/video/BV1Jy4y1e7R4/
=================================================================
下载时, 选择时钟 30MHZ (用户可自行修改频率).
STC16F初次下载:先用IRCBND=0，ISP界面设置为24M，
然后IRCBND=0，下载频率为30M；
或者IRCBND=1，下载频率为30M；好用为准
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"

/*************  SPI设备相关变量声明   **************/
sbit    LQ_IMU_SCK = P2^5;                 //SCK
sbit    LQ_IMU_SO  = P2^4;                 //MISO
sbit    LQ_IMU_SI  = P2^3;                 //MOSI
sbit    LQ_IMU_CE  = P2^2;                 //CS

#define SPI_CE_High()   LQ_IMU_CE  = 1     // set CE high
#define SPI_CE_Low()    LQ_IMU_CE  = 0     // clear CE low
#define SPI_Hold()      P_SPI_Hold = 0     // clear Hold pin
#define SPI_UnHold()    P_SPI_Hold = 1     // set Hold pin
#define SPI_WP()        P_SPI_WP   = 0     // clear WP pin
#define SPI_UnWP()      P_SPI_WP   = 1     // set WP pin

/************************************************************************/
void SPI_init(void)
{
    SPCTL |=  (1 << 7);                    //忽略 SS 引脚功能，使用 MSTR 确定器件是主机还是从机
    SPCTL |=  (1 << 6);                    //使能 SPI 功能
    SPCTL &= ~(1 << 5);                    //先发送/接收数据的高位（ MSB）
    SPCTL |=  (1 << 4);                    //设置主机模式
    SPCTL &= ~(1 << 3);                    //SCLK 空闲时为低电平，SCLK 的前时钟沿为上升沿，后时钟沿为下降沿
    SPCTL &= ~(1 << 2);                    //数据 SS 管脚为低电平驱动第一位数据并在 SCLK 的后时钟沿改变数据
    SPCTL = (SPCTL & ~3) | 0;              //SPI 时钟频率选择, 0: 4T, 1: 8T,  2: 16T,  3: 32T
	  P_SW1 = (P_SW1 & ~(3<<2)) | (1<<2);    //IO口切换. 0: P1.2/P5.4 P1.3 P1.4 P1.5, 
											                     //1: P2.2 P2.3 P2.4 P2.5, 
											                     //2: P5.4 P4.0 P4.1 P4.3, 
											                     //3: P3.5 P3.4 P3.3 P3.2
                                           
    LQ_IMU_SCK = 0;                        // set clock to low initial state
    LQ_IMU_SI = 1;	  
    SPSTAT = SPIF + WCOL;                  //清0 SPIF和WCOL标志
}

/************************************************************************/
void SPI_WriteByte(u8 out)
{	 
    SPDAT = out;
    while((SPSTAT & SPIF) == 0) ;
    SPSTAT = SPIF + WCOL;                  //清0 SPIF和WCOL标志	
}

/************************************************************************/
u8 SPI_ReadByte(void)
{	 
    SPDAT = 0xff;
    while((SPSTAT & SPIF) == 0) ;
    SPSTAT = SPIF + WCOL;                  //清0 SPIF和WCOL标志
    return (SPDAT);
}

/************************************************
从Flash中读取数据
入口参数:
    addr   : 地址参数
    lqbuf : 缓冲从Flash中读取的数据
    lqsize   : 数据块大小
出口参数:
    无
************************************************/
void SPI_Read_Nbytes(u8 addr, u8 *lqbuf, u16 lqsize)
{
    SPI_CE_Low();                         //enable device
    SPI_WriteByte(addr);                  //设置起始地址
	  lqbuf++;                              
    do{                                   
        *lqbuf = SPI_ReadByte();          //receive byte and store at lqbuf
        lqbuf++;                          
    }while(--lqsize);                     //read until no_bytes is reached
    SPI_CE_High();                        //disable device
}

/************************************************
写数据到Flash中
入口参数:
    addr   : 地址参数
    lqbuf : 缓冲需要写入Flash的数据
    lqsizee   : 数据块大小
出口参数: 无
************************************************/
void SPI_Write_Nbytes(u8 addr, u8 *lqbuf, u8 lqsize)
{

    SPI_CE_Low();                         // enable device
    SPI_WriteByte(addr);        				  // 发送页编程命令
	  lqbuf++;                              
    do{                                   
        SPI_WriteByte(*lqbuf++);          //连续页内写
        addr++;                           
        if ((addr & 0xff) == 0) break;    
    }while(--lqsize);                     
    SPI_CE_High();                        // disable device
}