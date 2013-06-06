/** ###################################################################
 **     Filename    : Events.c
 **     Project     : ProcessorExpert
 **     Processor   : MC9S08JM60CLHE
 **     Component   : Events
 **     Version     : Driver 01.02
 **     Compiler    : CodeWarrior HCS08 C Compiler
 **     Date/Time   : 2013-05-27, 11:04, # CodeGen: 0
 **     Abstract    :
 **         This is user's event module.
 **         Put your event handler code here.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

word tempSample = 0;

/**
 * Toggle the state of the power LED.
 */
void togglePowerLED() {
	bool powerLEDStatus = powerLED_GetVal();

	if (powerLEDStatus) {
		powerLED_ClrVal();
	} else {
		powerLED_SetVal();
	}
}

/*
 ** ===================================================================
 **     Event       :  Accelerometer1_OnEnd (module Events)
 **
 **     Component   :  Accelerometer1 [ADC]
 **     Description :
 **         This event is called after the measurement (which consists
 **         of <1 or more conversions>) is/are finished.
 **         The event is available only when the <Interrupt
 **         service/event> property is enabled.
 **     Parameters  : None
 **     Returns     : Nothing
 ** ===================================================================
 */
void Accelerometer1_OnEnd(void) {
	(void) Accelerometer1_GetValue16(&tempSample);
}

/*
** ===================================================================
**     Event       :  sampleTimer_OnInterrupt (module Events)
**
**     Component   :  sampleTimer [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void sampleTimer_OnInterrupt(void)
{
	togglePowerLED();
}

/* END Events */

/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.0 [05.03]
 **     for the Freescale HCS08 series of microcontrollers.
 **
 ** ###################################################################
 */
