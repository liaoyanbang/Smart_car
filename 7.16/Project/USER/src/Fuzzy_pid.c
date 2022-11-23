#include "Fuzzy_pid.h" 
//61   24
#include <math.h>
FuzzyStruct Duoji_P = 
	{ Fuzzy_Rank7, 0,0,
	{ -60, -40, -20, 0, 20, 40,  60 },//偏差
	{ -24, -16, -8,  0, 8,  16,  24 },// 偏差变化率
	{220 , 240 ,260, 280, 300, 320, 340},//舵机P值
	{ 
  { 6, 6, 6, 5, 5, 5, 5, }, // -3
	{ 5, 4, 4, 3, 2, 2, 1, }, // -2
	{ 4, 3, 2, 1, 0, 1, 2, }, // -1
	{ 4, 3, 1, 0, 1, 3, 4, }, // 0
	{ 2, 1, 0, 1, 2, 3, 4, }, // 1 
	{ 1, 2, 2, 3, 4, 4, 5, }, // 2 
	{ 5, 5, 5, 5, 6, 6, 6, }, // 3 
		} };
/***************
*****************************************
		
		
/*模糊算法程序*/

#define PMAX    100
#define PMIN    -100
#define DMAX    100
#define DMIN    -100
#define FMAX    100
/*语言值的满幅值*/

/**********************************************************/
float Fuzzy_sp(int *P,int *D)   /*模糊*/
{
  
  
  float PFF[4]={0,20,45,70};//{0,12,24,48};
  /*输入量D语言的特征点*/
  float DFF[4]={0,8,16,24};//{0,16,32,64};
  /*输出量U的语言值的特征点*/
  float UFF[7]={180,200,220,240,260,280,300};//{0,15,30,45,60,75,90};
  int rule[7][7]={
    //  //ec -3,-2,-1, 0, 1, 2, 3       //   e
    //      { 6, 5, 4, 3, 2, 1, 0,},   //   -3
    //      { 5, 4, 3, 2, 1, 0,-1,},   //   -2
    //      { 4, 3, 2, 1, 0,-1,-2,},   //   -1
    //      { 3, 2, 1, 0,-1,-2,-3,},   //    0
    //      { 4, 3, 2, 1, 0,-1,-2,},   //    1
    //      { 5, 4, 3, 2, 1, 0,-1,},   //    2
    //      { 6, 5, 4, 3, 2, 1, 0}     //    3
    //      };
    //ec -3,-2,-1, 0, 1, 2, 3       //   e
    { 6, 5, 4, 3, 3, 3, 2,},   //   -3
    { 5, 4, 3, 2, 2, 1, 0,},   //   -2
    { 4, 3, 2, 1, 0,-1,-2,},   //   -1
    { 3, 2, 1, 0,-1,-3,-4,},   //    0
    { 4, 3, 2, 1, 0,-1,-2,},   //    1
    { 5, 4, 3, 2, 2, 1, 0,},   //    2
    { 6, 5, 4, 3, 3, 3, 2}     //    3
  };
  
  
  
  float    U;
  /*偏差,偏差微分以及输出值的精确量*/
  int   PF[2],DF[2],UF[4];
  /*偏差,偏差微分以及输出值的隶属度*/
  int    Pn,Dn,Un[4];
  long   temp1,temp2;
  
  /*隶属度的确定*/
  /*根据P/偏差/D/偏差的变化量/的指定语言获得有效隶属度*/
  if(*P>-PFF[3] && *P<PFF[3])
  {
    if(*P<=-PFF[2])
    {
      Pn=-2;
      PF[0]=FMAX*((float)(-PFF[2]-*P)/(PFF[3]-PFF[2]));
    }
    else if(*P<=-PFF[1])
    {
      Pn=-1;
      PF[0]=FMAX*((float)(-PFF[1]-*P)/(PFF[2]-PFF[1]));
    }
    else if(*P<=PFF[0])
    {
      Pn=0;
      PF[0]=FMAX*((float)(-PFF[0]-*P)/(PFF[1]-PFF[0]));
    }
    else if(*P<=PFF[1])
    {
      Pn=1; PF[0]=FMAX*((float)(PFF[1]-*P)/(PFF[1]-PFF[0]));
    }
    else if(*P<=PFF[2])
    {
      Pn=2; PF[0]=FMAX*((float)(PFF[2]-*P)/(PFF[2]-PFF[1]));
    }
    else if(*P<=PFF[3])
    {
      Pn=3; PF[0]=FMAX*((float)(PFF[3]-*P)/(PFF[3]-PFF[2]));
    }
  }
  else if(*P<=-PFF[3])
  {
    Pn=-2;   PF[0]=FMAX;
  }
  else if(*P>=PFF[3])
  {
    Pn=3;   PF[0]=0;
  }
  PF[1]=FMAX-PF[0];
  if(*D>-DFF[3] && *D<DFF[3])
  {
    if(*D<=-DFF[2])
    {
      Dn=-2;DF[0]=FMAX*((float)(-DFF[2]-*D)/(DFF[3]-DFF[2]));
    }
    else if(*D<=-DFF[1])
    {
      Dn=-1;
      DF[0]=FMAX*((float)(-DFF[1]-*D)/(DFF[2]-DFF[1]));
    }
    else if(*D<=DFF[0])
    {
      Dn=0;
      DF[0]=FMAX*((float)(-DFF[0]-*D)/(DFF[1]-DFF[0]));
    }
    else if(*D<=DFF[1])
    {
      Dn=1;
      DF[0]=FMAX*((float)(DFF[1]-*D)/(DFF[1]-DFF[0]));
    }
    else if(*D<=DFF[2])
    {
      Dn=2; DF[0]=FMAX*((float)(DFF[2]-*D)/(DFF[2]-DFF[1]));
    }
    else if(*D<=DFF[3])
    {
      Dn=3; DF[0]=FMAX*((float)(DFF[3]-*D)/(DFF[3]-DFF[2]));
    }
  }
  else if(*D<=-DFF[3])
  {
    Dn=-2;
    DF[0]=FMAX;
  }
  else if(*D>=DFF[3])
  {
    Dn=3;
    DF[0]=0;
  }
  DF[1]=FMAX-DF[0];
  /*使用误差范围优化后的规则表rule[7][7]*/
  /*输出值使用13个隶属函数,中心值由UFF[7]指定*/
  /*一般都是四个规则有效*/
  Un[0]=rule[Pn-1+3][Dn-1+3];
  Un[1]=rule[Pn+3][Dn-1+3];
  Un[2]=rule[Pn-1+3][Dn+3];
  Un[3]=rule[Pn+3][Dn+3];
  if(PF[0]<=DF[0])
    UF[0]=PF[0];
  else
    UF[0]=DF[0];
  if(PF[1]<=DF[0])
    UF[1]=PF[1];
  else
    UF[1]=DF[0];
  if(PF[0]<=DF[1])
    UF[2]=PF[0];
  else
    UF[2]=DF[1];
  if(PF[1]<=DF[1])
    UF[3]=PF[1];
  else
    UF[3]=DF[1];
  /*同隶属函数输出语言值求大*/
  
  if(Un[0]==Un[1])
  {
    if(UF[0]>UF[1])
      UF[1]=0;
    else
      UF[0]=0;
  }
  if(Un[0]==Un[2])
  {
    if(UF[0]>UF[2])
      UF[2]=0;
    else
      UF[0]=0;
  }
  if(Un[0]==Un[3])
  {
    if(UF[0]>UF[3])
      UF[3]=0;
    else
      UF[0]=0;
  }
  if(Un[1]==Un[2])
  {
    if(UF[1]>UF[2])
      UF[2]=0;
    else
      UF[1]=0;
  }
  if(Un[1]==Un[3])
  {
    if(UF[1]>UF[3])
      UF[3]=0;
    else
      UF[1]=0;
  }
  if(Un[2]==Un[3])
  {
    if(UF[2]>UF[3])
      UF[3]=0;
    else
      UF[2]=0;
  }
  
  /*重心法反模糊*/
  /*Un[]原值为输出隶属函数的标号，转换为隶属函数值*/
  if(Un[0]>=0)
    Un[0]=UFF[Un[0]];
  else
    Un[0]=-UFF[-Un[0]];
  if(Un[1]>=0)
    Un[1]=UFF[Un[1]];
  else
    Un[1]=-UFF[-Un[1]];
  if(Un[2]>=0)
    Un[2]=UFF[Un[2]];
  else
    Un[2]=-UFF[-Un[2]];
  if(Un[3]>=0)
    Un[3]=UFF[Un[3]];
  else
    Un[3]=-UFF[-Un[3]];
  
  temp1=UF[0]*Un[0]+UF[1]*Un[1]+UF[2]*Un[2]+UF[3]*Un[3];
  temp2=UF[0]+UF[1]+UF[2]+UF[3];
  U=temp1/temp2;
  return U;
}


/************** * 
函数名称：FuzzyCtrl * 
函数功能：模糊化 去模糊 * 
入口参数：FuzzyStruct* Fuzzy_S * 
出口参数：Fuzzy Value *****/


float FuzzyCtrl(FuzzyStruct* Fuzzy_S) 
{ 
	float eFuzzy[2] = { 0.0, 0.0 };//偏差隶属度值 
	float ecFuzzy[2] = { 0.0, 0.0 };//偏差微分 
	float QRule[4] = { 0.0, 0.0, 0.0, 0.0 }; //URule[]输出值 
	float U1Fuzzy[4] = { 0.0, 0.0, 0.0, 0.0 }; //输出值的隶属度 
	int pe = 0, pec = 0, U1Rule[4] = { 0, 0, 0, 0 };//偏差 偏差的变化率 采用的四 条规则 
	float U = 0;//输出值的精确量
	int rank;//隶属度级别
	float temp1, temp2;//重心法中间变量
	rank = Fuzzy_S->Rank; 
	/*----------------------------------------模糊语言描述 ------------------------------------------*/ 
	switch (rank) 
	{
		case 7: { 
			/*-----误差隶属函数描述-----*/ 
			if (Fuzzy_S->fe <= Fuzzy_S->eRule[0]) // |x_x_x_x_x_x_x_
			{ 
				eFuzzy[0] = 1.0;
				pe = -2; 
			}
			else if (Fuzzy_S->fe <= Fuzzy_S->eRule[1]) // _x|x_x_x_x_x_x_
			{
				eFuzzy[0] = (Fuzzy_S->eRule[1] - Fuzzy_S->fe) / (Fuzzy_S->eRule[1] - Fuzzy_S->eRule[0]);//逼近程度描述 归一化处理 
				pe = -2; 
			}
			else if (Fuzzy_S->fe <= Fuzzy_S->eRule[2]) // _x_x|x_x_x_x_x_ 
			{ 
				eFuzzy[0] = (Fuzzy_S->eRule[2] - Fuzzy_S->fe) / (Fuzzy_S->eRule[2] - Fuzzy_S->eRule[1]);
				pe = -1;
			}else if (Fuzzy_S->fe <= Fuzzy_S->eRule[3]) // _x_x_x|x_x_x_x_ 
			{ 
				eFuzzy[0] = (Fuzzy_S->eRule[3] - Fuzzy_S->fe) / (Fuzzy_S->eRule[3] - Fuzzy_S->eRule[2]); 
				pe = 0; 
			}
			else if (Fuzzy_S->fe <= Fuzzy_S->eRule[4]) // _x_x_x_x|x_x_x_ 
			{ 
				eFuzzy[0] = (Fuzzy_S->eRule[4] - Fuzzy_S->fe) / (Fuzzy_S->eRule[4] - Fuzzy_S->eRule[3]); 
				pe = 1; 
			}
			else if (Fuzzy_S->fe <= Fuzzy_S->eRule[5]) // _x_x_x_x_x|x_x_ 
			{ 
				eFuzzy[0] = (Fuzzy_S->eRule[5] - Fuzzy_S->fe) / (Fuzzy_S->eRule[5] - Fuzzy_S->eRule[4]); 
				pe = 2; 
			}
			else if (Fuzzy_S->fe <= Fuzzy_S->eRule[6]) // _x_x_x_x_x_x|x_ 
			{ 
				eFuzzy[0] = (Fuzzy_S->eRule[6] - Fuzzy_S->fe) / (Fuzzy_S->eRule[6] - Fuzzy_S->eRule[5]);
				pe = 3; 
			}
			else if (Fuzzy_S->fe >= Fuzzy_S->eRule[6]) // _x_x_x_x_x_x_x| 
			{
				eFuzzy[0] = 0; 
				pe = 3; 
			}
			eFuzzy[1] = 1 - eFuzzy[0]; 
/*-----误差变化率隶属函数描述-----*/ 
			if (Fuzzy_S->fec <= Fuzzy_S->ecRule[0]) 
			{ 
				ecFuzzy[0] = 1.0; 
				pec = -2; 
			}
			else if (Fuzzy_S->fec <= Fuzzy_S->ecRule[1]) 
			{
				ecFuzzy[0] = (Fuzzy_S->ecRule[1] - Fuzzy_S->fec) / (Fuzzy_S->ecRule[1] - Fuzzy_S->ecRule[0]); 
				pec = -2; 
			}
			else if (Fuzzy_S->fec <= Fuzzy_S->ecRule[2]) 
			{
				ecFuzzy[0] = (Fuzzy_S->ecRule[2] - Fuzzy_S->fec) / (Fuzzy_S->ecRule[2] - Fuzzy_S->ecRule[1]);
				pec = -1; 
			}
			else if (Fuzzy_S->fec <= Fuzzy_S->ecRule[3]) 
			{ 
				ecFuzzy[0] = (Fuzzy_S->ecRule[3] - Fuzzy_S->fec) / (Fuzzy_S->ecRule[3] - Fuzzy_S->ecRule[2]);
				pec = 0; 
			}
			else if (Fuzzy_S->fec <= Fuzzy_S->ecRule[4]) 
			{ 
				ecFuzzy[0] = (Fuzzy_S->ecRule[4] - Fuzzy_S->fec) / (Fuzzy_S->ecRule[4] - Fuzzy_S->ecRule[3]); 
				pec = 1;
			}
			else if (Fuzzy_S->fec <= Fuzzy_S->ecRule[5]) 
			{ 
				ecFuzzy[0] = (Fuzzy_S->ecRule[5] - Fuzzy_S->fec) / (Fuzzy_S->ecRule[5] - Fuzzy_S->ecRule[4]);
				pec = 2;
			}
			else if 
			(Fuzzy_S->fec <= Fuzzy_S->ecRule[6]) 
			{
				ecFuzzy[0] = (Fuzzy_S->ecRule[6] - Fuzzy_S->fec) / (Fuzzy_S->ecRule[6] - Fuzzy_S->ecRule[5]); 
				pec = 3;
			}
			else 
			{ 
				ecFuzzy[0] = 0;
				pec = 3;
			}
		ecFuzzy[1] = 1 - ecFuzzy[0]; break; }//...7级隶属度
		default: break; }//隶属度描述完成
	
		/*****************************************************************/ 
		/*查询模糊规则表 rule[][]*/ 
		/*中心值由URule[]输出*/ 
		/*一般都是四个规则有效*/
		U1Rule[0] = Fuzzy_S->rule[pe - 1 + 3][pec - 1 + 3];
		U1Rule[1] = Fuzzy_S->rule[pe + 3][pec - 1 + 3];
		U1Rule[2] = Fuzzy_S->rule[pe - 1 + 3][pec + 3]; 
		U1Rule[3] = Fuzzy_S->rule[pe + 3][pec + 3]; //相近隶属度 
		if (eFuzzy[0] <= ecFuzzy[0]) // 0-0 
			U1Fuzzy[0] = eFuzzy[0];
		else U1Fuzzy[0] = ecFuzzy[0];
		if (eFuzzy[1] <= ecFuzzy[0]) // 1-0 
			U1Fuzzy[1] = eFuzzy[1]; 
		else U1Fuzzy[1] = ecFuzzy[0]; 
		if (eFuzzy[0] <= ecFuzzy[1]) // 0-1 
			U1Fuzzy[2] = eFuzzy[0]; 
		else U1Fuzzy[2] = ecFuzzy[1];
		if (eFuzzy[1] <= ecFuzzy[1]) // 1-1 
			U1Fuzzy[3] = eFuzzy[1]; 
		else U1Fuzzy[3] = ecFuzzy[1];
		/*同隶属函数输出语言值求大*/ 
		if (U1Rule[0] == U1Rule[1]) 
		{ 
			if (U1Fuzzy[0] > U1Fuzzy[1]) 
			U1Fuzzy[1] = 0; 
			else U1Fuzzy[0] = 0; 
		}
		if (U1Rule[0] == U1Rule[2]) 
		{ 
			if (U1Fuzzy[0] > U1Fuzzy[2]) 
			U1Fuzzy[2] = 0; 
			else U1Fuzzy[0] = 0; 
		}
		if (U1Rule[0] == U1Rule[3]) 
		{
			if (U1Fuzzy[0] > U1Fuzzy[3]) 
				U1Fuzzy[3] = 0;
			else U1Fuzzy[0] = 0; 
		}
		if (U1Rule[1] == U1Rule[2]) 
		{
			if (U1Fuzzy[1] > U1Fuzzy[2])
				U1Fuzzy[2] = 0;
			else U1Fuzzy[1] = 0; 
		}
		if (U1Rule[1] == U1Rule[3]) 
		{ 
			if (U1Fuzzy[1] > U1Fuzzy[3])
			U1Fuzzy[3] = 0; 
			else U1Fuzzy[1] = 0; 
		}
		if (U1Rule[2] == U1Rule[3])
		{ 
			if (U1Fuzzy[2] > U1Fuzzy[3]) 
			U1Fuzzy[3] = 0;
			else U1Fuzzy[2] = 0; 
		}
		
		/*****************************************************************/ 
		/*重心法反模糊*/ 
		/*U1Rule[]原值为输出隶属函数标号，转换为URule[]隶属函数值*/
		if (U1Rule[0] >= 0) 
			QRule[0] = Fuzzy_S->URule[U1Rule[0]]; 
		else QRule[0] = -(Fuzzy_S->URule[-U1Rule[0]]); 
		if (U1Rule[1] >= 0) 
			QRule[1] = Fuzzy_S->URule[U1Rule[1]];
		else QRule[1] = -(Fuzzy_S->URule[-U1Rule[1]]); 
		if (U1Rule[2] >= 0) 
			QRule[2] = Fuzzy_S->URule[U1Rule[2]]; 
		else QRule[2] = -(Fuzzy_S->URule[-U1Rule[2]]); 
		if (U1Rule[3] >= 0) 
			QRule[3] = Fuzzy_S->URule[U1Rule[3]]; 
		else QRule[3] = -(Fuzzy_S->URule[-U1Rule[3]]);
		temp1 = U1Fuzzy[0] * QRule[0] + U1Fuzzy[1] * QRule[1] + U1Fuzzy[2] * QRule[2] + U1Fuzzy[3] * QRule[3]; 
		temp2 = U1Fuzzy[0] + U1Fuzzy[1] + U1Fuzzy[2] + U1Fuzzy[3];
		U = temp1 / temp2; 
		return U; 
}

/********************************************************************** 
* 函数名称：Fuzzy_Update
* 函数功能：模糊控制更新 
* 入口参数：FuzzyStruct* F_S ekekc 
* 出口参数：Fuzzy Value 
**********************************************************************/
float Fuzzy_Update(FuzzyStruct* F_S, float ek, float ekc) 
{ 
	float value = 0;
	F_S->fe = ek; 
	F_S->fec = ekc; 
	value = FuzzyCtrl(F_S); 
	return value; 
}