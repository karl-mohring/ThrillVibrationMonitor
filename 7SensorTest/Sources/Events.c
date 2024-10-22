/* ###################################################################
 **     Filename    : Events.c
 **     Project     : ProcessorExpert
 **     Processor   : MC9S08JM60CLHE
 **     Component   : Events
 **     Version     : Driver 01.02
 **     Compiler    : CodeWarrior HCS08 C Compiler
 **     Date/Time   : 2013-07-05, 16:34, # CodeGen: 0
 **     Abstract    :
 **         This is user's event module.
 **         Put your event handler code here.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file Events.c
 ** @version 01.02
 ** @brief
 **         This is user's event module.
 **         Put your event handler code here.
 */
/*!
 **  @addtogroup Events_module Events module documentation
 **  @{
 */
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "Vib_Cont_01.h"

// Number of interrupts before toggling the status light (2000 == 1 second)
const UINT16 FLASH_PERIOD = 2000;
UINT16 ledCounter = 0;

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

	// Toggle the status LED every second
	if (ledCounter < FLASH_PERIOD) {
		ledCounter++;
	} else {
		toggleStatusLED();
		incrementRunningClock();
		ledCounter = 0;
	}
	
	AD1_Measure(TRUE);
	Sample_Accel();
	ReadData();
}

/* END Events */

/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.2 [05.06]
 **     for the Freescale HCS08 series of microcontrollers.
 **
 ** ###################################################################
 */
