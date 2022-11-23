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



// 隶属度级别
 typedef struct 
{ Fuzzy_Rank_e Rank; // 分级数量
	 float fe; // e(k)
	 float fec; // e(k)'
	 float eRule[7]; //误差表 
	 float ecRule[7]; //误差变化率表
	 float URule[7]; //舵机P值表 
	 int rule[7][7]; //规则表 
}FuzzyStruct; // 模糊结构体 


extern FuzzyStruct Direct_P; 
extern FuzzyStruct Duoji_P; 
extern float Fuzzy_Update(FuzzyStruct* F_S, float ek, float ekc); 
float FuzzyCtrl(FuzzyStruct* Fuzzy_S);


#endif

