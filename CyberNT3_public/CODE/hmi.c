/** ###################################################################
**     Filename  : hmi.C
**     Project   : CyberNT3
**     Processor : 56F8366
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011/8/10, 10:00
**     Contents  :
**         User source code
**
** ###################################################################*/

/* MODULE hmi */

#include "world.h"

static HMI_HANDLER *volatile HmipHandler = NULL;
static volatile U8 HmipKeyStatus[2] = {0, 0};

static inline U8 HmipSendRecv(U8 high, U8 low)
{
	U8 data;
	HMI_SS_ClrVal();
	HMI_SPI_SendChar(high);
	HMI_SPI_SendChar(low);
	HMI_SS_SetVal();
	HMI_SPI_RecvChar(&data);
	return data;
}

static inline void HmipWriteRegister(U8 address, U8 data)
{
	HmipSendRecv(address, data);
}

static inline U8 HmipReadRegister(U8 address)
{
	HmipSendRecv((U8)(address | 0x80), 0);
	return HmipSendRecv(0, 0);
}

void HmiInitSystem(void)
{
	HmipWriteRegister(0x00, 0x00);
	HmipWriteRegister(0x07, 0x01);
	Cpu_Delay100US(INTERACTIVE_DELAY);
	HmipWriteRegister(0x07, 0x00);
	HmipWriteRegister(0x02, 0x08);
	HmipWriteRegister(0x20, 0x20);
	HmipWriteRegister(0x21, 0x20);
	HmipWriteRegister(0x22, 0x20);
	HmipWriteRegister(0x23, 0x20);
	HmipWriteRegister(0x08, 0xFF);
	HmipWriteRegister(0x09, 0x0F);
	HmipWriteRegister(0x06, 0x5F);
	HmipWriteRegister(0x04, 0x01);
}

void HmiShowNumberEx(U16 number, U8 mask)
{
	U8 reg;
	
	for (reg = 0x23; reg >= 0x20; --reg) {
		HmipWriteRegister(reg, (U8)((mask & 1) ? 0x20 : 0x00));
		mask >>= 1;
	}
	for (reg = 0x2b; reg >= 0x28; --reg) {
		HmipWriteRegister(reg, (U8)((number % 10) | ((mask & 1) ? 0x80 : 0x00)));
		number /= 10;
		mask >>= 1;
	}
}

HMI_HANDLER *HmiAttachHandler(HMI_HANDLER *handler)
{
	HMI_HANDLER *old = HmipHandler;
	HmipHandler = handler;
	return old;
}

void HmiQueryKeyStatus(U8 status[2])
{
	status[0] = HmipKeyStatus[0];
	status[1] = HmipKeyStatus[1];
}

static volatile U8 HmipSingleKey;

static void HmipSingleKeyHandler(U8 key)
{
	HmipSingleKey = key;
}

U8 HmiWaitForSingleKey(void)
{
	HMI_HANDLER *old;
	U8 key;
	
	HmipSingleKey = 0xff;
	old = HmiAttachHandler(&HmipSingleKeyHandler);
	while ((key = HmipSingleKey) == 0xff) {}
	HmiDetachHandler(old);
	return key;
}

void HmiIrqHandler(void)
{
	U16 data[2];
	U8 key;
	HMI_HANDLER *handler = HmipHandler;
	
	if (HMI_KEY_GetValue16(data) == ERR_OK) {
		for (key = 0; key < 2; ++key) {
			if (data[key] < KEY_AD_THRESHOLD) {
				if (HmipKeyStatus[key] == 0) {
					if (handler != NULL) {
						(*handler)(key);
					}
					HmipKeyStatus[key] = 1;
				}
			} else {
				HmipKeyStatus[key] = 0;
			}
		}
	}
	HMI_KEY_Measure(FALSE);
}

/* END hmi */
