/******************************************************************************
*                                                  
*
*  File Name:   Vib_Sensor_01.c
*
*  Project:     Helicopter Vibration measurement
*               Basket Vibration Sensor Board
*               
*                                                                          
*  Description: Main Operating Functions.
*                    - Vibration Sensor Board
*
*  Processor:   MC9S08JM60CLD 
*                                                                                     
*  Assembler:   Codewarrior for HC(S)08 V6.3
*                                            
*  Version:     1.0                                                         
*                                                                                                                                                         
*  Author:      Peter Grabau
*                                                                                       
*  Location:    Townsville                                              
*                                                                                                                  
*                                                  
* UPDATED HISTORY:
*
* REV   YYYY.MM.DD  AUTHOR        DESCRIPTION OF CHANGE
* ---   ----------  ------        --------------------- 
* 1.0   2011.02.24  Peter Grabau  Initial version
*
******************************************************************************/



/* Includes */
#include "FslTypes.h"
#include "Vib_Sensor_01.h"
#include "Cpu.h"           // for Cpu_DisableInt()
#include "IO_Map.h"        // for port operation
#include "AD1.h"           // Processor Expert ADC functions


/******** Operating LED *********************************/
#define OPLED        PTCD_PTCD4     // Data pin
#define _OPLED       PTCDD_PTCDD4   // Data Direction bit

/******** Global Variables ******************************/
UINT8  u8LEDStatus;
UINT16 u16LEDFlash;            // Used to flash the LED
UINT8  u8AccelSamples[6];      // 3 x 16 bit results, MSB first
UINT8  u8SensorSample;         // Sample the accelerometer channels
UINT8  u8Channel;              // The channel that is being sampled

UINT16 u16DummyX_Axis;         // Dummy results
UINT16 u16DummyY_Axis;         // Dummy results
UINT16 u16DummyZ_Axis;         // Dummy results

/***********************************************************************
*
*    VibSensorInit
*
*  Description: Initialises the Vibration Sensor Board
*               Initialise operating LED & turn it OFF.
*                  
*                  
*                  
*
*               Called from main() in VibrationSensor01.c
*
*************************************************************************/
void VibSensorInit(void){

  /* Set Operating LED pin as an output & turn it OFF */
  _OPLED = _OUT;    // DDR to output
  OPLED  = OFF;     // pin high, LED off
  
  /* Initialise LED flashing */
  u8LEDStatus = OFF;
  u16LEDFlash = 0x0000;
  
  u8SensorSample = FALSE;       // don't sample accelerometer channels
  
  SPI1_Init();              // Initialise the SPI port
  
  /* ADC Initialisation done by Processor Expert */
//  ADC_Init();               // Initialise the ADC
  
  /* Initialise the dummy accelerometer results */
  u16DummyX_Axis = 0x0000;
  u16DummyY_Axis = 1000;
  u16DummyZ_Axis = 2000;
  
  
}

/***********************************************************************
*
*    *** Sensor_Sample_Accel ***
*
*  Description: Set the flag "u8SensorSample" to indicate that the three
*                 accelerometer channels should be sampled.
*               This function will be called every 500uSec..
*               Pulses the LED every 2 sec.   
*                  
*                  
*
*               Called from within the IRQ ISR EInt1_OnInterrupt() in Events.c
*
*************************************************************************/
void Sensor_Sample_Accel(void){
   u8SensorSample = TRUE;          // Set flag to indicate a set of samples 
                                   //  should be taken
  
   /* Flash the LED ON for 0.05 sec each 2 sec. */
   if(u8LEDStatus == ON)
   {
     if(u16LEDFlash == 100)        // ON for 0.05 sec?
     {
       OPLED = OFF;                  // Yes! Turn it OFF
       u8LEDStatus = OFF;
       u16LEDFlash = 0x0000;
     }   
   }
   else
   {
     if(u16LEDFlash == 3900)       // OFF for 1.95 sec?
     {
       OPLED = ON;                   // Yes! then turn it ON
       u8LEDStatus = ON;
       u16LEDFlash = 0x0000;
     }   
   }
   u16LEDFlash++;  
      
  
}

/***********************************************************************
*
*    *** Sample_Accel ***
*
*  Description: Sample the three accelerometer channels
*               This function will be called every 500uSec..
*               Pulses the LED every 2 sec.   
*                  
*                  
*
*               Called from EInt1_OnInterrupt() in Events.c
*
*************************************************************************/
void Sample_Accel(void){

   UINT8 adc_result, i = 0;
//   UINT16 count;
   
   /* Sample the Accelerometers */

   /* X-axis */
   u8Channel = 0x00;      // sample channel 0   
   /* Use processor Expert to take the sample */
   /* Single conversion of AD0. Interrupt on completion.*/
   adc_result = AD1_MeasureChan(FALSE, u8Channel);  // Don't wait for sample complete
//   ADCSC1 = 0x40;         // Single conversion of AD0. Interrupt on completion.
   asm
   {
    wait                // CPU in WAIT state during conversion to reduce noise.
                        // CPU exits WAIT state with COCO interrupt
   }
   
   /* Y-axis */
   u8Channel++;
   /* Use processor Expert to take the sample */
   /* Single conversion of AD1. Interrupt on completion.*/
   adc_result = AD1_MeasureChan(FALSE, u8Channel);  // Don't wait for sample complete   
//   ADCSC1 = 0x41;         // Single conversion of AD1. Interrupt on completion.
   asm
   {
    wait
   }
   
   /* Z-axis */
   u8Channel++;
   /* Use processor Expert to take the sample */
   /* Single conversion of AD2. Interrupt on completion.*/
   adc_result = AD1_MeasureChan(FALSE, u8Channel);  // Don't wait for sample complete
//   ADCSC1 = 0x42;         // Single conversion of AD2. Interrupt on completion.
   asm
   {
    wait
   }
   
//   ADCSC1 = 0x5F;         // Disable the ADC module
   


   /* Use dummy values at the moment */
//   u8AccelSamples[0] = (UINT8)(u16DummyX_Axis >> 8);  // X-Axis MSB
//   u8AccelSamples[1] = (UINT8)u16DummyX_Axis;         // X-Axis LSB
//   u16DummyX_Axis++;
//   if(u16DummyX_Axis >= 1000)
//      u16DummyX_Axis = 0000;
   
//   u8AccelSamples[2] = (UINT8)(u16DummyY_Axis >> 8);  // Y-Axis MSB
//   u8AccelSamples[3] = (UINT8)u16DummyY_Axis;         // Y-Axis LSB
//   u16DummyY_Axis++;
//   if(u16DummyY_Axis >= 2000)
//      u16DummyY_Axis = 1000;
  
//   u8AccelSamples[4] = (UINT8)(u16DummyZ_Axis >> 8);  // Z-Axis MSB
//   u8AccelSamples[5] = (UINT8)u16DummyZ_Axis;         // Z-Axis LSB
//   u16DummyZ_Axis++;
//   if(u16DummyZ_Axis >= 3000)
//      u16DummyZ_Axis = 2000;
   
   
  
}

/***********************************************************************
*
*    *** Sensor_Tx_Results ***
*
*  Description: Transmit the accelerometer results to the controller.
*               Program will loop here waiting for an accelerometer 
*               sample to complete & then handle transmission of the   
*               results to the Controller board using the SPI port.   
*               The controller should do two dummy reads first to   
*               flush the transmit data and shift registers.
*               
*
*************************************************************************/
void Sensor_Tx_Results(void){
  UINT8 i, j, u8TxCount = 0;
//  j=0;
  for(;;)
  {
   if(u8SensorSample)             // a new set of samples should be taken
   {
     /*** Sample Accelerometers and transmit the results loop ***/
     u8SensorSample = FALSE;
     
     /* Sample the accelerometers */
     Sample_Accel();             // Sample the three accelerometers                                  
     
     /* Send 2 0x00 */
     for(u8TxCount=0;u8TxCount<2;u8TxCount++)
     {
        while(!SPI1S_SPTEF && (u8SensorSample == FALSE)){}; // Wait for Tx data reg empty
        SPI1DL = 0x00;   // Send byte
     }
     
     /* Send 0xAA */   
        while(!SPI1S_SPTEF && (u8SensorSample == FALSE)){}; // Wait for Tx data reg empty
        SPI1DL = 0xAA;   

     /* Send the results */
     for(u8TxCount=0;u8TxCount<6;u8TxCount++)
     {
        while(!SPI1S_SPTEF && (u8SensorSample == FALSE)){}; // Wait for Tx data reg empty
        SPI1DL = u8AccelSamples[u8TxCount];   // Send next byte
     }
         
   }
   
  }
  
}

/***********************************************************************
*
*    *** Sensor_Tx_Results ***
*
*  Description: Transmit the accelerometer results to the controller.
*               Program will loop here waiting for an accelerometer 
*               sample to complete & then handle transmission of the   
*               results to the Controller board using the SPI port.   
*               The controller should do two dummy reads first to   
*               flush the transmit data and shift registers.
*               
*
*************************************************************************/
void Sensor_Tx_ResultsOld1(void){
  UINT8 i, j, u8TxCount = 0;
//  j=0;
  for(;;)
  {
   if(u8SensorSample)             // a new set of samples ready?
   {
     /* Transmit samples loop */
     u8SensorSample = FALSE;
     OPLED = ON;
     
     /* Wait for two dummy reads to flush the Tx Data & Shift registers */
//     while(!SPI1S_SPRF){};
//     i = SPI1DL;                  // Dummy read of the receive data register
//     while(!SPI1S_SPRF){};
//     j = SPI1DL;                  // Dummy read of the receive data register

     /* Send the results */
     for(u8TxCount=0;u8TxCount<6;u8TxCount++)
     {
        while(!SPI1S_SPTEF){};      // Wait for Tx data reg empty
        SPI1DL = u8AccelSamples[u8TxCount];   // Send next byte
     }
        
       OPLED = OFF;
       i=i+1;
       j=j+1;
     
   }
   
  }
  
}

/***********************************************************************
*
*    *** Sensor_Tx_Results ***
*
*  Description: Transmit the accelerometer results to the controller.
*               Program will loop here waiting for an accelerometer 
*               sample to complete & then handle transmission of the   
*               results to the Controller board using the SPI port.   
*               The controller should do two dummy reads first to   
*               flush the transmit data and shift registers.
*               
*
*************************************************************************/
void Sensor_Tx_ResultsOld(void){
  UINT8 i, j, u8DRxCount;
  j=0;
  for(;;)
  {
   if(u8SensorSample)             // a new set of samples ready?
   {
     /* Transmit samples loop */
     u8SensorSample = FALSE;
     
     /* Wait for two dummy reads to flush the Tx Data & Shift registers */
     u8DRxCount = 0;                    // Counts the dummy reads
     do                                 // loop for two dummy reads
     {
        if(SPI1S_SPRF)                  // byte received?
        {
           OPLED = ON;
           i = SPI1DL;                  // Dummy read of the receive data register
           u8DRxCount++;
           OPLED = OFF;  
        }
     }while ((u8SensorSample == False) && (u8DRxCount < 2));
     
     /* If two dummy sample reads received, then transmit results */
     if(!u8SensorSample)
       {
      
       /* loop until all samples are transmitted or a new sample is available */
       i = 0;                             // Tx byte counter
       do
       {
          (void)SPI1DL;                   // Clear the receive data register
          if(SPI1S_SPTEF)                 // Tx data register empty?
          {
            OPLED = ON;
            SPI1DL = u8AccelSamples[i];   // Send next byte
            while(!SPI1S_SPTEF){};        // Wait till byte goes
              i++;
            if(i==5)
               j++;
            OPLED = OFF;
          }
       } while ((u8SensorSample == FALSE) && (i < 6));
     
       i=i+1;
     }
   }
  }
  
  
}

/************************************************************************
*
*    SPI1_Init
*
* Description: Function initialises of SPI1 to communicate with the 
*              controller board.
*              Uses SPI1.
*              This device is the SPI slave.
*              Use Mode 0 @ 6MHz.
*
*************************************************************************/
void SPI1_Init(void){

  
  /* Setup SPI1 for >= 6MHz operation */
  SOPT2_SPI1FE = 0;                // Disable SPI1 input filters
  PTEDS_PTEDS4 = 1;                // High output drive strength for PTE4/MISO1/
  //PTBDS_PTBDS2 = 1;                //                                PTB2/SPSCK2
  
  
  (void)SPI1S;                         /* Dummy read of the status register */
  (void)SPI1DL;                        /* Dummy read of the data register */
  /* SPI1BR: ??=0,SPPR2=0,SPPR1=0,SPPR0=0,??=0,SPR2=0,SPR1=0,SPR0=1 */
//  SPI1BR = 0x00;                       /* Set the baud rate register for 6MHz with
//                                            a bus clock of 24MHz.
//                                            Prescale = 1, divide = 4 */ 
  /* SPI1C2: SPMIE=0,SPIMODE=0,??=0,MODFEN=0,BIDIROE=0,??=0,SPISWAI=0,SPC0=0 */
  SPI1C2 = 0x00;                      /* Configure the SPI port - control register 2
                                            
                                             */ 
  /* SPI1C1: SPIE=0,SPE=0,SPTIE=0,MSTR=0,CPOL=0,CPHA=0,SSOE=0,LSBFE=0 */
  SPI1C1 = 0x00;               // Configure the SPI port - control register 1
                               //    Slave mode, MSb first 
                               //    CPOL = 0, clock idles low
                               //    CPHA = 0, data sampled on first clk edge (rising) 

 
  SPI1C1_SPE = 1;                      /* Enable device */
  
}

/************************************************************************
*
*    ADC_Init
*
* Description: Function initialises of ADC to sample the three accelerometer  
*              channels.
*              
*              
*              
*
*************************************************************************/
void ADC_Init(void){
  /* Configuration register ADCCFG. */
     ADCCFG = 0x07;         // b7 ADLPC = 0; High speed configuration
                            // b6:5 ADIV = 0:0; Clock divide = 1.
                            // b4 ADLSMP = 0; Short sample time. 
                            //                AD824 has low impedence output.
                            // b3:2 MODE = 0:1; 12 bit mode.
                            // b1:0 Input clock select = 1:1; Asych clock.
                            
  /* Status and control register 2 */
     ADCSC2 = 0x00;         // b6 ADTRG = 0; Software triggered.
                            // b5 ACFE = 0; Compare fn disabled.
                            
  /* Pin control 1 register */
     APCTL1 = 0x07;         // b2:b0 = 1; AD2:AD0 pin I/O ctrl disabled.
     
                                 
  
}

/************************************************************************
*
*    ADC_ISR
*
* Description: ADC interrupt service routine  
*              
*              
*              
*              
*
*************************************************************************/
//interrupt VectorNumber_Vadc void ADC_ISR()
void ADC_ISR(void){
  /* u8Channel tells which channel is being sampled.
     Place the results in u8AccelSamples[] */
  u8AccelSamples[u8Channel << 1] = ADCRH;         // MSB
  u8AccelSamples[(u8Channel << 1) +1] = ADCRL;    // LSB
  
}