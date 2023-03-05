/** ###################################################################
**     Filename  : drv.H
**     Project   : CyberNT3
**     Processor : 56F8366
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011/8/11, 21:34
**     Contents  :
**         User source code
**
** ###################################################################*/

#ifndef __drv_H
#define __drv_H

/* MODULE drv */

#include "world.h"

extern S16 DrvServoMiddle;

extern void DrvEnableMotorControl(void);
extern void DrvDisableMotorControl(void);
extern void DrvSetSpeedMotor(S16 left, S16 right);
extern S16 DrvQueryCurrentSpeed(void);
extern void DrvEnableServoControl(void);
extern void DrvDisableServoControl(void);
extern void DrvSetValueServo(S16 value);
extern void DrvEmitPulseServo(S16 value);
extern void DrvIrqHandler(void);

/* END drv */

#endif

