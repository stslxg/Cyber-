/** ###################################################################
**     Filename  : sci.H
**     Project   : CyberNT3
**     Processor : 56F8366
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011/8/11, 15:13
**     Contents  :
**         User source code
**
** ###################################################################*/

#ifndef __sci_H
#define __sci_H

/* MODULE sci */

#include "world.h"

extern void SciIrqHandler(void);
extern U8 SciReadEvent(void);
extern void SciSendByte(U8 data);
extern void SciSendData(U8 *data, U16 size);

/* END sci */

#endif

