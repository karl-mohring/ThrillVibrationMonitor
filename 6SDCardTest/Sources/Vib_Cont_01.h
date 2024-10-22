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

/******** SAMPLE SWITCH ********************************/
#define SAMPLE      PTCD_PTCD4     // Sample Switch Status
#define _SAMPLE     PTCDD_PTCDD4   // Data Direction bit

/******** Sensor 1 LED *********************************/
#define LED1        PTGD_PTGD1     // Data pin
#define _LED1       PTGDD_PTGDD1   // Data Direction bit
/******** Sensor 2 LED *********************************/
#define LED2        PTGD_PTGD0     // Data pin
#define _LED2       PTGDD_PTGDD0   // Data Direction bit
/******** Sensor 3 LED *********************************/
#define LED3        PTFD_PTFD1     // Data pin
#define _LED3       PTFDD_PTFDD1   // Data Direction bit
/******** Sensor 4 LED *********************************/
#define LED4        PTFD_PTFD0     // Data pin
#define _LED4       PTFDD_PTFDD0   // Data Direction bit

/******** Sensor board Slave Select pins ***************/
#define SS1         PTDD_PTDD0     // Sensor board 1 SS
#define _SS1        PTDDD_PTDDD0   // Data Direction bit
#define SS2         PTDD_PTDD1     // Sensor board 2 SS
#define _SS2        PTDDD_PTDDD1   // Data Direction bit
#define SS3         PTDD_PTDD2     // Sensor board 3 SS
#define _SS3        PTDDD_PTDDD2   // Data Direction bit


#define ON          TRUE              // LED ON
#define OFF         FALSE              // LED off

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
//void vfnMGC_init(void);	  /* Initializes clock using external crystal */
//void vfnWelcome(void);	  /* Send a Welcome message */
//void vfnSysHalt(void);	  /* Halts the system */
void vfnSDwelcome(void);	/* Check if SD is present and if the file exists */
//void vfnReadFile(void);	  /* Reads the file stored in the SD card */
//void vfnWriteFile(void);	/* Writes the file stored in the SD card */
void vfnWriteFile(void);  /* Write the u8buffer to file */
void StringToFile(void);  /* Write a string to file */
void Uint16ToString(UINT16, UINT8 *, UINT16 *); // Convert a UINT16 number 
                             //  to a string & place it in u8buffer 
void ToBuffer(UINT8);       /* Place a UINT8 in u8buffer & inc putter */
void DataTestOld2(void);    // Write dummy accelerometer data to the circular buffer
void DataTestOld(UINT8 *, UINT16 *); /* Write test accelerometer data to file */
void DataTest(void);        // Place demo accelerometer data in the circular buffer
void VibContInit(void);     /* Initialise the vibration controller board */
void VibrationControl(void);/* Main function for the Vibration Control Board */
void VibrationControlOld(void);/* Main function for the Vibration Control Board */
void VibrationControlOld2(void);
void SampleDelay_Init(void);/* Initialise the sampling delay timer */
//__interrupt void SampleDelay_Interrupt(void); /* Sampling delay ISR */
//void interrupt VectorNumber_Vtpm1ch0 SampleDelayISR(void);
void ReadData(void);        /* Read sensor data */
void ReadDataOld(void);
void ErrorPulse1(UINT8 pulses); /* Pulse LED1 to incicate an error */
void ErrorPulse2(UINT8 pulses); /* Pulse LED2 to incicate an error */
void ErrorPulse3(UINT8 pulses); /* Pulse LED3 to incicate an error */
void ErrorPulse4(UINT8 pulses); /* Pulse LED4 to incicate an error */
void ToCBuffer(UINT8);          // Place byte in the circular buffer
UINT8 FromCBuffer(void);        // Get the next byte from the circular buffer



/********************** GENERAL VARIABLES USED *****************/

extern UINT8  u8sd_status;       /* Variable to indicate SD status */
extern UINT8  u8sd_command;      /* Variable to indicate commands for SD */
extern UINT8  u8irda_status;     /* Variable to indicate commands for IrDA */
extern UINT8  u8buffer[512];     /* Array to store information of the SD */
extern UINT16 u16buffer_index;   /* Variable to indicate position of buffer */
extern UINT16 u16file_counter;   /* Variable to indicate remaining bytes of a file */

/* Variables for storing Accelerometer readings */
extern UINT16 Accel[10];         /* Stores the record number and the 9 accelerometer readings */
extern UINT16 RecordNumber;      /* Record Number: the first colimn of a sample record */
extern UINT16 InterruptNumber;   /* The number of interrupts during sampling */
extern UINT8  Counter;

/* Variables for controlling the sampling operation */
extern UINT8  Sampling;          /* True when sampling is in progress */
extern UINT8  TakeSample;        /* True if a sample is due */
extern UINT8  System_Status;     /* System is currently sampling */




