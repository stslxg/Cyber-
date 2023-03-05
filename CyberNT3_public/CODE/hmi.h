/** ###################################################################
**     Filename  : hmi.H
**     Project   : CyberNT3
**     Processor : 56F8366
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011/8/10, 10:00
**     Contents  :
**         User source code
**
** ###################################################################*/

#ifndef __hmi_H
#define __hmi_H

/* MODULE hmi */

#include "world.h"

#define HMI_KEY_A (0)
#define HMI_KEY_B (1)

typedef void HMI_HANDLER(U8 key);

extern void HmiInitSystem(void);
extern void HmiShowNumberEx(U16 number, U8 mask);
#define HmiShowNumber(number) (HmiShowNumberEx((number), 0))
extern HMI_HANDLER *HmiAttachHandler(HMI_HANDLER *handler);
#define HmiDetachHandler(old) (void)HmiAttachHandler((old))
extern void HmiQueryKeyStatus(U8 status[2]);
extern U8 HmiWaitForSingleKey(void);
#define HmiEnableBuzz() HMI_BUZZ_SetRatio16(0xffffU)
#define HmiDisableBuzz() HMI_BUZZ_SetRatio16(0x0U)
extern void HmiIrqHandler(void);

/* END hmi */

#endif

