/** ###################################################################
**     Filename  : config.H
**     Project   : CyberNT3
**     Processor : 56F8366
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011/8/11, 12:42
**     Contents  :
**         User source code
**
** ###################################################################*/

#ifndef __config_H
#define __config_H

/* MODULE config */

#include "world.h"

#define CFG_CONTROL_LEVEL (2)
#define CFG_STARTLINE_DISTANCE (3)
#define CFG_SPEED_LOW (5)
#define CFG_SPEED_MID (6)
#define CFG_SPEED_HIGH (7)
#define CFG_MIDSPEED_ROW (8)
#define CFG_HIGHSPEED_ROW (9)
#define CFG_SERVO_ROW_LOW (10)
#define CFG_SERVO_ROW_HIGH (11)
#define CFG_TRACK_WIDTH (12)
#define CFG_SERVO_P (15)
#define CFG_SERVO_D (16)
#define CFG_TRANSFORM_NEAR (17)
#define CFG_MOTOR_P (20)
#define CFG_MOTOR_I (21)
#define CFG_MOTOR_D (22)
#define CFG_ACC_LM (25)
#define CFG_BRAKE_COUNT (26)
#define CFG_SM (29)

extern U8 CfgData[100];
extern void CfgMountUiObject(UI_OBJECT *ui);

#define CFG(x) ((S16)CfgData[CFG_##x])

/* END config */

#endif

