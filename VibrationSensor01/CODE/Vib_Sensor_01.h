/******************************************************************************
*                                                  
*
*  File Name:   Vib_Cont_01.c
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

/*   *** Functions Declarations ***   */
void VibSensorInit(void);         // Sensor board initialisation
void Sensor_Sample_Accel(void);   // Sample the three ADC channels
void Sensor_Tx_Results(void);     // Tx results to controller
void Sensor_Tx_ResultsOld(void);     // Tx results to controller
void Sensor_Tx_ResultsOld1(void);     // Tx results to controller
void SPI1_Init(void);              // Initialise the SPI1 port
void ADC_Init(void);
void ADC_ISR(void);
void Sample_Accel(void);


/*   *** Constant Declarations ***   */
#define ON          FALSE              // LED ON - Output low
#define OFF         TRUE               // LED OFF - Output hi
