#include "ANO_DT.h"


uint8 send_flag ;

uint8 data_to_send[50] = {0};



//��ֲʱ���û�Ӧ��������Ӧ�õ����������ʹ�õ�ͨ�ŷ�ʽ��ʵ�ִ˺���������Ͳ����������ӣ�����������2��
void ANO_DT_Send_Data(uint8 *dataToSend , uint8 length)
{
/**ʹ�ô��������������ݣ������Ҫ1.5ms*/
  uart_putbuff(UART_1,dataToSend,length);	//�����޸Ĳ�ͬ�Ĵ��ڷ�������

}

//ͨ��Э�麯��
void ANO_DT_SendtoUSER(uint16 data1, uint16 data2, uint16 data3,uint16 data4)	
{
  uint8 _cnt=0,sum = 0,i;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xF1; //�Զ���֡1
	data_to_send[_cnt++]=0;    //����lenλ������ _cnt-4����ֵ

	data_to_send[_cnt++]=BYTE1(data1);
	data_to_send[_cnt++]=BYTE0(data1);//���ݲ���
		
	data_to_send[_cnt++]=BYTE1(data2);
	data_to_send[_cnt++]=BYTE0(data2);//���ݲ���
	
	data_to_send[_cnt++]=BYTE1(data3);
	data_to_send[_cnt++]=BYTE0(data3);//���ݲ���
	
	data_to_send[_cnt++]=BYTE1(data4);
	data_to_send[_cnt++]=BYTE0(data4);//���ݲ���
	
	data_to_send[3] = _cnt-4;//����λ
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;//У��λ
	
	ANO_DT_Send_Data(data_to_send, _cnt);
}


////��϶�ʱ�������ø������ݷ��ͱ�־λ�����Զ��ڷ��͸������ݵ���λ����
////Data_Exchange��������������ݷ������󣬱�����ʵ��ÿ5ms����һ�δ�������������λ�������ڴ˺�����ʵ��
////�˺���ÿ1ms����һ��
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


