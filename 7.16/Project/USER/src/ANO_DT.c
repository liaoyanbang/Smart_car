#include "ANO_DT.h"


uint8 send_flag ;

uint8 data_to_send[50] = {0};



//移植时，用户应根据自身应用的情况，根据使用的通信方式，实现此函数，这里就采用有线连接，发送至串口2了
void ANO_DT_Send_Data(uint8 *dataToSend , uint8 length)
{
/**使用串口正常发送数据，大概需要1.5ms*/
  uart_putbuff(UART_1,dataToSend,length);	//可以修改不同的串口发送数据

}

//通信协议函数
void ANO_DT_SendtoUSER(uint16 data1, uint16 data2, uint16 data3,uint16 data4)	
{
  uint8 _cnt=0,sum = 0,i;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xF1; //自定义帧1
	data_to_send[_cnt++]=0;    //这是len位，后面 _cnt-4来赋值

	data_to_send[_cnt++]=BYTE1(data1);
	data_to_send[_cnt++]=BYTE0(data1);//数据部分
		
	data_to_send[_cnt++]=BYTE1(data2);
	data_to_send[_cnt++]=BYTE0(data2);//数据部分
	
	data_to_send[_cnt++]=BYTE1(data3);
	data_to_send[_cnt++]=BYTE0(data3);//数据部分
	
	data_to_send[_cnt++]=BYTE1(data4);
	data_to_send[_cnt++]=BYTE0(data4);//数据部分
	
	data_to_send[3] = _cnt-4;//长度位
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;//校验位
	
	ANO_DT_Send_Data(data_to_send, _cnt);
}


////结合定时器，设置各个数据发送标志位，可以定期发送各种数据到上位机。
////Data_Exchange函数处理各种数据发送请求，比如想实现每5ms发送一次传感器数据至上位机，即在此函数内实现
////此函数每1ms调用一次
//void ANO_DT_Data_Exchange(void)
//{
//	static uint8_t cnt 	= 0;
//	cnt++;
//	if(cnt>=10)//10MS
//	{
//		cnt = 0;
//		send_flag = 1;
//	}	
//	
//		//HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
//	if(send_flag)
//	{
//		send_flag = 0;
//	 // ANO_DT_SendtoUSER(Servo_pid.x_curr_posi,Servo_pid.y_curr_posi,Servo_pid.x_curr_angle,Servo_pid.y_curr_angle);
//	
//		ANO_DT_SendtoUSER(Position_y[0],Position_y[1],Position_y[2],Position_y[3]);
//	}
//}


