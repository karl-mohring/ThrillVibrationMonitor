/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : powerLED.h
**     Project     : ProcessorExpert
**     Processor   : MC9S08JM60CLHE
**     Component   : BitIO
**     Version     : Component 02.086, Driver 03.27, CPU db: 3.00.050
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2013-05-28, 00:33, # CodeGen: 7
**     Abstract    :
**         This component "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Methods of this component are mostly implemented as a macros
**         (if supported by target language and compiler).
**     Settings    :
**         Used pin                    :
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       27            |  PTG1_KBIP1
**             ----------------------------------------------------
**
**         Port name                   : PTG
**
**         Bit number (in port)        : 1
**         Bit mask of the port        : $0002
**
**         Initial direction           : Output (direction can be changed)
**         Safe mode                   : yes
**         Initial output value        : 0
**         Initial pull option         : off
**
**         Port data register          : PTGD      [$000C]
**         Port control register       : PTGDD     [$000D]
**
**         Optimization for            : speed
**     Contents    :
**         SetDir - void powerLED_SetDir(bool Dir);
**         GetVal - bool powerLED_GetVal(void);
**         PutVal - void powerLED_PutVal(bool Val);
**         ClrVal - void powerLED_ClrVal(void);
**         SetVal - void powerLED_SetVal(void);
**
**     Copyright : 1997 - 2013 Freescale Semiconductor, Inc. All Rights Reserved.
**     SOURCE DISTRIBUTION PERMISSIBLE as directed in End User License Agreement.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/
/*!
** @file powerLED.h
** @version 03.27
** @brief
**         This component "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Methods of this component are mostly implemented as a macros
**         (if supported by target language and compiler).
*/         
/*!
**  @addtogroup powerLED_module powerLED module documentation
**  @{
*/         

#ifndef powerLED_H_
#define powerLED_H_

/* MODULE powerLED. */

  /* Including shared modules, which are used in the whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Cpu.h"


/*
** ===================================================================
**     Method      :  powerLED_GetVal (component BitIO)
**     Description :
**         This method returns an input value.
**           a) direction = Input  : reads the input value from the
**                                   pin and returns it
**           b) direction = Output : returns the last written value
**     Parameters  : None
**     Returns     :
**         ---             - Input value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)

** ===================================================================
*/
#define powerLED_GetVal() ( \
    (bool)((getReg8(PTGD) & 0x02U))    /* Return port data */ \
  )

/*
** ===================================================================
**     Method      :  powerLED_PutVal (component BitIO)
**     Description :
**         This method writes the new output value.
**           a) direction = Input  : sets the new output value;
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes the value to the
**                                   appropriate pin
**     Parameters  :
**         NAME       - DESCRIPTION
**         Val             - Output value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)
**     Returns     : Nothing
** ===================================================================
*/
void powerLED_PutVal(bool Val);

/*
** ===================================================================
**     Method      :  powerLED_ClrVal (component BitIO)
**     Description :
**         This method clears (sets to zero) the output value.
**           a) direction = Input  : sets the output value to "0";
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes "0" to the
**                                   appropriate pin
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define powerLED_ClrVal() ( \
    (void)clrReg8Bits(PTGD, 0x02U)     /* PTGD1=0x00U */, \
    (Shadow_PTG &= 0xFDU)              /* Set appropriate bit in shadow variable */ \
  )

/*
** ===================================================================
**     Method      :  powerLED_SetVal (component BitIO)
**     Description :
**         This method sets (sets to one) the output value.
**           a) direction = Input  : sets the output value to "1";
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes "1" to the
**                                   appropriate pin
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
#define powerLED_SetVal() ( \
    (void)setReg8Bits(PTGD, 0x02U)     /* PTGD1=0x01U */, \
    (Shadow_PTG |= 0x02U)              /* Set appropriate bit in shadow variable */ \
  )

/*
** ===================================================================
**     Method      :  powerLED_SetDir (component BitIO)
**     Description :
**         This method sets direction of the component.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Dir        - Direction to set (FALSE or TRUE)
**                      FALSE = Input, TRUE = Output
**     Returns     : Nothing
** ===================================================================
*/
void powerLED_SetDir(bool Dir);



/* END powerLED. */
#endif /* #ifndef __powerLED_H_ */
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