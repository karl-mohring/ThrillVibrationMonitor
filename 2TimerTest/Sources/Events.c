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
void sampleTimer_OnInterrupt(void) {
	bool powerLEDStatus = powerLED_GetVal();

	// If the LED is off (true), switch it on (false)
	if (powerLEDStatus) {
		powerLED_ClrVal();
	} else {
		powerLED_SetVal();
	}

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
