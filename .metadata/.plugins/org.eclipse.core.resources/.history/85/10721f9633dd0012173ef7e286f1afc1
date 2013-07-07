/******************************************************************************
*                                                  
*
*  File Name:   SPI.c
*
*  Project:     Helicopter Vibration measurement
*               Basket Vibration Controller Board
*               SPI2 interface for controlling the SD Card.
*                                                                          
*  Description: SD Card using SPI Driver
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
* 1.0   2011.01.14  Peter Grabau  Initial version
*
******************************************************************************/
/*****************************************************************************
*
**  Setup:
**
***         Serial channel              : SPI2
**
**         Protocol
**             Init baud rate          : 375kHz (low speed)
**             Clock edge              : falling
**             Width                   : 8 bits
**             Empty character         : 0
**             Empty char. on input    : RECEIVED
**
**         Registers
**             Input buffer            : SPI2DL    [$0075]
**             Output buffer           : SPI2DL    [$0075]
**             Control register        : SPI2C1    [$0070]
**             Mode register           : SPI2C2    [$0071]
**             Baud setting reg.       : SPI2BR    [$0072]
**
**
**
**         Used pins                   :
**         ----------------------------------------------------------
**              Function    | On package |    Name
**         ----------------------------------------------------------
**               Input      |     23     |  PTB0_MISO2_ADP0
**               Output     |     24     |  PTB1_MOSI2_ADP1
**               Clock      |     25     |  PTB2_SPSCK2_ADP2
**         ----------------------------------------------------------

* 
******************************************************************************/ 

#include "SPI.h"

/************************************************************************
*
*    SPI_Init
*
* Description: Function does a low speed (375kHz) initialisation of SPI2
*              which is required to allow the SD Card initialisation.
*
*************************************************************************/
void SPI_Init(void){

  /* Initialise SD Card Chip Select (CS - pin 1 on the SD Card).
     PTB3 is used as the SPI2 Slave Select (software driven) and is
     connected to the SD Card CS. */
  SPI_SS = DISABLE;       // SD Card disabled (CS high)
  _SPI_SS= _OUT;           // PTB3 as an output
  
  
  /* Setup SPI2 for > 6MHz operation */
  SOPT2_SPI2FE = 0;                // Disable SPI2 input filters
  PTBDS_PTBDS1 = 1;                // High output drive strength for PTB1/MOSO2/
  PTBDS_PTBDS2 = 1;                // PTB2/SPSCK2
  
  
//  #pragma MESSAGE DISABLE C4002        /* Disable warning C4002 "Result not used" */
  (void)SPI2S;                         /* Dummy read of the status register */
  (void)SPI2DL;                        /* Dummy read of the device register */
  /* SPI2BR: ??=0,SPPR2=0,SPPR1=0,SPPR0=0,??=0,SPR2=1,SPR1=0,SPR0=1 */
  SPI2BR = 0x05;                       /* Set the baud rate register to the initialisation rate of 375kHz. */ 
  /* SPI2C2: SPMIE=0,SPIMODE=0,??=0,MODFEN=0,BIDIROE=0,??=0,SPISWAI=0,SPC0=0 */
  SPI2C2 = 0x00;                      /* Configure the SPI port - control register 2
                                            CPOL = 0, clock idles low.
                                            CPHA = 0, data sampled on first clk edge */ 
  /* SPI2C1: SPIE=0,SPE=0,SPTIE=0,MSTR=1,CPOL=0,CPHA=0,SSOE=0,LSBFE=0 */
  SPI2C1 = 0x10;               // Configure the SPI port - control register 1
                               //    Master mode, MSb first 
                               //    CPOL = 0, clock idles low
                               //    CPHA = 0, data sampled on first clk edge (rising) 


/* Try Mode 3 */
  /* SPI2C1: SPIE=0,SPE=0,SPTIE=0,MSTR=1,CPOL=1,CPHA=1,SSOE=0,LSBFE=0 */
//  SPI2C1 = 0x1C;               // Configure the SPI port - control register 1
                               //  Master mode, MSb first
                               //  CPOL = 1, clock idles High
                               //  CPHA = 1, data sampled on first clk edge 
 
  SPI2C1_SPE = 1;                      /* Enable device */
  
}

/************************************************************************
*
*    SPI1_Init
*
* Description: Function initialises of SPI1 to communicate with the 
*              sensor board.
*              Uses SPI1.
*              This device is the SPI master.
*              Use Mode 0 @ 6MHz.
*
*************************************************************************/
void SPI1_Init(void){

  
  /* Setup SPI1 for >= 6MHz operation */
  SOPT2_SPI1FE = 0;                // Disable SPI1 input filters
  PTEDS_PTEDS5 = 1;                // High output drive strength for PTE5/MOSI1/
  PTEDS_PTEDS6 = 1;                //                                PTE6/SPSCK1
  
  
  (void)SPI1S;                         /* Dummy read of the status register */
  (void)SPI1DL;                        /* Dummy read of the data register */
  /* SPI1BR: ??=0,SPPR2=0,SPPR1=0,SPPR0=0,??=0,SPR2=0,SPR1=0,SPR0=1 */
  SPI1BR = 0x01;                       /* Set the baud rate register for 6MHz with
                                            a bus clock of 24MHz.
                                            Prescale = 1, divide = 4 */ 
  /* SPI1C2: SPMIE=0,SPIMODE=0,??=0,MODFEN=0,BIDIROE=0,??=0,SPISWAI=0,SPC0=0 */
  SPI1C2 = 0x00;                      /* Configure the SPI port - control register 2
                                            
                                             */ 
  /* SPI1C1: SPIE=0,SPE=0,SPTIE=0,MSTR=1,CPOL=0,CPHA=0,SSOE=0,LSBFE=0 */
  SPI1C1 = 0x10;               // Configure the SPI port - control register 1
                               //    Slave mode, MSb first 
                               //    CPOL = 0, clock idles low
                               //    CPHA = 0, data sampled on first clk edge (rising) 

 
  SPI1C1_SPE = 1;                      /* Enable device */
  
}

/***********************************************************************
*
*    SPI1_Send_byte
*
*  Description: Function transmits a byte from SPI1.
*               It then waits for the transmit buffer to become empty.
*
*************************************************************************/
void SPI1_Send_byte(UINT8 u8Data){
//	(void)SPI1S;               // Dummy read of SPI2S_SPTEF
	while(!SPI1S_SPTEF){};     // Check TDRE empty

	SPI1DL = u8Data;              // Send byte
	while(!SPI1S_SPTEF){};     // Wait till byte goes
}

/***********************************************************************
*
*    SPI_Send_byte
*
*  Description: Function transmits a byte from SPI2.
*               It then waits for the transmit buffer to become empty.
*
*************************************************************************/
void SPI_Send_byte(UINT8 u8Data){
//	(void)SPI2S;               // Dummy read of SPI2S_SPTEF
	while(!SPI2S_SPTEF){};     // Check TDRE empty

	SPI2DL = u8Data;              // Send byte
	while(!SPI2S_SPTEF){};     // Wait till byte goes
}

/***********************************************************************
*
*    SPI1_Receive_byte
*
*  Description: Function receives a byte from SPI1.
*               1. Receive data register is cleared.
*               2. A dummy byte (0xFF) is transmitted.
*               3. When the receive data register is full it is read.
*
*************************************************************************/
UINT8 SPI1_Receive_byte(void){
	(void)SPI1DL;              // Clear the receive data register
	SPI1DL = 0xFF;               // Transmit dummy byte (0xFF)
	while(!SPI1S_SPRF){};	    // Wait for byte to be received.
	return(SPI1DL);            // Read received byte
}

/***********************************************************************
*
*    SPI_Receive_byte
*
*  Description: Function receives a byte from SPI2.
*               1. Receive data register is cleared.
*               2. A dummy byte (0xFF) is transmitted.
*               3. When the receive data register is full it is read.
*
*************************************************************************/
UINT8 SPI_Receive_byte(void){
	(void)SPI2DL;              // Clear the receive data register
	SPI2DL = 0xFF;               // Transmit dummy byte (0xFF)
	while(!SPI2S_SPRF){};	    // Wait for byte to be received.
	return(SPI2DL);            // Read received byte
}

/***********************************************************************
*
*    SPI_High rate
*
*  Description: Function sets the SPI2 clock rate to 4MHz.
*               This is the SPI-SD Card operational clock rate after
*               initialisation.
*
*************************************************************************/
void SPI_High_rate(void){
  SPI2C1_SPE = 0;       // Disable SPI2
  //SPI2BR = 0x20;        // SPI2 clock rate 4MHz for 24 Mhz bus clock 
  SPI2BR = 0x00;        // SPI2 clock rate 12MHz for 24 Mhz bus clock      		
     		
  SPI2C1_SPE = 1;       // Enable SPI2
}