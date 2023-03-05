/** ###################################################################
**     Filename  : config.C
**     Project   : CyberNT3
**     Processor : 56F8366
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011/8/11, 12:42
**     Contents  :
**         User source code
**
** ###################################################################*/

/* MODULE config */

#include "world.h"

U8 CfgData[100];

static U8 CfgpReadCallback(U8 key)
{
	return CfgData[key];
}

static void CfgpWriteCallback(U8 key, U8 data)
{
	CfgData[key] = data;
}

void CfgMountUiObject(UI_OBJECT *ui)
{
#define MOUNT_ENTRY(idx, dfl, low, high) \
	do { \
		CfgData[idx] = dfl; \
		UiRegisterEntryObject(ui, idx, &CfgpReadCallback, &CfgpWriteCallback, low, high); \
	} while (0)
	
	MOUNT_ENTRY(CFG_CONTROL_LEVEL, 2, 0, 3);
	MOUNT_ENTRY(CFG_STARTLINE_DISTANCE, 20, 0, 100);
	MOUNT_ENTRY(CFG_SPEED_LOW, 26, 0, 100);
	MOUNT_ENTRY(CFG_SPEED_MID, 30, 0, 100);
	MOUNT_ENTRY(CFG_SPEED_HIGH, 80, 0, 100);
	MOUNT_ENTRY(CFG_MIDSPEED_ROW, 25, 0, ROWS);
	MOUNT_ENTRY(CFG_HIGHSPEED_ROW, 38, 0, ROWS);
	MOUNT_ENTRY(CFG_SERVO_ROW_LOW, 20, 0, ROWS);
	MOUNT_ENTRY(CFG_SERVO_ROW_HIGH, 25, 0, ROWS);
	MOUNT_ENTRY(CFG_TRACK_WIDTH, 49, 0, 100);
	MOUNT_ENTRY(CFG_SERVO_P, 40, 0, 100);
	MOUNT_ENTRY(CFG_SERVO_D, 40, 0, 100);
	MOUNT_ENTRY(CFG_TRANSFORM_NEAR, 30, 0, 100);
	MOUNT_ENTRY(CFG_MOTOR_P, 40, 0, 100);
	MOUNT_ENTRY(CFG_MOTOR_I, 30, 0, 100);
	MOUNT_ENTRY(CFG_MOTOR_D, 35, 0, 100);
	MOUNT_ENTRY(CFG_ACC_LM, 0, 0, ROWS);
	MOUNT_ENTRY(CFG_BRAKE_COUNT, 1, 0, 100);
	MOUNT_ENTRY(CFG_SM, 50, 0, 100);
}

/* END config */
