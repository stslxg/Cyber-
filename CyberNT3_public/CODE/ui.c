/** ###################################################################
**     Filename  : ui.C
**     Project   : CyberNT3
**     Processor : 56F8366
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011/8/11, 12:36
**     Contents  :
**         User source code
**
** ###################################################################*/

/* MODULE ui */

#include "world.h"

void UiInitializeObject(UI_OBJECT *object)
{
	object->lowLimit = 0;
	object->highLimit = 0;
}

void UiRegisterEntryObject(UI_OBJECT *object, U8 index,
	UI_READ_CALLBACK *readCallback, UI_WRITE_CALLBACK *writeCallback,
	U8 lowLimit, U8 highLimit)
{
	UI_ENTRY *entry;
	
	if (object->lowLimit == object->highLimit) {
		object->lowLimit = index;
		object->highLimit = (U8)(index + 1);
	} else if (index < object->lowLimit) {
		for (entry = &object->entry[index + 1];
			entry != &object->entry[object->lowLimit];
			++entry)
		{
			entry->readCallback = NULL;
			entry->writeCallback = NULL;
		}
		object->lowLimit = index;
	} else if (index >= object->highLimit) {
		for (entry = &object->entry[object->highLimit];
			entry != &object->entry[index];
			++entry)
		{
			entry->readCallback = NULL;
			entry->writeCallback = NULL;
		}
		object->highLimit = (U8)(index + 1);
	}
	entry = &object->entry[index];
	entry->readCallback = readCallback;
	entry->writeCallback = writeCallback;
	entry->lowLimit = lowLimit;
	entry->highLimit = highLimit;
}

#define UI_KEY_UP (0)
#define UI_KEY_DOWN (1)
#define UI_KEY_CLEAR (2)
#define UI_KEY_SUBMIT (3)

static volatile U8 UipKeyBuffer;

static void UipKeyHandler(U8 key)
{
	UipKeyBuffer = key;
}

static U8 UipWaitForSingleKey(void)
{
	HMI_HANDLER *old;
	U8 key = 0xff, temp;
	U16 pulsData[2];
	U16 pulsData0[2];
	S16 delta;
	
	RtlQueryPulseAccumulator(pulsData0);
	UipKeyBuffer = 0xff;
	old = HmiAttachHandler(&UipKeyHandler);
	
	while (key == 0xff) {
		RtlQueryPulseAccumulator(pulsData);
		delta = (S16)(pulsData[1] - pulsData0[1]);
		temp = UipKeyBuffer;
		if (delta > ROTATE_LIMIT) {
			key = UI_KEY_UP;
		} else if (delta < -ROTATE_LIMIT) {
			key = UI_KEY_DOWN;
		} else if (temp == HMI_KEY_B) {
			key = UI_KEY_CLEAR;
		} else if (temp == HMI_KEY_A) {
			key = UI_KEY_SUBMIT;
		}
	}
	HmiDetachHandler(old);
	return key;
}

void UiPromptObject(UI_OBJECT *object)
{
	U8 key = object->lowLimit, value;
	UI_ENTRY *entry;
	U8 canRead, canWrite;
	
UiPromptObject_Loop:
	entry = &object->entry[key];
	if (key >= object->lowLimit && key < object->highLimit) {
		canRead = (entry->readCallback != NULL);
		canWrite = (entry->writeCallback != NULL);
	} else {
		canRead = 0;
		canWrite = 0;
	}
	
	if (canRead) {
		value = (*(entry->readCallback))(key);
		HmiShowNumberEx((U16)key * 100 + value, 0x40);
	} else {
		HmiShowNumberEx((U16)key * 100, 0x43);
	}
	
	switch (UipWaitForSingleKey()) {
	case UI_KEY_UP:
		if (key + 1 < object->highLimit) {
			++key;
		}
		break;
	case UI_KEY_DOWN:
		if (key > object->lowLimit) {
			--key;
		}
		break;
	case UI_KEY_CLEAR:
		HmiShowNumberEx(0, 0x0f);
		return;
	case UI_KEY_SUBMIT:
		if (!canWrite) {
			break;
		}
		if (entry->lowLimit >= entry->highLimit) {
			value = entry->lowLimit;
			(*(entry->writeCallback))(key, value);
			break;
		}
		if (canRead) {
			if (value < entry->lowLimit) {
				value = entry->lowLimit;
			} else if (value >= entry->highLimit) {
				value = (U8)(entry->highLimit - 1);
			}
		} else {
			value = entry->lowLimit;
		}
		
		while (1) {
			HmiShowNumberEx((U16)key * 100 + value, 0x10);
			switch (UipWaitForSingleKey()) {
			case UI_KEY_UP:
				if (value + 1 < entry->highLimit) {
					++value;
				}
				break;
			case UI_KEY_DOWN:
				if (value > entry->lowLimit) {
					--value;
				}
				break;
			case UI_KEY_CLEAR:
				goto UiPromptObject_Loop;
			case UI_KEY_SUBMIT:
				(*(entry->writeCallback))(key, value);
				goto UiPromptObject_Loop;
			}
		}
		break;
	}
	goto UiPromptObject_Loop;
}

/* END ui */
