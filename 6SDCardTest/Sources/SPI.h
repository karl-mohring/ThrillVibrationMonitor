/******************************************************************************
*                                                  
*
*  File Name:   SPI.h
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





#ifndef __SPI__
#define __SPI__

/* Includes */

#include "FslTypes.h"
// #include "derivative.h"
#include "IO_Map.h"


/* definitions */

#define SPI_SS    PTBD_PTBD3      /* Slave Select (CS pin for the SD Card) */                      
#define _SPI_SS   PTBDD_PTBDD3    /* Slave Select DDR (CS pin for the SD Card) */




#define ENABLE    0
#define DISABLE   1

/* Port Data Direction Bit Directions */
//#define _IN    0
//#define _OUT   1

/* Global Variables */

/* Prototypes */
void SPI_Init(void);
void SPI1_Init(void);
void SPI_Send_byte(UINT8 u8Data);
void SPI1_Send_byte(UINT8 u8Data);
UINT8 SPI_Receive_byte(void);
UINT8 SPI1_Receive_byte(void);

void SPI_High_rate(void);


#endif /* __SPI__ */