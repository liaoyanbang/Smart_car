#ifndef _Fuzzy_Pid_H 
#define _Fuzzy_Pid_H

typedef enum
{ 
	Fuzzy_Rank9 = 9,
	Fuzzy_Rank7 = 7, 
	Fuzzy_Rank5 = 5, 
	Fuzzy_Rank3 = 3, 
}
 Fuzzy_Rank_e;

#define PMAX    100
#define PMIN    -100
#define DMAX    100
#define DMIN    -100
#define FMAX    100
/*����ֵ������ֵ*/

// �����ȼ���
 typedef struct 
{ Fuzzy_Rank_e Rank; // �ּ�����
	 float fe; // e(k)
	 float fec; // e(k)'
	 float eRule[7]; //���� 
	 float ecRule[7]; //���仯�ʱ�
	 float URule[7]; //���Pֵ�� 
	 int rule[7][7]; //����� 
}FuzzyStruct; // ģ���ṹ�� 

float Fuzzy_sp(int * P,int * D);
extern FuzzyStruct Direct_P; 
extern FuzzyStruct Duoji_P; 
extern float Fuzzy_Update(FuzzyStruct* F_S, float ek, float ekc); 
float FuzzyCtrl(FuzzyStruct* Fuzzy_S);


#endif

