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
 **     Event       :  AD1_OnEnd (module Events)
 **
 **     Component   :  AD1 [ADC]
 **     Description :
 **         This event is called after the measurement (which consists
 **         of <1 or more conversions>) is/are finished.
 **         The event is available only when the <Interrupt
 **         service/event> property is enabled.
 **     Parameters  : None
 **     Returns     : Nothing
 ** ===================================================================
 */
void AD1_OnEnd(void) {
	// Interrupts currently disabled for AD1
	//ADC_ISR();
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
void sampleTimer_OnInterrupt(void) {

	// Toggle the status LED every second
	if (ledCounter < FLASH_PERIOD) {
		ledCounter++;
	} else {
		toggleStatusLED();
		incrementRunningClock();
		ledCounter = 0;
	}
	Sample_Accel();
	ReadData();
}

/*
** ===================================================================
**     Event       :  SDSerial_OnRxChar (module Events)
**
**     Component   :  SDSerial [SynchroMaster]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**         
**         [ Version specific information for Freescale HCS08 and RS08
**         and HCS12 and HCS12X and ColdFireV1 derivatives ] 
**         The <Bidirectional mode> property setting doesn't affect
**         this event. For more information please see <Bidirectional
**         mode behavior> paragraph.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SDSerial_OnRxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  SDSerial_OnTxChar (module Events)
**
**     Component   :  SDSerial [SynchroMaster]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SDSerial_OnTxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  SDSerial_OnError (module Events)
**
**     Component   :  SDSerial [SynchroMaster]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void SDSerial_OnError(void)
{
  /* Write your code here ... */
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
