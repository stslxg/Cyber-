
#ifndef _para_h
#define _para_h

#define MAXPARA 22             // 可调参数的个数


union paraUnion{
  struct paraStruct{
    unsigned char mode;                 // 模式
    unsigned char lineStep;             // 检测黑线时隔离的点数
    unsigned char blackValueStep0,blackStepPoint0,blackValueStep1,blackStepPoint1;  // 黑线跳变阈值插值参数
    unsigned char blackValue0,blackValuePoint0,blackValue1,blackValuePoint1;          // 黑线阈值插值参数
    unsigned char firstLineLeft,firstLineRight,firstLineDelta;
    unsigned char midLine;
  } data;
  unsigned char paraTable[MAXPARA + 1];
} paraData;

#define CARMODE paraData.data.mode
#define LINESTEP paraData.data.lineStep
#define FIRST_LEFT paraData.data.firstLineLeft
#define FIRST_RIGHT paraData.data.firstLineRight
#define FIRST_DELTA paraData.data.firstLineDelta
#define MIDLINE paraData.data.midLine

#define PASS asm("NOP")
#define True 1
#define False 0

extern unsigned char hasOvered,hasStartLine;
extern unsigned char overDelayCnt,startDelayCnt;
int speed0,speed1;
#endif
