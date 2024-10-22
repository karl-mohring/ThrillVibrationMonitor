/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : EInt1.h
**     Project   : VibrationSensor01
**     Processor : MC9S08JM60CLDE
**     Component : ExtInt
**     Version   : Component 02.094, Driver 01.22, CPU db: 3.00.046
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 2/23/2011, 10:29 PM
**     Abstract  :
**         This component "ExtInt" implements an external 
**         interrupt, its control methods and interrupt/event 
**         handling procedure.
**         The bean uses one pin which generates interrupt on 
**         selected edge.
**     Settings  :
**         Interrupt name              : Virq
**         User handling procedure     : EInt1_OnInterrupt
**
**         Used pin                    :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       2             |  IRQ_TPMCLK
**             ----------------------------------------------------
**
**         Port name                   : IRQinput
**
**         Bit number (in port)        : 0
**         Bit mask of the port        : $0001
**
**         Signal edge/level           : falling
**         Priority                    : 
**         Pull option                 : off
**         Initial state               : Enabled
**
**         Edge register               : IRQSC     [$001B]
**         Enable register             : IRQSC     [$001B]
**         Request register            : IRQSC     [$001B]
**
**         Port data register          : IRQSC     [$001B]
**     Contents  :
**         GetVal - bool EInt1_GetVal(void);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

#ifndef __EInt1_H
#define __EInt1_H

/* MODULE EInt1. */



/*Including shared modules, which are used in the whole project*/
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Events.h"
#include "Cpu.h"


__interrupt void EInt1_Interrupt(void);
/*
** ===================================================================
**     Method      :  EInt1_Interrupt (component ExtInt)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes the components event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

bool EInt1_GetVal(void);

/*
** ===================================================================
**     Method      :  EInt1_GetVal (component ExtInt)
**
**     Description :
**         Returns the actual value of the input pin of the component.
**     Parameters  : None
**     Returns     :
**         ---             - Returned input value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)
** ===================================================================
*/
#endif /* __EInt1_H*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
