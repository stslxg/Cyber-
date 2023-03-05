// ÍŒÏñŽŠÀíÄ£¿é
#ifndef _image_h
#define _image_h
#define True 1
#define False 0



#define MAXROW 60
#define MAXCOL 90
#define MAXBASELINE 10

const unsigned char lineDelta[MAXROW] = {
  2,2,2,2,2,2,2,2,2,2,
  2,2,2,2,2,2,2,2,2,2,
  3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,3,3,
  4,4,4,4,4,4,4,4,4,4,
  4,4,4,4,4,4,4,4,4,4
};
const unsigned char minBlackWidth[MAXROW] = {
  3,3,3,3,3,3,3,3,3,3,
  2,2,2,2,2,2,2,2,2,2,
  1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1
};

const unsigned char maxBlackWidth[MAXROW] = {
  6,6,6,6,6,6,6,6,6,6,
  5,5,5,5,5,5,5,5,5,5,
  5,5,5,5,5,5,5,5,5,5,
  3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,3,3,3,3
};

unsigned short blackValueStep[MAXROW];
unsigned short whiteValue[MAXROW];

extern unsigned short image[MAXROW][MAXCOL];
unsigned short blackLine[MAXROW];
unsigned char preBlackLine[MAXBASELINE];

unsigned char lineLeft,lineRight;
unsigned char hasStartLine,hasOvered,overDelayCnt,startDelayCnt;
unsigned short hasMissed,forwardBase,forwardDistance;

#define MAXMISSCNT 2

#define STARTLINELENGTH 10
#define STARTLINESTEP 10
#define STARTLINEERR 5
#define STARTDELAY 10
#define OVERDELAY 10
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InitLine();
void GetLine();
void GetPointbyDelta(const unsigned char x,const unsigned char y,const unsigned char delta,unsigned char * newX,unsigned char * newY);
void GetSituation();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
