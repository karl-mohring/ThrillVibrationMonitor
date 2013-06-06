/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : samplingTimer.h
**     Project     : ProcessorExpert
**     Processor   : MC9S08JM60CLDE
**     Component   : TimerInt
**     Version     : Component 02.160, Driver 01.23, CPU db: 3.00.048
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2013-05-27, 12:03, # CodeGen: 7
**     Abstract    :
**         This component "TimerInt" implements a periodic interrupt.
**         When the component and its events are enabled, the "OnInterrupt"
**         event is called periodically with the period that you specify.
**         TimerInt supports also changing the period in runtime.
**         The source of periodic interrupt can be timer compare or reload
**         register or timer-overflow interrupt (of free running counter).
**     Settings    :
**         Timer name                  : TPM2 (16-bit)
**         Compare name                : TPM20
**         Counter shared              : No
**
**         High speed mode
**             Prescaler               : divide-by-1
**             Clock                   : 6000000 Hz
**           Initial period/frequency
**             Xtal ticks              : 6000
**             microseconds            : 500
**             seconds (real)          : 0.0005
**             Hz                      : 2000
**             kHz                     : 2
**
**         Runtime setting             : none
**
**         Initialization:
**              Timer                  : Enabled
**              Events                 : Enabled
**
**         Timer registers
**              Counter                : TPM2CNT   [$0061]
**              Mode                   : TPM2SC    [$0060]
**              Run                    : TPM2SC    [$0060]
**              Prescaler              : TPM2SC    [$0060]
**
**         Compare registers
**              Compare                : TPM2C0V   [$0066]
**
**         Flip-flop registers
**              Mode                   : TPM2C0SC  [$0065]
**     Contents    :
**         No public methods
**
**     Copyright : 1997 - 2012 Freescale, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef __samplingTimer
#define __samplingTimer

/* MODULE samplingTimer. */

/*Include shared modules, which are used for whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PE_Cnvrt.h"
#include "Cpu.h"

__interrupt void samplingTimer_Interrupt(void);
/*
** ===================================================================
**     Method      :  samplingTimer_Interrupt (component TimerInt)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes the component event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

void samplingTimer_Init(void);
/*
** ===================================================================
**     Method      :  samplingTimer_Init (component TimerInt)
**
**     Description :
**         Initializes the associated peripheral(s) and the component 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/


/* END samplingTimer. */

#endif /* ifndef __samplingTimer */
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.0 [05.03]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/