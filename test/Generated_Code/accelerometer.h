/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : accelerometer.h
**     Project     : ProcessorExpert
**     Processor   : MC9S08JM60CLDE
**     Component   : ADC
**     Version     : Component 01.667, Driver 01.25, CPU db: 3.00.048
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2013-05-27, 12:03, # CodeGen: 7
**     Abstract    :
**         This device "ADC" implements an A/D converter,
**         its control methods and interrupt/event handling procedure.
**     Settings    :
**         AD control register         : ADCSC1      [$0010]
**         AD control register         : ADCCFG      [$0016]
**         AD control register         : ADCRH       [$0012]
**         AD control register         : ADCRL       [$0013]
**         AD control register         : ADCCVH      [$0014]
**         AD control register         : ADCCVL      [$0015]
**         AD control register         : ADCSC2      [$0011]
**         AD control register         : APCTL1      [$0017]
**         AD control register         : APCTL2      [$0018]
**         Interrupt name              : Vadc
**         User handling procedure     : accelerometer_OnEnd
**         Number of conversions       : 1
**         AD resolution               : 12-bit
**
**         Input pins
**
**              Port name              : PTB
**              Bit number (in port)   : 5
**              Bit mask of the port   : $0020
**              Port data register     : PTBD        [$0002]
**              Port control register  : PTBDD       [$0003]
**
**              Port name              : PTD
**              Bit number (in port)   : 0
**              Bit mask of the port   : $0001
**              Port data register     : PTDD        [$0006]
**              Port control register  : PTDDD       [$0007]
**
**              Port name              : PTD
**              Bit number (in port)   : 1
**              Bit mask of the port   : $0002
**              Port data register     : PTDD        [$0006]
**              Port control register  : PTDDD       [$0007]
**
**         Initialization:
**              Conversion             : Enabled
**              Event                  : Enabled
**         High speed mode
**             Prescaler               : divide-by-1
**     Contents    :
**         Measure    - byte accelerometer_Measure(bool WaitForResult);
**         GetValue16 - byte accelerometer_GetValue16(word *Values);
**
**     Copyright : 1997 - 2012 Freescale, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef __accelerometer
#define __accelerometer

/* MODULE accelerometer. */

#include "Cpu.h"




byte accelerometer_GetValue16(word *Values);
/*
** ===================================================================
**     Method      :  accelerometer_GetValue16 (component ADC)
**
**     Description :
**         This method returns the last measured values of all
**         channels justified to the left. Compared with <GetValue>
**         method this method returns more accurate result if the
**         <number of conversions> is greater than 1 and <AD
**         resolution> is less than 16 bits. In addition, the user
**         code dependency on <AD resolution> is eliminated.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Values          - Pointer to the array that
**                           contains the measured data.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_NOTAVAIL - Requested value not
**                           available
**                           ERR_OVERRUN - External trigger overrun
**                           flag was detected after last value(s)
**                           was obtained (for example by GetValue).
**                           This error may not be supported on some
**                           CPUs (see generated code).
** ===================================================================
*/

void accelerometer_Init(void);
/*
** ===================================================================
**     Method      :  accelerometer_Init (component ADC)
**
**     Description :
**         Initializes the associated peripheral(s) and the component's 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

byte  accelerometer_Measure(bool WaitForResult);
/*
** ===================================================================
**     Method      :  accelerometer_Measure (component ADC)
**
**     Description :
**         This method performs one measurement on all channels that
**         are set in the component inspector. (Note: If the <number of
**         conversions> is more than one the conversion of A/D
**         channels is performed specified number of times.)
**     Parameters  :
**         NAME            - DESCRIPTION
**         WaitForResult   - Wait for result of
**                           the conversion
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled
**                           ERR_BUSY - A conversion is already
**                           running
** ===================================================================
*/


/*
** ###################################################################
**
**     This file was created by Processor Expert 10.0 [05.03]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
#endif /* ifndef __accelerometer */
