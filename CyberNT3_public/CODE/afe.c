/** ###################################################################
**     Filename  : afe.C
**     Project   : CyberNT3
**     Processor : 56F8366
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011/8/11, 14:21
**     Contents  :
**         User source code
**
** ###################################################################*/

/* MODULE afe */

#include "world.h"

volatile AFE_FRAME AfeFrameBuffer[2];
volatile U8 AfeFrameIndex;

static U8 AfepSlaveAddress = 0x5c;
static U16 AfepRowCounter;

static inline void AfepSwitchSlave(void)
{
	AfepSlaveAddress = (U8)(AfepSlaveAddress == 0x5c ? 0x5d : 0x5c);
	AFE_I2C_SelectSlave(AfepSlaveAddress);
}

static void AfepWriteRegister(U8 reg, U8 data)
{
	U8 buffer[2];
	U16 snd;
	
	buffer[0] = reg;
	buffer[1] = data;
	
	while (1) {
		if (AFE_I2C_SendBlock(buffer, 2, &snd) != ERR_OK) {
			AfepSwitchSlave();
			continue;
		}
		
		return;
	}
}

void AfeInitSystem(void)
{
	// reset
	AfepWriteRegister(0xe8, 0x02);
	AfepWriteRegister(0xe9, 0x00);
	AfepWriteRegister(0xea, 0x80);
	AfepWriteRegister(0xe0, 0x01);
	AfepWriteRegister(0xe8, 0x60);
	AfepWriteRegister(0xe9, 0x00);
	AfepWriteRegister(0xea, 0xb0);
	AfepWriteRegister(0xe0, 0x01);
	AfepWriteRegister(0xe0, 0x00);
	AfepWriteRegister(0x03, 0x01);
	AfepWriteRegister(0x03, 0x00);
	
	// input/output format
	AfepWriteRegister(0x00, 0x00);
	AfepWriteRegister(0x32, 0x00);
	AfepWriteRegister(0x33, 0x41);
	AfepWriteRegister(0x34, 0x11);
	AfepWriteRegister(0x35, 0xee);
	AfepWriteRegister(0x36, 0xaf);
	
	// events
	AFE_VSYNC_Enable();
	AFE_HSYNC_Enable();
}

void AfeVsyncHandler(void)
{
	U8 index = AfeFrameIndex;
	AfeFrameBuffer[index & 1].ready = 0xff;
	++index;
	AfeFrameBuffer[index & 1].index = index;
	AfeFrameBuffer[index & 1].ready = 0;
	AfepRowCounter = 0;
	AfeFrameIndex = index;
}

void AfeHsyncHandler(void)
{
	volatile AFE_FRAME *frame;
	volatile U8 row;
	volatile U8 *p, *q;
	
	if (AfepRowCounter >= 90 && AfepRowCounter < 290 && AfepRowCounter % 5 == 0) {
		frame = &AfeFrameBuffer[AfeFrameIndex & 1];
		row = (U8)((AfepRowCounter - 90) / 5);
		p = &frame->data[row][0];
		q = p + COLS;
		
		// ignore 6 black pixels
		*(volatile U8 *)0x10000;
		*(volatile U8 *)0x10000;
		*(volatile U8 *)0x10000;
		*(volatile U8 *)0x10000;
		*(volatile U8 *)0x10000;
		*(volatile U8 *)0x10000;
		
		while (p != q) {
			*p++ = *(volatile U8 *)0x10000;
			*p++ = *(volatile U8 *)0x10000;
			*p++ = *(volatile U8 *)0x10000;
			*p++ = *(volatile U8 *)0x10000;
		}
		
		frame->ready = row;
	}
	
	AFE_FRST_ClrVal();
	++AfepRowCounter;
	AFE_FRST_SetVal();
}

/* END afe */
