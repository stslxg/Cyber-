/** ###################################################################
**     Filename  : rtl.H
**     Project   : CyberNT3
**     Processor : 56F8366
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011/8/11, 12:39
**     Contents  :
**         User source code
**
** ###################################################################*/

#ifndef __rtl_H
#define __rtl_H

/* MODULE rtl */

#include "world.h"

static inline void RtlQueryPulseAccumulator(U16 data[2])
{
	LEFTPA_GetCounterValue(&data[0]);
	RIGHTPA_GetCounterValue(&data[1]);
}

static inline S16 RtlLimitS16(S16 number, S16 min, S16 max)
{
	if (number < min) {
		number = min;
	} else if (number > max) {
		number = max;
	}
	return number;
}

static inline S16 RtlAbsS16(S16 number)
{
	if (number >= 0) {
		return number;
	} else {
		return -number;
	}
}

static inline S16 RtlSquareS16(S16 number)
{
	return number * number;
}

static inline S32 RtlSquareS32(S32 number)
{
	return number * number;
}

static inline S16 RtlScaleS16(S16 x, S16 l0, S16 h0, S16 l, S16 h)
{
	return (S16)(((S32)x - l0) * ((S32)h - l) / ((S32)h0 - l0) + l);
}

/* END rtl */

#endif

