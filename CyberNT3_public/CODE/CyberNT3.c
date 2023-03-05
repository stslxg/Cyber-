/** ###################################################################
**     Filename  : CyberNT3.C
**     Project   : CyberNT3
**     Processor : 56F8366
**     Version   : Driver 01.14
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011/8/10, 9:56
**     Abstract  :
**         Main module.
**         This module contains user's application code.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/
/* MODULE CyberNT3 */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "DRV_LH.h"
#include "DRV_LL.h"
#include "DRV_RH.h"
#include "DRV_RL.h"
#include "DRV_BYTE.h"
#include "DRV_PS.h"
#include "AFE_I2C.h"
#include "Inhr4.h"
#include "Inhr5.h"
#include "AFE_FRST.h"
#include "AFE_HSYNC.h"
#include "AFE_VSYNC.h"
#include "PID.h"
#include "BATT.h"
#include "MFR.h"
#include "HMI_SS.h"
#include "HMI_SPI.h"
#include "Inhr1.h"
#include "Inhr2.h"
#include "Inhr3.h"
#include "HMI_BUZZ.h"
#include "HMI_KEY.h"
#include "TMR.h"
#include "RIGHTPA.h"
#include "LEFTPA.h"
#include "AS.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "world.h"

static UI_OBJECT ui;

static U8 BatteryTest(U8 dummy)
{
	U16 x;
	UNREFERENCED_PARAMETER(dummy);
	
	if (BATT_Measure(TRUE) == ERR_OK
		&& BATT_GetValue16(&x) == ERR_OK) {
		return (U8)(((U32)x * 100) >> 16);
	} else {
		return 99;
	}
}

static void ServoTest(U8 dummy0, U8 dummy1)
{
	U8 key[2];
	U16 servo = SERVO_MIDDLE;
	
	UNREFERENCED_PARAMETER(dummy0);
	UNREFERENCED_PARAMETER(dummy1);
	
	DrvServoMiddle = SERVO_MIDDLE;
	DrvEnableServoControl();
	while (1) {
		HmiShowNumber(servo / 8);
		HmiQueryKeyStatus(key);
		if (key[HMI_KEY_A]) {
			if (servo != SERVO_MIDDLE + SERVO_RANGE) {
				servo += 8;
			}
		} else if (key[HMI_KEY_B]) {
			if (servo != SERVO_MIDDLE - SERVO_RANGE) {
				servo -= 8;
			}
		}
		DrvSetValueServo((S16)servo - SERVO_MIDDLE);
		Cpu_Delay100US(300);
	}
}

void main(void)
{
	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	DrvEnableServoControl();
	HmiInitSystem();
	AfeInitSystem();
	UiInitializeObject(&ui);
	CfgMountUiObject(&ui);
	UiRegisterEntryObject(&ui, 0, &BatteryTest, NULL, 0, 0);
	UiRegisterEntryObject(&ui, 1, NULL, &ServoTest, 0, 0);
	DrvDisableServoControl();
	
	while (1) {
		UiPromptObject(&ui);
		DrvServoMiddle = SERVO_MIDDLE - 50*8 + CFG(SM)*8;
		DrvEnableServoControl();
		HmiShowNumberEx(3, 0x0e);
		Cpu_Delay100US(INTERACTIVE_DELAY);
		HmiShowNumberEx(2, 0x0e);
		Cpu_Delay100US(INTERACTIVE_DELAY);
		DrvDisableServoControl();
		HmiShowNumberEx(1, 0x0e);
		Cpu_Delay100US(INTERACTIVE_DELAY);
		NtSystemEntry();
	}
	
	for(;;) {}
}

/* END CyberNT3 */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.00 [04.35]
**     for the Freescale 56800 series of microcontrollers.
**
** ###################################################################
*/
