/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技STC16核心板
【修    改】chiusir
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
#ifndef __STC16F_H__
#define __STC16F_H__

/////////////////////////////////////////////////
//SFR:80H-SFR:FFH
/////////////////////////////////////////////////

sfr P0              =   0x80;
sbit P00            =   P0^0;
sbit P01            =   P0^1;
sbit P02            =   P0^2;
sbit P03            =   P0^3;
sbit P04            =   P0^4;
sbit P05            =   P0^5;
sbit P06            =   P0^6;
sbit P07            =   P0^7;
sfr SP              =   0x81;
sfr DPL             =   0x82;
sfr DPH             =   0x83;
sfr DPXL            =   0x84;
sfr USBCLK          =   0x85;
sfr DPS             =   0x86;
sfr PCON            =   0x87;
//sbit IDL            =   PCON^0;
//sbit PD             =   PCON^1;
//sbit GF0            =   PCON^2;
//sbit GF1            =   PCON^3;
//sbit POF            =   PCON^4;
//sbit LVDF           =   PCON^5;
//sbit SMOD0          =   PCON^6;
//sbit SMOD           =   PCON^7;
sfr TCON            =   0x88;
sbit IT0            =   TCON^0;
sbit IE0            =   TCON^1;
sbit IT1            =   TCON^2;
sbit IE1            =   TCON^3;
sbit TR0            =   TCON^4;
sbit TF0            =   TCON^5;
sbit TR1            =   TCON^6;
sbit TF1            =   TCON^7;
sfr TMOD            =   0x89;
sfr TL0             =   0x8a;
sfr TL1             =   0x8b;
sfr TH0             =   0x8c;
sfr TH1             =   0x8d;
sfr CKCON           =   0x8e;
sfr DMAIR           =   0x8f;
sfr P1              =   0x90;
sbit P10            =   P1^0;
sbit P11            =   P1^1;
sbit P12            =   P1^2;
sbit P13            =   P1^3;
sbit P14            =   P1^4;
sbit P15            =   P1^5;
sbit P16            =   P1^6;
sbit P17            =   P1^7;
sfr USBCON          =   0x91;
sfr WTST            =   0x92;
sfr AUXR            =   0x93;
//sbit S2BRS          =   AUXR^0;
//sbit EXRAM          =   AUXR^1;
//sbit T2x12          =   AUXR^2;
//sbit T2CT           =   AUXR^3;
//sbit TR2            =   AUXR^4;
//sbit S1M0x6         =   AUXR^5;
//sbit T1x12          =   AUXR^6;
//sbit T0x12          =   AUXR^7;
sfr AUXR2           =   0x94;
#define P40ADR    0x40
#define ADIBF     0x20
#define TXLINRX   0x10
#define USBEN     0x04
#define CANEN     0x02
#define LINEN     0x01
sfr INTCLKO         =   0x95;
//sbit T0CLKO         =   INTCLKO^0;
//sbit T1CLKO         =   INTCLKO^1;
//sbit T2CLKO         =   INTCLKO^2;
//sbit EX2            =   INTCLKO^4;
//sbit EX3            =   INTCLKO^5;
//sbit EX4            =   INTCLKO^6;
sfr USBADR          =   0x96;
sbit USBBUSY        =   USBADR^7;
sfr USBDAT          =   0x97;
sfr SCON            =   0x98;
sbit RI             =   SCON^0;
sbit TI             =   SCON^1;
sbit RB8            =   SCON^2;
sbit TB8            =   SCON^3;
sbit REN            =   SCON^4;
sbit SM2            =   SCON^5;
sbit SM1            =   SCON^6;
sbit SM0            =   SCON^7;
sfr SBUF            =   0x99;
sfr P_SW1           =   0x9a;
sfr P_SW2           =   0x9b;
sfr BGTRIM          =   0x9c;
sfr VRTRIM          =   0x9d;
sfr LIRTRIM         =   0x9e;
sfr IRTRIM          =   0x9f;
sfr P2              =   0xa0;
sbit P20            =   P2^0;
sbit P21            =   P2^1;
sbit P22            =   P2^2;
sbit P23            =   P2^3;
sbit P24            =   P2^4;
sbit P25            =   P2^5;
sbit P26            =   P2^6;
sbit P27            =   P2^7;
sfr CANICR          =   0xa1;
sfr CANAR           =   0xa2;
sfr CANDR           =   0xa3;
sfr MACDR0          =   0xa4;
sfr MACDR1          =   0xa5;
sfr MACDR2          =   0xa6;
sfr MACDR3          =   0xa7;
sfr IE              =   0xa8;
sbit EX0            =   IE^0;
sbit ET0            =   IE^1;
sbit EX1            =   IE^2;
sbit ET1            =   IE^3;
sbit ES             =   IE^4;
sbit EADC           =   IE^5;
sbit ELVD           =   IE^6;
sbit EA             =   IE^7;
sfr IRCBAND         =   0xa9;
sfr WKTCL           =   0xaa;
sfr WKTCH           =   0xab;
sfr VOCTRL          =   0xac;
sfr VOSEL           =   0xad;
sfr MACADDR         =   0xae;
sfr MACDATA         =   0xaf;
sfr P3              =   0xb0;
sbit P30            =   P3^0;
sbit P31            =   P3^1;
sbit P32            =   P3^2;
sbit P33            =   P3^3;
sbit P34            =   P3^4;
sbit P35            =   P3^5;
sbit P36            =   P3^6;
sbit P37            =   P3^7;
sfr LINICR          =   0xb1;
sfr LINAR           =   0xb2;
sfr LINDR           =   0xb3;
sfr CMPCR1          =   0xb4;
//sbit CMPRES         =   CMPCR1^0;
//sbit CMPOE          =   CMPCR1^1;
//sbit CMPNIS         =   CMPCR1^2;
//sbit CMPPIS         =   CMPCR1^3;
//sbit CMPNIE         =   CMPCR1^4;
//sbit CMPPIE         =   CMPCR1^5;
//sbit CMPIF          =   CMPCR1^6;
//sbit CMPEN          =   CMPCR1^7;
sfr CMPCR2          =   0xb5;
//sbit DISFLT         =   CMPCR2^6;
//sbit INVCMPO        =   CMPCR2^7;
sfr IP2H            =   0xb6;
//sbit PS2H           =   IP2H^0;
//sbit PSPIH          =   IP2H^1;
//sbit PUSBH          =   IP2H^2;
//sbit PPWM2H         =   IP2H^3;
//sbit PX4H           =   IP2H^4;
//sbit PCMPH          =   IP2H^5;
//sbit PSSBH          =   IP2H^6;
sfr IPH             =   0xb7;
//sbit PX0H           =   IPH^0;
//sbit PT0H           =   IPH^1;
//sbit PX1H           =   IPH^2;
//sbit PT1H           =   IPH^3;
//sbit PS0H           =   IPH^4;
//sbit PSH            =   IPH^4;
//sbit PADCH          =   IPH^5;
//sbit PLVDH          =   IPH^6;
//sbit PPWM1H         =   IPH^7;
sfr IP              =   0xb8;
sbit PX0            =   IP^0;
sbit PT0            =   IP^1;
sbit PX1            =   IP^2;
sbit PT1            =   IP^3;
sbit PS0            =   IP^4;
sbit PS             =   IP^4;
sbit PADC           =   IP^5;
sbit PLVD           =   IP^6;
sbit PPWM1          =   IP^7;
sfr SPSTAT          =   0xb9;
//sbit WCOL           =   SPSTAT^6;
//sbit SPIF           =   SPSTAT^7;
#define SPIF    0x80        //SPI传输完成标志。写入1清0。
#define WCOL    0x40        //SPI写冲突标志。写入1清0。
sfr SPCTL           =   0xba;
//sbit CPHA           =   SPCTL^2;
//sbit CPOL           =   SPCTL^3;
//sbit MSTR           =   SPCTL^4;
//sbit DORD           =   SPCTL^5;
//sbit SPEN           =   SPCTL^6;
//sbit SSIG           =   SPCTL^7;
sfr SPDAT           =   0xbb;
sfr S4CON           =   0xbc;
//sbit S4RI           =   S4CON^0;
//sbit S4TI           =   S4CON^1;
//sbit S4RB8          =   S4CON^2;
//sbit S4TB8          =   S4CON^3;
//sbit S4REN          =   S4CON^4;
//sbit S4SM2          =   S4CON^5;
//sbit S4SM1          =   S4CON^6;
//sbit S4SM0          =   S4CON^7;
sfr S4BUF           =   0xbd;
sfr SPH             =   0xbe;
sfr BUS_SPEED       =   0xbf;
sfr P4              =   0xc0;
sbit P40            =   P4^0;
sbit P41            =   P4^1;
sbit P42            =   P4^2;
sbit P43            =   P4^3;
sbit P44            =   P4^4;
sbit P45            =   P4^5;
sbit P46            =   P4^6;
sbit P47            =   P4^7;
sfr DID             =   0xc1;
sfr IAP_DATA        =   0xc2;
sfr IAP_ADDRH       =   0xc3;
sfr IAP_ADDRL       =   0xc4;
sfr IAP_CMD         =   0xc5;
sfr IAP_TRIG        =   0xc6;
sfr IAP_CONTR       =   0xc7;
sfr P5              =   0xc8;
sbit P50            =   P5^0;
sbit P51            =   P5^1;
sbit P52            =   P5^2;
sbit P53            =   P5^3;
sbit P54            =   P5^4;
sbit P55            =   P5^5;
sbit P56            =   P5^6;
sbit P57            =   P5^7;
sfr T4H             =   0xc9;
sfr T4L             =   0xca;
sfr T3H             =   0xcb;
sfr T3L             =   0xcc;
sfr T2H             =   0xcd;
sfr T2L             =   0xce;
sfr DEVICE          =   0xcf;
sfr PSW             =   0xd0;
sbit P              =   PSW^0;
sbit F1             =   PSW^1;
sbit OV             =   PSW^2;
sbit RS0            =   PSW^3;
sbit RS1            =   PSW^4;
sbit F0             =   PSW^5;
sbit AC             =   PSW^6;
sbit CY             =   PSW^7;
sfr PSW1            =   0xd1;
//sbit Z              =   PSW1^1;
//sbit N              =   PSW1^5;
sfr P7M1            =   0xd2;
sfr P7M0            =   0xd3;
sfr RTCCON          =   0xd4;
sfr RTCSEC          =   0xd5;
sfr RTCREG          =   0xd6;
sfr RTCICR          =   0xd7;
sfr RSTIF           =   0xd8;
sfr ICECR           =   0xd9;
//sbit ICEPIF         =   ICECR^0;
//sbit BRKIF          =   ICECR^1;
//sbit EICEPI         =   ICECR^4;
//sbit EBRKI          =   ICECR^5;
//sbit REITS          =   ICECR^6;
//sbit ICEON          =   ICECR^7;
sfr AUXINTIF        =   0xda;
//sbit T2IF           =   AUXINTIF^0;
//sbit T3IF           =   AUXINTIF^1;
//sbit T4IF           =   AUXINTIF^2;
//sbit INT2IF         =   AUXINTIF^4;
//sbit INT3IF         =   AUXINTIF^5;
//sbit INT4IF         =   AUXINTIF^6;
sfr T4T3M           =   0xdb;
//sbit T3CLKO         =   T4T3M^0;
//sbit T3x12          =   T4T3M^1;
//sbit T3CT           =   T4T3M^2;
//sbit T3R            =   T4T3M^3;
//sbit T4CLKO         =   T4T3M^4;
//sbit T4x12          =   T4T3M^5;
//sbit T4CT           =   T4T3M^6;
//sbit T4R            =   T4T3M^7;
sfr ADC_CONTR       =   0xdc;
//sbit ADC_FLAG       =   ADC_CONTR^5;
//sbit ADC_START      =   ADC_CONTR^6;
//sbit ADC_POWER      =   ADC_CONTR^7;
sfr ADC_RES         =   0xdd;
sfr ADC_RESL        =   0xde;
sfr ADCCFG          =   0xdf;
sfr ACC             =   0xe0;
sfr SADDR           =   0xe1;
sfr SADEN           =   0xe2;
sfr S2CON           =   0xe3;
//sbit S2RI           =   S2CON^0;
//sbit S2TI           =   S2CON^1;
//sbit S2RB8          =   S2CON^2;
//sbit S2TB8          =   S2CON^3;
//sbit S2REN          =   S2CON^4;
//sbit S2SM2          =   S2CON^5;
//sbit S2SM0          =   S2CON^7;
sfr S2BUF           =   0xe4;
sfr S3CON           =   0xe5;
//sbit S3RI           =   S3CON^0;
//sbit S3TI           =   S3CON^1;
//sbit S3RB8          =   S3CON^2;
//sbit S3TB8          =   S3CON^3;
//sbit S3REN          =   S3CON^4;
//sbit S3SM2          =   S3CON^5;
//sbit S3SM1          =   S3CON^6;
//sbit S3SM0          =   S3CON^7;
sfr S3BUF           =   0xe6;
sfr IE2             =   0xe7;
sbit ES2            =   IE2^0;
sbit ESPI           =   IE2^1;
sbit ET2            =   IE2^2;
sbit ES3            =   IE2^3;
sbit ES4            =   IE2^4;
sbit ET3            =   IE2^5;
sbit ET4            =   IE2^6;
sfr P6              =   0xe8;
sbit P60            =   P6^0;
sbit P61            =   P6^1;
sbit P62            =   P6^2;
sbit P63            =   P6^3;
sbit P64            =   P6^4;
sbit P65            =   P6^5;
sbit P66            =   P6^6;
sbit P67            =   P6^7;
sfr STATUS          =   0xe9;
sfr MXAX            =   0xea;
sfr TA              =   0xeb;
sfr P5M1            =   0xec;
sfr P5M0            =   0xed;
sfr P6M1            =   0xee;
sfr P6M0            =   0xef;
sfr B               =   0xf0;
sfr P0M1            =   0xf1;
sfr P0M0            =   0xf2;
sfr P1M1            =   0xf3;
sfr P1M0            =   0xf4;
sfr P2M1            =   0xf5;
sfr P2M0            =   0xf6;
sfr IAP_TPS         =   0xf7;
sfr P7              =   0xf8;
sbit P70            =   P7^0;
sbit P71            =   P7^1;
sbit P72            =   P7^2;
sbit P73            =   P7^3;
sbit P74            =   P7^4;
sbit P75            =   P7^5;
sbit P76            =   P7^6;
sbit P77            =   P7^7;
sfr P3M1            =   0xf9;
sfr P3M0            =   0xfa;
sfr P4M1            =   0xfb;
sfr P4M0            =   0xfc;
sfr WDT_CONTR       =   0xfd;
sfr IP2             =   0xfe;
//sbit PS2            =   IP2^0;
//sbit PSPI           =   IP2^1;
//sbit PUSB           =   IP2^2;
//sbit PPWM2          =   IP2^3;
//sbit PX4            =   IP2^4;
//sbit PCMP           =   IP2^5;
//sbit PSSB           =   IP2^6;
sfr RSTCFG          =   0xff;
//sbit P54RST         =   RSTCFG^4;
//sbit ENLVR          =   RSTCFG^6;

/////////////////////////////////////////////////
//7E:FF00H-7E:FFFFH
/////////////////////////////////////////////////



/////////////////////////////////////////////////
//7E:FE00H-7E:FEFFH
/////////////////////////////////////////////////

#define CLKSEL              (*(unsigned char volatile far *)0x7efe00)
#define CLKDIV              (*(unsigned char volatile far *)0x7efe01)
#define IRC24MCR            (*(unsigned char volatile far *)0x7efe02)
#define XOSCCR              (*(unsigned char volatile far *)0x7efe03)
#define IRC32KCR            (*(unsigned char volatile far *)0x7efe04)
#define PLLCR               (*(unsigned char volatile far *)0x7efe05)
#define USBCON1             (*(unsigned char volatile far *)0x7efe06)
#define MCLKOCR             (*(unsigned char volatile far *)0x7efe07)
#define IRC48MCR            (*(unsigned char volatile far *)0x7efe08)
#define IRC48ATRIM          (*(unsigned char volatile far *)0x7efe09)
#define IRC48BTRIM          (*(unsigned char volatile far *)0x7efe0a)
#define IRCDB               (*(unsigned char volatile far *)0x7efe0b)

#define P0PU                (*(unsigned char volatile far *)0x7efe10)
#define P1PU                (*(unsigned char volatile far *)0x7efe11)
#define P2PU                (*(unsigned char volatile far *)0x7efe12)
#define P3PU                (*(unsigned char volatile far *)0x7efe13)
#define P4PU                (*(unsigned char volatile far *)0x7efe14)
#define P5PU                (*(unsigned char volatile far *)0x7efe15)
#define P6PU                (*(unsigned char volatile far *)0x7efe16)
#define P7PU                (*(unsigned char volatile far *)0x7efe17)
#define P0NCS               (*(unsigned char volatile far *)0x7efe18)
#define P1NCS               (*(unsigned char volatile far *)0x7efe19)
#define P2NCS               (*(unsigned char volatile far *)0x7efe1a)
#define P3NCS               (*(unsigned char volatile far *)0x7efe1b)
#define P4NCS               (*(unsigned char volatile far *)0x7efe1c)
#define P5NCS               (*(unsigned char volatile far *)0x7efe1d)
#define P6NCS               (*(unsigned char volatile far *)0x7efe1e)
#define P7NCS               (*(unsigned char volatile far *)0x7efe1f)
#define P0SR                (*(unsigned char volatile far *)0x7efe20)
#define P1SR                (*(unsigned char volatile far *)0x7efe21)
#define P2SR                (*(unsigned char volatile far *)0x7efe22)
#define P3SR                (*(unsigned char volatile far *)0x7efe23)
#define P4SR                (*(unsigned char volatile far *)0x7efe24)
#define P5SR                (*(unsigned char volatile far *)0x7efe25)
#define P6SR                (*(unsigned char volatile far *)0x7efe26)
#define P7SR                (*(unsigned char volatile far *)0x7efe27)
#define P0DR                (*(unsigned char volatile far *)0x7efe28)
#define P1DR                (*(unsigned char volatile far *)0x7efe29)
#define P2DR                (*(unsigned char volatile far *)0x7efe2a)
#define P3DR                (*(unsigned char volatile far *)0x7efe2b)
#define P4DR                (*(unsigned char volatile far *)0x7efe2c)
#define P5DR                (*(unsigned char volatile far *)0x7efe2d)
#define P6DR                (*(unsigned char volatile far *)0x7efe2e)
#define P7DR                (*(unsigned char volatile far *)0x7efe2f)
#define P0IE                (*(unsigned char volatile far *)0x7efe30)
#define P1IE                (*(unsigned char volatile far *)0x7efe31)

#define I2CCFG              (*(unsigned char volatile far *)0x7efe80)
#define ENI2C               0x80
#define I2CMASTER           0x40
#define I2CSLAVE            0x00
#define I2CMSCR             (*(unsigned char volatile far *)0x7efe81)
#define EMSI                0x80
#define MS_IDLE                     0x00
#define MS_START                    0x01
#define MS_SENDDAT                  0x02
#define MS_RECVACK                  0x03
#define MS_RECVDAT                  0x04
#define MS_SENDACK                  0x05
#define MS_STOP                     0x06
#define MS_START_SENDDAT_RECVACK    0x09
#define MS_SENDDAT_RECVACK          0x0a
#define MS_RECVDAT_SENDACK          0x0b
#define MS_RECVDAT_SENDNAK          0x0c
#define I2CMSST             (*(unsigned char volatile far *)0x7efe82)
#define MSBUSY              0x80
#define MSIF                0x40
#define MSACKI              0x02
#define MSACKO              0x01
#define I2CSLCR             (*(unsigned char volatile far *)0x7efe83)
#define ESTAI               0x40
#define ERXI                0x20
#define ETXI                0x10
#define ESTOI               0x08
#define SLRST               0x01
#define I2CSLST             (*(unsigned char volatile far *)0x7efe84)
#define SLBUSY              0x80
#define STAIF               0x40
#define RXIF                0x20
#define TXIF                0x10
#define STOIF               0x08
#define TXING               0x04
#define SLACKI              0x02
#define SLACKO              0x01
#define I2CSLADR            (*(unsigned char volatile far *)0x7efe85)
#define I2CTXD              (*(unsigned char volatile far *)0x7efe86)
#define I2CRXD              (*(unsigned char volatile far *)0x7efe87)
#define I2CMSAUX            (*(unsigned char volatile far *)0x7efe88)

#define TIMER2PS            (*(unsigned char volatile far *)0x7efea2)
#define TIMER3PS            (*(unsigned char volatile far *)0x7efea3)
#define TIMER4PS            (*(unsigned char volatile far *)0x7efea4)

#define ADCTIM              (*(unsigned char volatile far *)0x7efeab)
#define T3T4PS              (*(unsigned char volatile far *)0x7efeac)

#define PWM1_ETRPS          (*(unsigned char volatile far *)0x7efeb0)
#define PWM1_ENO            (*(unsigned char volatile far *)0x7efeb1)
#define PWM1_PS             (*(unsigned char volatile far *)0x7efeb2)
#define PWM1_IOAUX          (*(unsigned char volatile far *)0x7efeb3)
#define PWM2_ETRPS          (*(unsigned char volatile far *)0x7efeb4)
#define PWM2_ENO            (*(unsigned char volatile far *)0x7efeb5)
#define PWM2_PS             (*(unsigned char volatile far *)0x7efeb6)
#define PWM2_IOAUX          (*(unsigned char volatile far *)0x7efeb7)

#define PWMA_ETRPS          (*(unsigned char volatile far *)0x7efeb0)
#define PWMA_ENO            (*(unsigned char volatile far *)0x7efeb1)
#define PWMA_PS             (*(unsigned char volatile far *)0x7efeb2)
#define PWMA_IOAUX          (*(unsigned char volatile far *)0x7efeb3)
#define PWMB_ETRPS          (*(unsigned char volatile far *)0x7efeb4)
#define PWMB_ENO            (*(unsigned char volatile far *)0x7efeb5)
#define PWMB_PS             (*(unsigned char volatile far *)0x7efeb6)
#define PWMB_IOAUX          (*(unsigned char volatile far *)0x7efeb7)

#define PWM1_CR1            (*(unsigned char volatile far *)0x7efec0)
#define PWM1_CR2            (*(unsigned char volatile far *)0x7efec1)
#define PWM1_SMCR           (*(unsigned char volatile far *)0x7efec2)
#define PWM1_ETR            (*(unsigned char volatile far *)0x7efec3)
#define PWM1_IER            (*(unsigned char volatile far *)0x7efec4)
#define PWM1_SR1            (*(unsigned char volatile far *)0x7efec5)
#define PWM1_SR2            (*(unsigned char volatile far *)0x7efec6)
#define PWM1_EGR            (*(unsigned char volatile far *)0x7efec7)
#define PWM1_CCMR1          (*(unsigned char volatile far *)0x7efec8)
#define PWM1_CCMR2          (*(unsigned char volatile far *)0x7efec9)
#define PWM1_CCMR3          (*(unsigned char volatile far *)0x7efeca)
#define PWM1_CCMR4          (*(unsigned char volatile far *)0x7efecb)
#define PWM1_CCER1          (*(unsigned char volatile far *)0x7efecc)
#define PWM1_CCER2          (*(unsigned char volatile far *)0x7efecd)
#define PWM1_CNTRH          (*(unsigned char volatile far *)0x7efece)
#define PWM1_CNTRL          (*(unsigned char volatile far *)0x7efecf)
#define PWM1_PSCRH          (*(unsigned char volatile far *)0x7efed0)
#define PWM1_PSCRL          (*(unsigned char volatile far *)0x7efed1)
#define PWM1_ARRH           (*(unsigned char volatile far *)0x7efed2)
#define PWM1_ARRL           (*(unsigned char volatile far *)0x7efed3)
#define PWM1_RCR            (*(unsigned char volatile far *)0x7efed4)
#define PWM1_CCR1H          (*(unsigned char volatile far *)0x7efed5)
#define PWM1_CCR1L          (*(unsigned char volatile far *)0x7efed6)
#define PWM1_CCR2H          (*(unsigned char volatile far *)0x7efed7)
#define PWM1_CCR2L          (*(unsigned char volatile far *)0x7efed8)
#define PWM1_CCR3H          (*(unsigned char volatile far *)0x7efed9)
#define PWM1_CCR3L          (*(unsigned char volatile far *)0x7efeda)
#define PWM1_CCR4H          (*(unsigned char volatile far *)0x7efedb)
#define PWM1_CCR4L          (*(unsigned char volatile far *)0x7efedc)
#define PWM1_BKR            (*(unsigned char volatile far *)0x7efedd)
#define PWM1_DTR            (*(unsigned char volatile far *)0x7efede)
#define PWM1_OISR           (*(unsigned char volatile far *)0x7efedf)

#define PWM2_CR1            (*(unsigned char volatile far *)0x7efee0)
#define PWM2_CR2            (*(unsigned char volatile far *)0x7efee1)
#define PWM2_SMCR           (*(unsigned char volatile far *)0x7efee2)
#define PWM2_ETR            (*(unsigned char volatile far *)0x7efee3)
#define PWM2_IER            (*(unsigned char volatile far *)0x7efee4)
#define PWM2_SR1            (*(unsigned char volatile far *)0x7efee5)
#define PWM2_SR2            (*(unsigned char volatile far *)0x7efee6)
#define PWM2_EGR            (*(unsigned char volatile far *)0x7efee7)
#define PWM2_CCMR1          (*(unsigned char volatile far *)0x7efee8)
#define PWM2_CCMR2          (*(unsigned char volatile far *)0x7efee9)
#define PWM2_CCMR3          (*(unsigned char volatile far *)0x7efeea)
#define PWM2_CCMR4          (*(unsigned char volatile far *)0x7efeeb)
#define PWM2_CCER1          (*(unsigned char volatile far *)0x7efeec)
#define PWM2_CCER2          (*(unsigned char volatile far *)0x7efeed)
#define PWM2_CNTRH          (*(unsigned char volatile far *)0x7efeee)
#define PWM2_CNTRL          (*(unsigned char volatile far *)0x7efeef)
#define PWM2_PSCRH          (*(unsigned char volatile far *)0x7efef0)
#define PWM2_PSCRL          (*(unsigned char volatile far *)0x7efef1)
#define PWM2_ARRH           (*(unsigned char volatile far *)0x7efef2)
#define PWM2_ARRL           (*(unsigned char volatile far *)0x7efef3)
#define PWM2_RCR            (*(unsigned char volatile far *)0x7efef4)
#define PWM2_CCR1H          (*(unsigned char volatile far *)0x7efef5)
#define PWM2_CCR1L          (*(unsigned char volatile far *)0x7efef6)
#define PWM2_CCR2H          (*(unsigned char volatile far *)0x7efef7)
#define PWM2_CCR2L          (*(unsigned char volatile far *)0x7efef8)
#define PWM2_CCR3H          (*(unsigned char volatile far *)0x7efef9)
#define PWM2_CCR3L          (*(unsigned char volatile far *)0x7efefa)
#define PWM2_CCR4H          (*(unsigned char volatile far *)0x7efefb)
#define PWM2_CCR4L          (*(unsigned char volatile far *)0x7efefc)
#define PWM2_BKR            (*(unsigned char volatile far *)0x7efefd)
#define PWM2_DTR            (*(unsigned char volatile far *)0x7efefe)
#define PWM2_OISR           (*(unsigned char volatile far *)0x7efeff)

#define PWMA_CR1            (*(unsigned char volatile far *)0x7efec0)
#define PWMA_CR2            (*(unsigned char volatile far *)0x7efec1)
#define PWMA_SMCR           (*(unsigned char volatile far *)0x7efec2)
#define PWMA_ETR            (*(unsigned char volatile far *)0x7efec3)
#define PWMA_IER            (*(unsigned char volatile far *)0x7efec4)
#define PWMA_SR1            (*(unsigned char volatile far *)0x7efec5)
#define PWMA_SR2            (*(unsigned char volatile far *)0x7efec6)
#define PWMA_EGR            (*(unsigned char volatile far *)0x7efec7)
#define PWMA_CCMR1          (*(unsigned char volatile far *)0x7efec8)
#define PWMA_CCMR2          (*(unsigned char volatile far *)0x7efec9)
#define PWMA_CCMR3          (*(unsigned char volatile far *)0x7efeca)
#define PWMA_CCMR4          (*(unsigned char volatile far *)0x7efecb)
#define PWMA_CCER1          (*(unsigned char volatile far *)0x7efecc)
#define PWMA_CCER2          (*(unsigned char volatile far *)0x7efecd)
#define PWMA_CNTRH          (*(unsigned char volatile far *)0x7efece)
#define PWMA_CNTRL          (*(unsigned char volatile far *)0x7efecf)
#define PWMA_PSCRH          (*(unsigned char volatile far *)0x7efed0)
#define PWMA_PSCRL          (*(unsigned char volatile far *)0x7efed1)
#define PWMA_ARRH           (*(unsigned char volatile far *)0x7efed2)
#define PWMA_ARRL           (*(unsigned char volatile far *)0x7efed3)
#define PWMA_RCR            (*(unsigned char volatile far *)0x7efed4)
#define PWMA_CCR1H          (*(unsigned char volatile far *)0x7efed5)
#define PWMA_CCR1L          (*(unsigned char volatile far *)0x7efed6)
#define PWMA_CCR2H          (*(unsigned char volatile far *)0x7efed7)
#define PWMA_CCR2L          (*(unsigned char volatile far *)0x7efed8)
#define PWMA_CCR3H          (*(unsigned char volatile far *)0x7efed9)
#define PWMA_CCR3L          (*(unsigned char volatile far *)0x7efeda)
#define PWMA_CCR4H          (*(unsigned char volatile far *)0x7efedb)
#define PWMA_CCR4L          (*(unsigned char volatile far *)0x7efedc)
#define PWMA_BKR            (*(unsigned char volatile far *)0x7efedd)
#define PWMA_DTR            (*(unsigned char volatile far *)0x7efede)
#define PWMA_OISR           (*(unsigned char volatile far *)0x7efedf)

#define PWMB_CR1            (*(unsigned char volatile far *)0x7efee0)
#define PWMB_CR2            (*(unsigned char volatile far *)0x7efee1)
#define PWMB_SMCR           (*(unsigned char volatile far *)0x7efee2)
#define PWMB_ETR            (*(unsigned char volatile far *)0x7efee3)
#define PWMB_IER            (*(unsigned char volatile far *)0x7efee4)
#define PWMB_SR1            (*(unsigned char volatile far *)0x7efee5)
#define PWMB_SR2            (*(unsigned char volatile far *)0x7efee6)
#define PWMB_EGR            (*(unsigned char volatile far *)0x7efee7)
#define PWMB_CCMR1          (*(unsigned char volatile far *)0x7efee8)
#define PWMB_CCMR2          (*(unsigned char volatile far *)0x7efee9)
#define PWMB_CCMR3          (*(unsigned char volatile far *)0x7efeea)
#define PWMB_CCMR4          (*(unsigned char volatile far *)0x7efeeb)
#define PWMB_CCER1          (*(unsigned char volatile far *)0x7efeec)
#define PWMB_CCER2          (*(unsigned char volatile far *)0x7efeed)
#define PWMB_CNTRH          (*(unsigned char volatile far *)0x7efeee)
#define PWMB_CNTRL          (*(unsigned char volatile far *)0x7efeef)
#define PWMB_PSCRH          (*(unsigned char volatile far *)0x7efef0)
#define PWMB_PSCRL          (*(unsigned char volatile far *)0x7efef1)
#define PWMB_ARRH           (*(unsigned char volatile far *)0x7efef2)
#define PWMB_ARRL           (*(unsigned char volatile far *)0x7efef3)
#define PWMB_RCR            (*(unsigned char volatile far *)0x7efef4)
#define PWMB_CCR1H          (*(unsigned char volatile far *)0x7efef5)
#define PWMB_CCR1L          (*(unsigned char volatile far *)0x7efef6)
#define PWMB_CCR2H          (*(unsigned char volatile far *)0x7efef7)
#define PWMB_CCR2L          (*(unsigned char volatile far *)0x7efef8)
#define PWMB_CCR3H          (*(unsigned char volatile far *)0x7efef9)
#define PWMB_CCR3L          (*(unsigned char volatile far *)0x7efefa)
#define PWMB_CCR4H          (*(unsigned char volatile far *)0x7efefb)
#define PWMB_CCR4L          (*(unsigned char volatile far *)0x7efefc)
#define PWMB_BKR            (*(unsigned char volatile far *)0x7efefd)
#define PWMB_DTR            (*(unsigned char volatile far *)0x7efefe)
#define PWMB_OISR           (*(unsigned char volatile far *)0x7efeff)

/////////////////////////////////////////////////
//7E:FD00H-7E:FDFFH
/////////////////////////////////////////////////



/////////////////////////////////////////////////
//7E:FC00H-7E:FCFFH
/////////////////////////////////////////////////



/////////////////////////////////////////////////
//7E:FB00H-7E:FBFFH
/////////////////////////////////////////////////



/////////////////////////////////////////////////
//7E:FA00H-7E:FAFFH
/////////////////////////////////////////////////



/////////////////////////////////////////////////
//DCAN Control Regiter
/////////////////////////////////////////////////

#define MR       0x00 
#define CMR      0x01 
#define SR       0x02 
#define ISR      0x03 
#define IMR      0x04 
#define RMC      0x05 
#define BTR0     0x06 
#define BTR1     0x07 
#define TM0      0x06 
#define TM1      0x07 
#define TX_BUF0  0x08 
#define TX_BUF1  0x09 
#define TX_BUF2  0x0a 
#define TX_BUF3  0x0b 
#define RX_BUF0  0x0c 
#define RX_BUF1  0x0d 
#define RX_BUF2  0x0e 
#define RX_BUF3  0x0f 
#define ACR0     0x10 
#define ACR1     0x11 
#define ACR2     0x12 
#define ACR3     0x13 
#define AMR0     0x14 
#define AMR1     0x15 
#define AMR2     0x16 
#define AMR3     0x17 
#define ECC      0x18 
#define TXERR    0x19 
#define RXERR    0x1a 
#define ALC      0x1b 

/////////////////////////////////////////////////

#define INT0_VECTOR         0       //0003H
#define TMR0_VECTOR         1       //000BH
#define INT1_VECTOR         2       //0013H
#define TMR1_VECTOR         3       //001BH
#define UART1_VECTOR        4       //0023H
#define ADC_VECTOR          5       //002BH
#define LVD_VECTOR          6       //0033H
#define PWM1_VECTOR         7       //003BH
#define UART2_VECTOR        8       //0043H
#define SPI_VECTOR          9       //004BH
#define INT2_VECTOR         10      //0053H
#define INT3_VECTOR         11      //005BH
#define TMR2_VECTOR         12      //0063H
#define USER_VECTOR         13      //006BH
#define INT4_VECTOR         16      //0083H
#define UART3_VECTOR        17      //008BH
#define UART4_VECTOR        18      //0093H
#define TMR3_VECTOR         19      //009BH
#define TMR4_VECTOR         20      //00A3H
#define CMP_VECTOR          21      //00ABH
#define USB_VECTOR          22      //00B3H
#define PWM2_VECTOR         23      //00BBH
#define I2C_VECTOR          24      //00C3H
#define CAN_VECTOR          28      //00E3H
#define LIN_VECTOR          29      //00EBH

/////////////////////////////////////////////////

#endif

