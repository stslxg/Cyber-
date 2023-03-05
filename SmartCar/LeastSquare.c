#include <stdio.h>
typedef unsigned int U16;
typedef signed int S16;

#define MAX_POSITION_HISTORY (5)
typedef struct{
  S16 MultiplySum;
	S16 PositionSum;
	U16 LineSum;
	U16 LineSqareSum;
	U16 Line[MAX_POSITION_HISTORY];
	S16 Position[MAX_POSITION_HISTORY];	
	U16 CURRENT;
} POSITION_SET;
static inline void NtpInitPosition(POSITION_SET * p){
	U16 i;

	p -> MultiplySum = 0;
	p -> PositionSum = 0;
	p -> LineSum = 0;
	p -> LineSqareSum = 0;
	for(i = 0;i < MAX_POSITION_HISTORY;i++){
		p -> Line[i] = 0;
		p -> Position[i] = 0; 
	}
	p -> CURRENT = 0;
}

static inline S16 NtpGetFirstDeriv(POSITION_SET * p){
	static S16 temp,temp2;
	temp = (S16)p -> MultiplySum - (S16)(p -> PositionSum * p -> LineSum) / MAX_POSITION_HISTORY;
	temp2 = (S16)p -> LineSqareSum - (S16)(p -> LineSum * p -> LineSum) / MAX_POSITION_HISTORY;
	return (temp / temp2);
}

static inline void NtpUpdatePosition(POSITION_SET * p, U16 line, S16 position){
  p -> MultiplySum -= p->Line[p->CURRENT] * p->Position[p->CURRENT];
  p -> PositionSum -= p->Position[p->CURRENT];
  p -> LineSum -= p->Line[p->CURRENT];
  p -> LineSqareSum -= p->Line[p->CURRENT] * p->Line[p->CURRENT];
  
  p->Line[p->CURRENT] = line;
  p->Position[p->CURRENT] = position;
  
  p -> MultiplySum += line * position;
  p -> PositionSum += position;
  p -> LineSum += line;
  p -> LineSqareSum += line * line;
  
  if(++p -> CURRENT >= MAX_POSITION_HISTORY){
  	p -> CURRENT = 0;
  }
}

POSITION_SET NtpPosSet; 
U16 line,n;
S16 pos;
int main(){
  NtpInitPosition(&NtpPosSet);
  n = 0;
  while(1){
    scanf("%d %d",&line,&pos);
    n++;
    NtpUpdatePosition(&NtpPosSet,line,pos);
    if (n>=2) printf("%d \n",NtpGetFirstDeriv(&NtpPosSet));
  };
};

