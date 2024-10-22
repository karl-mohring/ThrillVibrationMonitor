/******************************************************************************
*                                                  
*
*  File Name:   Vib_Cont_01.h
*
*  Project:     Helicopter Vibration measurement
*               Basket Vibration Controller Board
*               
*                                                                          
*  Description: Main Operating Functions.
*                    - SD Card Writer
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
* 1.0   2011.01.15  Peter Grabau  Initial version
*
******************************************************************************/


/* Includes */
#include "FslTypes.h"


/******** VIBRATION CONTROLLER SYSTEM STATUS ***********/
#define NOT_SAMPLING FALSE   // 
#define SAMPLING     TRUE   //


/******** Sensor 1 LED *********************************/
#define powerLED        PTGD_PTGD1     // Data pin
#define _powerLED       PTGDD_PTGDD1   // Data Direction bit
/******** Sensor 2 LED *********************************/
#define statusLED        PTGD_PTGD0     // Data pin
#define _statusLED       PTGDD_PTGDD0   // Data Direction bit
/******** Sensor 3 LED *********************************/
#define receiveLED        PTFD_PTFD1     // Data pin
#define _receiveLED       PTFDD_PTFDD1   // Data Direction bit
/******** Sensor 4 LED *********************************/
#define transmitLED        PTFD_PTFD0     // Data pin
#define _transmitLED       PTFDD_PTFDD0   // Data Direction bit

#define ON          FALSE              // LED ON
#define OFF         TRUE              // LED off

/******** LINE TERMINATOR ******************************/
#define CR          0x0D           // Carriage return
#define LF          0x0A           // Line Feed



/******** COMMANDS USED IN THE APPLICATION *************/
enum
{
 SD_INACTIVE,
 SD_FILE_WRITING,   
 SD_FILE_READING   
};  
     

/********************** FUNCTION PROTOTYPES *****************/
void vfnSDwelcome(void);	/* Check if SD is present and if the file exists */
void vfnWriteFile(void);  /* Write the u8buffer to file */
void StringToFile(void);  /* Write a string to file */
void Uint16ToString(UINT16, UINT8 *, UINT16 *); // Convert a UINT16 number to a string & place it in u8buffer 
void ToBuffer(UINT8);       /* Place a UINT8 in u8buffer & inc putter */
void DataTestOld2(void);    // Write dummy accelerometer data to the circular buffer
void DataTestOld(UINT8 *, UINT16 *); /* Write test accelerometer data to file */
void DataTest(void);        // Place demo accelerometer data in the circular buffer
void initialiseSensorBoard(void);     /* Initialise the vibration controller board */
void initialiseLEDs(void);
void writeRecordToCBuffer(void);
void stopSampling(void);
void startSampling(void);/* Main function for the Vibration Control Board */
void SampleDelay_Init(void);/* Initialise the sampling delay timer */
void ReadData(void);        /* Read sensor data */
void pulsePowerLED(UINT8 pulses); /* Pulse LED1 to indicate an error */
void pulseStatusLED(UINT8 pulses); /* Pulse LED2 to indicate an error */
void pulseReceiveLED(UINT8 pulses); /* Pulse LED3 to indicate an error */
void pulseTransmitLED(UINT8 pulses); /* Pulse LED4 to indicate an error */
void ToCBuffer(UINT8);          // Place byte in the circular buffer
UINT8 FromCBuffer(void);        // Get the next byte from the circular buffer
void togglePowerLED(void);
void toggleStatusLED(void);
void toggleReceiveLED(void);
void toggleTransmitLED(void);

/********************** GENERAL VARIABLES USED *****************/

extern UINT8  u8sd_status;       /* Variable to indicate SD status */
extern UINT8  u8sd_command;      /* Variable to indicate commands for SD */
extern UINT8  u8irda_status;     /* Variable to indicate commands for IrDA */
extern UINT8  u8buffer[512];     /* Array to store information of the SD */
extern UINT16 u16buffer_index;   /* Variable to indicate position of buffer */
extern UINT16 u16file_counter;   /* Variable to indicate remaining bytes of a file */

/* Variables for storing Accelerometer readings */
extern UINT16 RecordNumber;      /* Record Number: the first column of a sample record */

/* Variables for controlling the sampling operation */
extern UINT8  Sampling;          /* True when sampling is in progress */
extern UINT8  TakeSample;        /* True if a sample is due */
extern UINT8  System_Status;     /* System is currently sampling */




