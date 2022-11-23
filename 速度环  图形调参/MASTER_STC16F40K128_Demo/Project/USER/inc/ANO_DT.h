#ifndef __ANO_TC_H__
#define __ANO_TC_H__

#include "zf_uart.h"
#include "common.h"


//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆分成单独字节进行发送
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)	  ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )


void ANO_DT_Send_Data(uint8 *dataToSend , uint8 length);
void ANO_DT_SendtoUSER(uint16 data1, uint16 data2, uint16 data3,uint16 data4);
//void ANO_DT_Data_Exchange(void);


#endif


