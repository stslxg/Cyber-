/** ###################################################################
**     Filename  : sci.C
**     Project   : CyberNT3
**     Processor : 56F8366
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011/8/11, 15:13
**     Contents  :
**         User source code
**
** ###################################################################*/

/* MODULE sci */

#include "world.h"

static volatile U8 ScipEventBuffer = 0;

void SciIrqHandler(void)
{
	U8 c;
	
	while (AS_RecvChar(&c) == ERR_OK) {
		ScipEventBuffer = c;
	}
}

U8 SciReadEvent(void)
{
	U8 result = ScipEventBuffer;
	ScipEventBuffer = 0;
	return result;
}

void SciSendByte(U8 data)
{
	while (AS_SendChar(data) != ERR_OK) { /* spin */ }
}

void SciSendData(U8 *data, U16 size)
{
	U16 snd;
	
	while (size != 0) {
		snd = 0;
		if (AS_SendBlock(data, size, &snd) == ERR_OK) {
			data += snd;
			size -= snd;
		}
	}
}

/* END sci */
