/** ###################################################################
**     Filename  : afe.H
**     Project   : CyberNT3
**     Processor : 56F8366
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011/8/11, 14:21
**     Contents  :
**         User source code
**
** ###################################################################*/

#ifndef __afe_H
#define __afe_H

/* MODULE afe */

#include "world.h"

#define ROWS (40)
#define COLS (172)

typedef struct _AFE_FRAME {
	U8 index;
	U8 ready;
	U8 data[ROWS][COLS];
} AFE_FRAME;

extern volatile AFE_FRAME AfeFrameBuffer[2];
extern volatile U8 AfeFrameIndex;

extern void AfeInitSystem(void);
extern void AfeVsyncHandler(void);
extern void AfeHsyncHandler(void);

/* END afe */

#endif

