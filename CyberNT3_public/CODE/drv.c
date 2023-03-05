/** ###################################################################
**     Filename  : drv.C
**     Project   : CyberNT3
**     Processor : 56F8366
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011/8/11, 21:34
**     Contents  :
**         User source code
**
** ###################################################################*/

/* MODULE drv */

#include "world.h"

#define HISTORY_MAX (20)

typedef struct _DRV_MOTOR_CONTEXT {
	S16 expect;
	S16 speed;
	U16 index;
	U16 history[HISTORY_MAX];
	mc_sPIDparams param;
} DRV_MOTOR_CONTEXT;

S16 DrvServoMiddle = SERVO_MIDDLE;

static volatile U8 DrvpMotorEnabled = 0;
static volatile U8 DrvpServoEnabled = 0;
static DRV_MOTOR_CONTEXT DrvpMotorContext[2];
static S16 DrvpServoValue = 0;
static U16 DrvpServoCounter = 0;

static inline void DrvpSetPwmMotor(S16 left, S16 right)
{
	U16 l = (U16)left + 32768U;
	U16 r = (U16)right + 32768U;
	DRV_LH_SetRatio16(l);
	DRV_LL_SetRatio16(l);
	DRV_RH_SetRatio16(r);
	DRV_RL_SetRatio16(r);
}

static inline void DrvpInitializePidController(mc_sPIDparams *param)
{
	param->ProportionalGain = CFG(MOTOR_P) << 4;
	param->ProportionalGainScale = 0;
	param->IntegralGain = CFG(MOTOR_I);
	param->IntegralGainScale = 4;
	param->DerivativeGain = CFG(MOTOR_D) << 3;
	param->DerivativeGainScale = 0;
	param->PositivePIDLimit = 32767;
	param->NegativePIDLimit = -32768;
	param->IntegralPortionK_1 = 0;
	param->InputErrorK_1 = 0;
}

void DrvEnableMotorControl(void)
{
	DRV_MOTOR_CONTEXT *mc;
	U16 data[2];
	U16 i, j;
	
	RtlQueryPulseAccumulator(data);
	
	for (i = 0; i < 2; ++i) {
		mc = &DrvpMotorContext[i];
		mc->expect = 0;
		mc->speed = 0;
		mc->index = 0;
		for (j = 0; j < HISTORY_MAX; ++j) {
			mc->history[j] = data[i];
		}
		DrvpInitializePidController(&mc->param);
	}
	
	DrvpSetPwmMotor(0, 0);
	DRV_BYTE_PutVal(0xFF);
	DrvpMotorEnabled = 1;
}

void DrvDisableMotorControl(void)
{
	DRV_BYTE_PutVal(0x0);
	DrvpMotorEnabled = 0;
}

void DrvSetSpeedMotor(S16 left, S16 right)
{
	DrvpMotorContext[0].expect = left;
	DrvpMotorContext[1].expect = right;
}

S16 DrvQueryCurrentSpeed(void)
{
	if (DrvpMotorContext[0].speed >= DrvpMotorContext[1].speed) {
		return DrvpMotorContext[0].speed;
	} else {
		return DrvpMotorContext[1].speed;
	}
}

void DrvEnableServoControl(void)
{
	DrvpServoValue = 0;
	DrvpServoCounter = 0;
	DrvpServoEnabled = 1;
}

void DrvDisableServoControl(void)
{
	DrvpServoEnabled = 0;
}

void DrvSetValueServo(S16 value)
{
	DrvpServoValue = RtlLimitS16(value, -SERVO_RANGE, SERVO_RANGE);
}

void DrvEmitPulseServo(S16 value)
{
	DRV_PS_SetClockTicks16((U16)(DrvServoMiddle + value));
	DRV_PS_Enable();
}

static inline void DrvpUpdateCurrentSpeed(U16 index, U16 current)
{
	DRV_MOTOR_CONTEXT *mc = &DrvpMotorContext[index];
	mc->speed = (S16)(current - mc->history[mc->index]);
	mc->history[mc->index] = current;
	if (++mc->index >= HISTORY_MAX) {
		mc->index = 0;
	}
}

static inline void DrvpMotorControl(void)
{
	DRV_MOTOR_CONTEXT *mc;
	U16 current[2];
	S16 pwm[2];
	
	RtlQueryPulseAccumulator(current);

#define ACTION(idx) \
	do { \
		mc = &DrvpMotorContext[(idx)]; \
		mc->speed = (S16)(current[(idx)] - mc->history[mc->index]); \
		mc->history[mc->index] = current[(idx)]; \
		if (++mc->index >= HISTORY_MAX) { \
			mc->index = 0; \
		} \
		pwm[(idx)] = PID_controllerPIDtype1(mc->expect, mc->speed, &mc->param); \
	} while (0)
	
	ACTION(0);
	ACTION(1);
	
	DrvpSetPwmMotor(pwm[0], pwm[1]);
}

static inline void DrvpServoControl(void)
{
	if (++DrvpServoCounter >= SERVO_FREQUENCY) {
		DrvEmitPulseServo(DrvpServoValue);
		DrvpServoCounter = 0;
	}
}

void DrvIrqHandler(void)
{
	if (DrvpServoEnabled) {
		DrvpServoControl();
	}

	if (DrvpMotorEnabled) {
		DrvpMotorControl();
	}
}

/* END drv */
