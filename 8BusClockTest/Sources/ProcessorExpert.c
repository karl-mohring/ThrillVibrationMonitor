/* ###################################################################
 **     Filename    : ProcessorExpert.c
 **     Project     : ProcessorExpert
 **     Processor   : MC9S08JM60CLHE
 **     Version     : Driver 01.12
 **     Compiler    : CodeWarrior HCS08 C Compiler
 **     Date/Time   : 2013-07-12, 00:10, # CodeGen: 45
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file ProcessorExpert.c
 ** @version 01.12
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup ProcessorExpert_module ProcessorExpert module documentation
 **  @{
 */
/* MODULE ProcessorExpert */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "AD1.h"
#include "sampleTimer.h"
#include "filterTimer.h"
#include "SDSerial.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
const byte MANUAL_SPI = 1;
const byte PE_SPI = 2;
const byte HI_SPEED_MANUAL = 3;
const byte HI_SPEED_PE = 4;

byte testMode = MANUAL_SPI;

void main(void) {
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	/* For example: for(;;) { } */
	switch (testMode) {
	case MANUAL_SPI:
		SPI_Init();

		for (;;) {
			SPI_Send_byte(0xFB);
		}
		break;

	case PE_SPI:
		SDSerial_Enable();

		for (;;) {
			SDSerial_SendChar(0xBA);
		}
		break;
		
	case HI_SPEED_MANUAL:
		SPI_Init();
		SPI_High_rate();
		
		for (;;) {
			SPI_Send_byte(0xFB);
		}
		break;
		
	case HI_SPEED_PE:
		SDSerial_Enable();
		SDSerial_SetBaudRateMode(2);
		for (;;) {
					SDSerial_SendChar(0xBA);
				}
				break;
	}
	
	
	for (;;) {
		pulsePowerLED(1);
		pulseStatusLED(1);
		pulseReceiveLED(1);
		pulseTransmitLED(1);
	}

	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END ProcessorExpert */
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
