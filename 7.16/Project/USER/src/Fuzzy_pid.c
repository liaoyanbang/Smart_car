#include "Fuzzy_pid.h" 
//61   24
#include <math.h>
FuzzyStruct Duoji_P = 
	{ Fuzzy_Rank7, 0,0,
	{ -60, -40, -20, 0, 20, 40,  60 },//ƫ��
	{ -24, -16, -8,  0, 8,  16,  24 },// ƫ��仯��
	{220 , 240 ,260, 280, 300, 320, 340},//���Pֵ
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
		
		
/*ģ���㷨����*/

#define PMAX    100
#define PMIN    -100
#define DMAX    100
#define DMIN    -100
#define FMAX    100
/*����ֵ������ֵ*/

/**********************************************************/
float Fuzzy_sp(int *P,int *D)   /*ģ��*/
{
  
  
  float PFF[4]={0,20,45,70};//{0,12,24,48};
  /*������D���Ե�������*/
  float DFF[4]={0,8,16,24};//{0,16,32,64};
  /*�����U������ֵ��������*/
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
  /*ƫ��,ƫ��΢���Լ����ֵ�ľ�ȷ��*/
  int   PF[2],DF[2],UF[4];
  /*ƫ��,ƫ��΢���Լ����ֵ��������*/
  int    Pn,Dn,Un[4];
  long   temp1,temp2;
  
  /*�����ȵ�ȷ��*/
  /*����P/ƫ��/D/ƫ��ı仯��/��ָ�����Ի����Ч������*/
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
  /*ʹ����Χ�Ż���Ĺ����rule[7][7]*/
  /*���ֵʹ��13����������,����ֵ��UFF[7]ָ��*/
  /*һ�㶼���ĸ�������Ч*/
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
  /*ͬ���������������ֵ���*/
  
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
  
  /*���ķ���ģ��*/
  /*Un[]ԭֵΪ������������ı�ţ�ת��Ϊ��������ֵ*/
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
�������ƣ�FuzzyCtrl * 
�������ܣ�ģ���� ȥģ�� * 
��ڲ�����FuzzyStruct* Fuzzy_S * 
���ڲ�����Fuzzy Value *****/


float FuzzyCtrl(FuzzyStruct* Fuzzy_S) 
{ 
	float eFuzzy[2] = { 0.0, 0.0 };//ƫ��������ֵ 
	float ecFuzzy[2] = { 0.0, 0.0 };//ƫ��΢�� 
	float QRule[4] = { 0.0, 0.0, 0.0, 0.0 }; //URule[]���ֵ 
	float U1Fuzzy[4] = { 0.0, 0.0, 0.0, 0.0 }; //���ֵ�������� 
	int pe = 0, pec = 0, U1Rule[4] = { 0, 0, 0, 0 };//ƫ�� ƫ��ı仯�� ���õ��� ������ 
	float U = 0;//���ֵ�ľ�ȷ��
	int rank;//�����ȼ���
	float temp1, temp2;//���ķ��м����
	rank = Fuzzy_S->Rank; 
	/*----------------------------------------ģ���������� ------------------------------------------*/ 
	switch (rank) 
	{
		case 7: { 
			/*-----���������������-----*/ 
			if (Fuzzy_S->fe <= Fuzzy_S->eRule[0]) // |x_x_x_x_x_x_x_
			{ 
				eFuzzy[0] = 1.0;
				pe = -2; 
			}
			else if (Fuzzy_S->fe <= Fuzzy_S->eRule[1]) // _x|x_x_x_x_x_x_
			{
				eFuzzy[0] = (Fuzzy_S->eRule[1] - Fuzzy_S->fe) / (Fuzzy_S->eRule[1] - Fuzzy_S->eRule[0]);//�ƽ��̶����� ��һ������ 
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
/*-----���仯��������������-----*/ 
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
		ecFuzzy[1] = 1 - ecFuzzy[0]; break; }//...7��������
		default: break; }//�������������
	
		/*****************************************************************/ 
		/*��ѯģ������� rule[][]*/ 
		/*����ֵ��URule[]���*/ 
		/*һ�㶼���ĸ�������Ч*/
		U1Rule[0] = Fuzzy_S->rule[pe - 1 + 3][pec - 1 + 3];
		U1Rule[1] = Fuzzy_S->rule[pe + 3][pec - 1 + 3];
		U1Rule[2] = Fuzzy_S->rule[pe - 1 + 3][pec + 3]; 
		U1Rule[3] = Fuzzy_S->rule[pe + 3][pec + 3]; //��������� 
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
		/*ͬ���������������ֵ���*/ 
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
		/*���ķ���ģ��*/ 
		/*U1Rule[]ԭֵΪ�������������ţ�ת��ΪURule[]��������ֵ*/
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
* �������ƣ�Fuzzy_Update
* �������ܣ�ģ�����Ƹ��� 
* ��ڲ�����FuzzyStruct* F_S ekekc 
* ���ڲ�����Fuzzy Value 
**********************************************************************/
float Fuzzy_Update(FuzzyStruct* F_S, float ek, float ekc) 
{ 
	float value = 0;
	F_S->fe = ek; 
	F_S->fec = ekc; 
	value = FuzzyCtrl(F_S); 
	return value; 
}