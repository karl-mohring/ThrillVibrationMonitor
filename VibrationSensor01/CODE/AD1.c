/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : AD1.c
**     Project   : VibrationSensor01
**     Processor : MC9S08JM60CLDE
**     Component : ADC
**     Version   : Component 01.587, Driver 01.29, CPU db: 3.00.046
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 3/12/2011, 2:26 PM
**     Abstract  :
**         This device "ADC" implements an A/D converter,
**         its control methods and interrupt/event handling procedure.
**     Settings  :
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
**         Priority                    : 
**         User handling procedure     : AD1_OnEnd
**         Number of conversions       : 1
**         AD resolution               : 12-bit
**
**         Input pins
**
**              Port name              : PTB
**              Bit number (in port)   : 0
**              Bit mask of the port   : $0001
**              Port data register     : PTBD        [$0002]
**              Port control register  : PTBDD       [$0003]
**
**              Port name              : PTB
**              Bit number (in port)   : 1
**              Bit mask of the port   : $0002
**              Port data register     : PTBD        [$0002]
**              Port control register  : PTBDD       [$0003]
**
**              Port name              : PTB
**              Bit number (in port)   : 2
**              Bit mask of the port   : $0004
**              Port data register     : PTBD        [$0002]
**              Port control register  : PTBDD       [$0003]
**
**         Initialization:
**              Conversion             : Enabled
**              Event                  : Enabled
**         High speed mode
**             Prescaler               : divide-by-1
**     Contents  :
**         MeasureChan - byte AD1_MeasureChan(bool WaitForResult, byte Channel);
**         GetValue16  - byte AD1_GetValue16(word *Values);
**
**     Copyright : 1997 - 2010 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/


/* MODULE AD1. */

#include "Events.h"
#include "AD1.h"



#define STOP            0              /* STOP state           */
#define MEASURE         1              /* MESURE state         */
#define CONTINUOUS      2              /* CONTINUOS state      */
#define SINGLE          3              /* SINGLE state         */

static const  byte Table[3] = {0x01,0x02,0x04};  /* Table of mask constants */

static const  byte Channels[3] = {0x40,0x41,0x42};  /* Contents for the device control register */

static volatile byte OutFlg;           /* Measurement finish flags */
static volatile byte SumChan;          /* Number of measured channels */
static volatile byte ModeFlg;          /* Current state of device */

volatile word AD1_OutV[3];             /* Sum of measured values */





/*
** ===================================================================
**     Method      :  AD1_Interrupt (component ADC)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes event(s) of the component.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
ISR(AD1_Interrupt)
{
  ((TWREG volatile*)(&AD1_OutV[SumChan]))->b.high = ADCRH; /* Save measured value */
  ((TWREG volatile*)(&AD1_OutV[SumChan]))->b.low = ADCRL; /* Save measured value */
    OutFlg |= Table[SumChan];          /* Value of measured channel is available */
    AD1_OnEnd();                       /* Invoke user event */
    ModeFlg = STOP;                    /* Set the device to the stop mode */
}

/*
** ===================================================================
**     Method      :  AD1_HWEnDi (component ADC)
**
**     Description :
**         Enables or disables the peripheral(s) associated with the 
**         component. The method is called automatically as a part of the 
**         Enable and Disable methods and several internal methods.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void AD1_HWEnDi(void)
{
  if (ModeFlg) {                       /* Start or stop measurement? */
    OutFlg &= ~Table[SumChan];         /* Output value isn't available */
    AD1_OutV[SumChan] = 0;             /* Set variable for storing measured values to 0 */
    ADCSC1 = Channels[SumChan];        /* If yes then start the conversion */
  }
}

/*
** ===================================================================
**     Method      :  AD1_MeasureChan (component ADC)
**
**     Description :
**         This method performs measurement on one channel. (Note: If
**         the <number of conversions> is more than one the conversion
**         of the A/D channel is performed specified number of times.)
**     Parameters  :
**         NAME            - DESCRIPTION
**         WaitForResult   - Wait for a result of
**                           conversion. If the <interrupt service> is
**                           disabled and at the same time a <number of
**                           conversions> is greater than 1, the
**                           WaitForResult parameter is ignored and the
**                           method waits for each result every time.
**         Channel         - Channel number. If only one
**                           channel in the component is set this
**                           parameter is ignored, because the parameter
**                           is set inside this method.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled
**                           ERR_BUSY - A conversion is already running
**                           ERR_RANGE - Parameter "Channel" out of range
** ===================================================================
*/
byte AD1_MeasureChan(bool WaitForResult,byte Channel)
{
  if (Channel >= 3) {                  /* Is channel number greater than or equal to 3 */
    return ERR_RANGE;                  /* If yes then error */
  }
  if (ModeFlg != STOP) {               /* Is the device in different mode than "stop"? */
    return ERR_BUSY;                   /* If yes then error */
  }
  ModeFlg = SINGLE;                    /* Set state of device to the measure mode */
  SumChan = Channel;                   /* Set required channel */
  AD1_HWEnDi();                        /* Enable the device */
  if (WaitForResult) {                 /* Is WaitForResult TRUE? */
    while (ModeFlg == SINGLE) {}       /* If yes then wait for end of measurement */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AD1_GetValue16 (component ADC)
**
**     Description :
**         This method returns the last measured values of all channels
**         justified to the left. Compared with <GetValue> method this
**         method returns more accurate result if the <number of
**         conversions> is greater than 1 and <AD resolution> is less
**         than 16 bits. In addition, the user code dependency on <AD
**         resolution> is eliminated.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Values          - Pointer to the array that contains
**                           the measured data.
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_NOTAVAIL - Requested value not
**                           available
**                           ERR_OVERRUN - External trigger overrun flag
**                           was detected after the last value(s) was
**                           obtained (for example by GetValue). This
**                           error may not be supported on some CPUs
**                           (see generated code).
** ===================================================================
*/
byte AD1_GetValue16(word *Values)
{
  if (OutFlg != 0x07) {                /* Is output flag set? */
    return ERR_NOTAVAIL;               /* If no then error */
  }
  Values[0] = (word)((AD1_OutV[0]) << 4); /* Save measured values to the output buffer */
  Values[1] = (word)((AD1_OutV[1]) << 4); /* Save measured values to the output buffer */
  Values[2] = (word)((AD1_OutV[2]) << 4); /* Save measured values to the output buffer */
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  AD1_Init (component ADC)
**
**     Description :
**         Initializes the associated peripheral(s) and the component's 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void AD1_Init(void)
{
  /* ADCSC1: COCO=0,AIEN=0,ADCO=0,ADCH4=1,ADCH3=1,ADCH2=1,ADCH1=1,ADCH0=1 */
  setReg8(ADCSC1, 0x1F);               /* Disable the module */ 
  /* ADCSC2: ADACT=0,ADTRG=0,ACFE=0,ACFGT=0,??=0,??=0,??=0,??=0 */
  setReg8(ADCSC2, 0x00);               /* Disable HW trigger and autocompare */ 
  OutFlg = 0;                          /* No measured value */
  SumChan = 0;
  ModeFlg = STOP;                      /* Device isn't running */
  /* ADCCFG: ADLPC=0,ADIV1=0,ADIV0=0,ADLSMP=0,MODE1=0,MODE0=1,ADICLK1=1,ADICLK0=1 */
  setReg8(ADCCFG, 0x07);               /* Set prescaler bits */ 
}


/* END AD1. */

/*
** ###################################################################
**
**     This file was created by Processor Expert 3.09 [04.41]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/