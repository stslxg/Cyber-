/** ###################################################################
**     Filename  : ui.H
**     Project   : CyberNT3
**     Processor : 56F8366
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011/8/11, 12:36
**     Contents  :
**         User source code
**
** ###################################################################*/

#ifndef __ui_H
#define __ui_H

/* MODULE ui */

#include "world.h"

typedef U8 UI_READ_CALLBACK(U8 key);
typedef void UI_WRITE_CALLBACK(U8 key, U8 value);

typedef struct _UI_ENTRY {
	UI_READ_CALLBACK *readCallback;
	UI_WRITE_CALLBACK *writeCallback;
	U8 lowLimit;
	U8 highLimit;
} UI_ENTRY;

#define UI_ENTRY_LIMIT (100)

typedef struct _UI_OBJECT {
	UI_ENTRY entry[UI_ENTRY_LIMIT];
	U8 lowLimit;
	U8 highLimit;
} UI_OBJECT;

extern void UiInitializeObject(UI_OBJECT *object);
extern void UiRegisterEntryObject(UI_OBJECT *object, U8 index,
	UI_READ_CALLBACK *readCallback, UI_WRITE_CALLBACK *writeCallback,
	U8 lowLimit, U8 highLimit);
extern void UiPromptObject(UI_OBJECT *object);

/* END ui */

#endif

