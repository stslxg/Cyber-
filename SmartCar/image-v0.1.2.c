
#include "image-v0.1.2.h"
#include "stdlib.h"
#include <math.h>
#include "para.h"
void InitLine(){
  unsigned char i;
  unsigned char s0,p0,s1,p1;
    
  /////////////////////  计算各行黑线跳变阈值 //////////////////////////////////

  s0 = paraData.data.blackValueStep0;
  p0 = paraData.data.blackStepPoint0;
  s1 = paraData.data.blackValueStep1;
  p1 = paraData.data.blackStepPoint1;
  for(i = 0;i< p0;i++) 
    blackValueStep[i] = s0;
  for(i = p0;i< p1;i++)
    blackValueStep[i] = floor(s0 + (float)(s1-s0)*(float)(i-p0)/(float)(p1-p0));
  for(i = p1;i<MAXROW;i++)
    blackValueStep[i] = s1;

  /////////////////////////  计算各行黑线阈值  ///// ///////////////////////////

  s0 = paraData.data.blackValue0;
  p0 = paraData.data.blackValuePoint0;
  s1 = paraData.data.blackValue1;
  p1 = paraData.data.blackValuePoint1;
  for(i = 0;i< p0;i++) 
    whiteValue[i] = s0;
  for(i = p0;i< p1;i++)
    whiteValue[i] = floor(s0 + (float)(s1-s0)*(float)(i-p0)/(float)(p1-p0));
  for(i = p1;i<MAXROW;i++)
    whiteValue[i] = s1;

  /////////////////////////////////////////////////////////////////////////
  
  hasOvered = False;
  hasStartLine = False;
  startDelayCnt = STARTDELAY;
  hasMissed = True;
};

void GetPointbyDelta(const unsigned char x,const unsigned char y,const unsigned char delta,unsigned char * newX,unsigned char * newY){
  *newX = x - delta;
  if((*newX) <= 0) *newX = 0;
  *newY = y + delta;
  if((*newY) >= MAXCOL) *newY = MAXCOL-1;
};

void GetLine(){
  char i,row,line1,line2;
  unsigned char  hasFound,isAllBlack;  
  unsigned char bestDistance,bestPoint,hasGotLine,tmpPoint,missedCnt;

  //////////////////////////  前MAXBASELINE行特殊处理，作为基准行 ////////////////////
  for(i = 0;i<MAXBASELINE;i++){
    hasFound = False;
    if(hasMissed == True){                   // 上次丢线，全部重新查找
      GetPointbyDelta(FIRST_LEFT,FIRST_RIGHT,FIRST_DELTA,&lineLeft,&lineRight);
    } else {                                 // 在上次找到的范围附近查找
      GetPointbyDelta(preBlackLine[i],preBlackLine[i],FIRST_DELTA+maxBlackWidth[i] / 2,&lineLeft,&lineRight);
    };

    row = MAXROW - i - 1;
    ///////////////////  查找跳变点  ////////////////////////////////////////
    for(line1 = lineLeft;line1 <= lineRight;line1++){
      if(line1 < LINESTEP && image[row][line1] < whiteValue[i] || image[row][line1-LINESTEP] >= image[row][line1]+blackValueStep[i]){
        hasFound = True;
        break;
      };
    };
    if(hasFound == True){
      hasFound = False;
      for(line2 = line1;line2 <= lineRight;line2++){
        if(line2 >= MAXCOL - LINESTEP && image[row][line2] < whiteValue[i] || image[row][line2+LINESTEP] >= image[row][line2]+blackValueStep[i]){
	  hasFound = True;
	  break;
        };
      };
    };
    ////////////////////////////////////////////////////////////////////////
    if( hasFound == True && line2 - line1+1 >=minBlackWidth[i] && line2 - line1+1 <= maxBlackWidth[i]){                    // 终于找到了 //////////////////
      blackLine[i] = (line1 + line2) / 2;
      forwardBase = i;
      break;
    };
  };


  if(hasMissed == False && hasFound == False){
    for(i = 0;i<MAXBASELINE;i++){
      hasFound = False;
      GetPointbyDelta(FIRST_LEFT,FIRST_RIGHT,FIRST_DELTA,&lineLeft,&lineRight);

      row = MAXROW - i - 1;
      ///////////////////  查找跳变点  ////////////////////////////////////////
      for(line1 = lineLeft;line1 <= lineRight;line1++){
        if(line1 < LINESTEP && image[row][line1] < whiteValue[i] || image[row][line1-LINESTEP] >= image[row][line1]+blackValueStep[i]){
          hasFound = True;
          break;
        };
      };
      if(hasFound == True){
        hasFound = False;
        for(line2 = line1;line2 <= lineRight;line2++){
          if(line2 >= MAXCOL - LINESTEP && image[row][line2] < whiteValue[i] || image[row][line2+LINESTEP] >= image[row][line2]+blackValueStep[i]){
	    hasFound = True;
	    break;
          };
        };
      };
      ////////////////////////////////////////////////////////////////////////
      if( hasFound == True && line2 - line1+1 >=minBlackWidth[i] && line2 - line1+1 <= maxBlackWidth[i]){                    // 终于找到了 //////////////////
        blackLine[i] = (line1 + line2) / 2;
        forwardBase = i;
        break;
      };
    };
  };

  if(hasFound == False){              //// 前MAXBASELINE行都找不到  /////
    hasMissed = True;
    forwardDistance = 0;
    return;
  } else {
    hasMissed = False;
    for(i = forwardBase-1; i>=0;i--)
      blackLine[i] = blackLine[forwardBase];
    forwardDistance = i = forwardBase;
  };


  missedCnt = 0;
  ////////////////////////////////    *** 继续 ****查找其他行  ///////////////////////////////////////
  /// 注意是  继续！！！！！ ////////////////////
  while(i<MAXROW){
    i++;
    row = MAXROW - 1 - i;
    
    ////// 由前面信息推本行有可能线的位置 //////////
    if ( i == forwardBase + 1 ) blackLine[i] = blackLine[i-1]; /// 前面只有一行
    else if (2*blackLine[i-1] < blackLine[i-2])
      blackLine[i] = 0;
    else blackLine[i] = 2*blackLine[i-1] - blackLine[i-2];
    if(blackLine[i] >= MAXCOL ) blackLine[i] = MAXCOL -1;

    ////////   得出搜索范围   /////////////////////
    GetPointbyDelta(blackLine[i],blackLine[i],lineDelta[i]+(maxBlackWidth[i]+1)/2,&lineLeft,&lineRight);
    hasFound = False;


    ////////    搜索多条黑线 ////////////////////////
    hasGotLine = False;
    bestDistance = MAXCOL;
    bestPoint = 0;
    line1 = lineLeft;
    while(line1 <=lineRight){               
      hasFound = False;
      for(;line1 <= lineRight;line1++){
        if(line1 < LINESTEP && image[row][line1] < whiteValue[i] || image[row][line1-LINESTEP] >= image[row][line1]+blackValueStep[i]){
          hasFound = True;
          break;
        };
      };
      if(hasFound == True){
        hasFound = False;
        for(line2 = line1 ;line2 <= lineRight;line2++){
          if(line2 >= MAXCOL - LINESTEP && image[row][line2] < whiteValue[i] ||image[row][line2+LINESTEP] >= image[row][line2]+blackValueStep[i]){
	    hasFound = True;
	    break;
          };
        };
      };
     
     if(hasFound == True && line2-line1+1 >= minBlackWidth[i]){
	tmpPoint = (line1 + line2) / 2; 
	hasGotLine = True;
	///////   找到距离更近的黑线  ///////////////////
	if (abs(tmpPoint-blackLine[i]) < bestDistance){
	  bestDistance = abs(tmpPoint - blackLine[i]);
	  bestPoint = tmpPoint;
	};
        line1 = line2 + 1;
      } else {
        line1 = line1 + 1;
      };
    };

    /////////////  判断是否全为黑线 ///////////////////////////
    isAllBlack = True;
    for(line1 = lineLeft;line1<=lineRight;line1++)
      if(image[row][line1] > whiteValue[i]){
	isAllBlack = False;
	break;
      };
    if(isAllBlack == True){
      hasGotLine = True;
      bestPoint = blackLine[i];
    };

    ////////////  判断是否已经丢线 ////////////////////////////
    if(hasGotLine == True){
      forwardDistance = i;
      blackLine[i] = bestPoint;
      missedCnt = 0;
    } else {
      missedCnt++;
      if (missedCnt >= MAXMISSCNT)
        break;               ///  跳出，不再搜索黑线 
    };
  };

  while(blackLine[forwardDistance] == 0 && blackLine[forwardDistance -1] == 0)
    forwardDistance--;
  /////   保存数据 ///////////////
  for(i = 0;i<MAXBASELINE;i++){
    preBlackLine[i] = blackLine[i];
  };
};

void GetSituation(){
  unsigned char i,row,line1,line2,leftCnt,rightCnt,startLineCnt;
  unsigned char hasFound;

  ////////////////////////////  起始线停车部分  ////////////////////////////////////
  if(startDelayCnt >0) {
    startDelayCnt--;
  } else {
    if (overDelayCnt > 0){
      overDelayCnt--;
      if(overDelayCnt == 0) hasOvered = True;
      else hasOvered = False;
    } else {
      startLineCnt = 0;
      for(i = 0;i< 20;i++){
	leftCnt = rightCnt = 0;
	hasFound = False;
	row = MAXROW - i -1;
	/////////////////////////       起始线判断  //////////////////////////////////
	for(line1=blackLine[i]-STARTLINESTEP;line1>0;line1--){
	  if (image[row][line1+LINESTEP] >= image[row][line1] + blackValueStep[i]){
	    hasFound = True;
	    break;
	  }; 
	};
	if(hasFound == True){
	  hasFound = False;

   	  for(line2=line1;line2>LINESTEP;line2--){
	    if (image[row][line2-LINESTEP] >= image[row][line2] + blackValueStep[i]){
	      hasFound = True;
	      break;
	    }; 
	  };          

	  if(hasFound == True){
	    hasFound = False;
	    leftCnt = line1 - line2;

	    if (leftCnt >= STARTLINELENGTH){
	      /////////////////////   已经找到左边的线，寻找右边的线  //////////////////////////////

   	      for(line1=blackLine[i]+STARTLINESTEP;line1<MAXCOL;line1++){
	        if (image[row][line1-LINESTEP] >= image[row][line1] + blackValueStep[i]){
	          hasFound = True;
	          break;
	        }; 
	      };
	      if(hasFound == True){
	        hasFound = False;

   	        for(line2=line1;line2+LINESTEP < MAXCOL;line2++){
	          if (image[row][line2+LINESTEP] >= image[row][line2] + blackValueStep[i]){
	            hasFound = True;
	            break;
	          }; 
	        };
		if(hasFound == True){
		  rightCnt = line2-line1;
		  if(rightCnt >=STARTLINELENGTH){
		    ///////////////////////  找到右边的线 ////////////////////////////////////
		    if(abs(leftCnt-rightCnt)<=STARTLINEERR)
		      startLineCnt++;
		  }; 
		};
	      };
	    };
	  };
	};
      };
      ///////////////////////////////////////////////////////////////////////////////
      if(startLineCnt >= 2){
	hasStartLine = True; 
	overDelayCnt = OVERDELAY;  
      };
    };
  }; 
  //////////////////////////////  起始线停车部分  //////////////////////////////////////
  //////////////////////////////  判断是否丢线   /////////////////////////////////////
  if(hasMissed == True){
  };
  ///////////////////////////////////////////////////////////////////////////////////
};

