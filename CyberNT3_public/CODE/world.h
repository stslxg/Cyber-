/** ###################################################################
**     Filename  : world.H
**     Project   : CyberNT3
**     Processor : 56F8366
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011/8/10, 10:01
**     Contents  :
**         User source code
**
** ###################################################################*/

#ifndef __world_H
#define __world_H

/* MODULE world */

#include "PE_Types.h"

typedef UInt8 U8;
typedef Int8 S8;
typedef UInt16 U16;
typedef Int16 S16;
typedef UInt32 U32;
typedef Int32 S32;

#define UNREFERENCED_PARAMETER(x) (x)

#include "Events.h"
#include "const.h"
#include "rtl.h"
#include "hmi.h"
#include "ui.h"
#include "config.h"
#include "afe.h"
#include "drv.h"
#include "sci.h"
#include "nt.h"

/* END world */

#endif
