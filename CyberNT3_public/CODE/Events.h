/** ###################################################################
**     Filename  : Events.H
**     Project   : CyberNT3
**     Processor : 56F8366
**     Component : Events
**     Version   : Driver 01.03
**     Compiler  : Metrowerks DSP C Compiler
**     Date/Time : 2011/8/10, 9:56
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PE_Timer.h"
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


void HMI_KEY_OnLowLimit(void);
/*
** ===================================================================
**     Event       :  HMI_KEY_OnLowLimit (module Events)
**
**     Component   :  HMI_KEY [ADC]
**     Description :
**         This event is called when the low limit any channel has been
**         exceeded. If the <Number of conversions> property is greater
**         than 1, then during one measurement this event may be
**         invoked up to the <Number of conversions> times per each of
**         the measured channels. The event is available only when the
**         <Interrupt service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void HMI_KEY_OnZeroCrossing(void);
/*
** ===================================================================
**     Event       :  HMI_KEY_OnZeroCrossing (module Events)
**
**     Component   :  HMI_KEY [ADC]
**     Description :
**         This event is called when the Zero crossing on any channel
**         has occurred. The Zero crossing means that the sign of the
**         result has been changed in the selected direction by <Zero
**         crossing> property. If the <Number of conversions> property
**         is greater than 1, then during one measurement this event
**         may be invoked more than once per each of the measured
**         channels.
**         Version specific information for 56800 derivatives ] 
**         This event is available only when the <Interrupt
**         service/event> property is enabled and <MeasureChan> and
**         <EnableIntChanTrigger> methods are disabled - "don't
**         generate code".
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void TMR_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  TMR_OnInterrupt (module Events)
**
**     Component   :  TMR [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void AFE_VSYNC_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  AFE_VSYNC_OnInterrupt (module Events)
**
**     Component   :  AFE_VSYNC [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void AFE_HSYNC_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  AFE_HSYNC_OnInterrupt (module Events)
**
**     Component   :  AFE_HSYNC [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void AS_OnRxChar(void);
/*
** ===================================================================
**     Event       :  AS_OnRxChar (module Events)
**
**     Component   :  AS [AsynchroSerial]
**     Description :
**         This event is called after a correct character is
**         received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the
**         <Receiver> property is enabled or the <SCI output mode>
**         property (if supported) is set to Single-wire mode.
**         Version specific information for Freescale 56800
**         derivatives ] 
**         DMA mode:
**         If DMA controller is available on the selected CPU and
**         the receiver is configured to use DMA controller then
**         this event is disabled. Only OnFullRxBuf method can be
**         used in DMA mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END Events */
#endif /* __Events_H*/

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.00 [04.35]
**     for the Freescale 56800 series of microcontrollers.
**
** ###################################################################
*/
