/******************************************************************************
 *                                                  
 *
 *  File Name:   Vib_Cont_01.c
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

/*-----------------------------------------------------*/

#include "Fat.h"
#include "Vib_Cont_01.h"
#include "Cpu.h"   // for Cpu_DisableInt()
#include "IO_Map.h"  // for port operation
#include "SD.h"

/******************** CONSTANTS ******************************/
#define SampleSize 31     // # bytes in a sample line
#define CBufferSize 2048  // # bytes in the circular buffer 
/******************** USED VARIABLES *************************/

UINT8 u8sd_status; /* Variable to indicate SD status */
UINT8 u8sd_command; /* Variable to indicate commands for SD */
UINT8 u8irda_status; /* Variable to indicate commands for IrDA */
UINT8 u8buffer[512]; /* Array to store information of the SD */
UINT16 u16buffer_index; /* Variable to indicate position of buffer */
UINT16 u16file_counter; /* Variable to indicate remaining bytes of a file */

/* Variables for storing Accelerometer readings */
UINT16 Accel[10]; /* Stores the record number and the 9 accelerometer readings */
UINT16 RecordNumber; /* Record Number: the first column of a sample record */
UINT16 InterruptNumber; /* S/H pulse counter */
UINT8 Counter;

/* Variables for controlling the sampling operation */
//UINT8  Sampling;          /* True when sampling is in progress */
UINT8 TakeSample; /* True if a sample is due */
UINT8 System_Status; /* System is currently sampling */
//UINT8  u8bufferEven[512]; /* SD Card sample buffer */
UINT8 u8bufferEven[1]; /* SD Card sample buffer */
//UINT8  u8bufferOdd[512];  /* SD Card sample buffer */
UINT8 u8bufferOdd[1]; /* SD Card sample buffer */
UINT8 EvenBufferActive; /* Active buffer flag */
UINT16 u16Evenbuffer_index;/* Variable to indicate position of buffer */
UINT16 u16Oddbuffer_index; /* Variable to indicate position of buffer */
UINT8 EvenBufferEmpty; /* Buffer is empty & can be written to */
UINT8 OddBufferEmpty; /* Buffer is empty & can be written to */
UINT8 SamplingBlocked; /* Sampling is blocked due to buffers full */
UINT8 EBToDisk; /* Even buffer is being written to disk */
UINT8 OBToDisk; /* Odd buffer is being written to disk */
UINT8 EBToDiskPending; /* Even buffer is ready to write to disk */
UINT8 OBToDiskPending; /* Odd buffer is ready to write to disk */
UINT8 u8CircularBuffer[CBufferSize]; // Sample buffer
UINT16 u16CBufferPutter; // points to next vacant byte location in the buffer
UINT16 u16CBufferGetter; // points to the next available byte in the buffer
UINT16 u16CBufferBytes; // # bytes in the buffer
UINT16 u16TempCBufferBytes;
UINT8 ReadDataStop; // Blocks reading sensor daya while writing to the SD card
//UINT16 LostReads;         // # lost reads
UINT8 BufferEmpty;
//UINT8  u8Sensor1Rx[6];

extern UINT16 u16FAT_Data_BASE;
extern WriteRHandler WHandler;
extern UINT32 u32DataSector;
extern UINT16 u16FAT_Data_BASE;
extern UINT16 u16FAT_Cluster_Size;

/************************************************************************************/
/************************************************************************************/

/************************************************************************************
 **
 ***   FUNCTIONS FOR OPERATING AN SD CARD   ***
 **
 *************************************************************************************/

/*----------------------------------------------------------------------------------*/

/***********************************************************************
 *
 *    VibContInit
 *
 *  Description: Initialises the Vibration Controller Board
 *               Enables reading of the"Sampling Switch" .
 *               Sets the Vibration Controller Board System Status.   
 *               Configures the SPI module & FAT.   
 *               Checks that an SD card is present and contains the file "FILE>TXT".   
 *
 *               Called from main() in SD_Card_Writer.c
 *
 *************************************************************************/
void VibContInit(void) {

	/* Enable reading of the Sample Switch on PTC4 */
	_SAMPLE = _IN; // Set pin as an input

	/* Set Sensor 1 LED pin as an output */_LED1 = _OUT; // DDR to output
	LED1 = OFF; // pin low, LED off

	/* Set Sensor 2 LED pin as an output */_LED2 = _OUT; // DDR to output
	LED2 = OFF; // pin low, LED off

	/* Set Sensor 3 LED pin as an output */_LED3 = _OUT; // DDR to output
	LED3 = OFF; // pin low, LED off

	/* Set Sensor 4 LED pin as an output */_LED4 = _OUT; // DDR to output
	LED4 = OFF; // pin low, LED off

	/* Set Sensor board Slave Selects as outputs & initialise high */_SS1 = _OUT; // DDR to output
	SS1 = OFF; // Slave Select. NB: Signal inverted on Sensor board
	_SS2 = _OUT; // DDR to output
	SS2 = OFF; // Slave Select. NB: Signal inverted on Sensor board 
	_SS3 = _OUT; // DDR to output
	SS3 = OFF; // Slave Select. NB: Signal inverted on Sensor board

	ErrorPulse1(5);
	ErrorPulse3(2);

	/* Initialise sampling buffers */
	EvenBufferActive = TRUE;
	u16Evenbuffer_index = 0;
	u16Oddbuffer_index = 0;
	EvenBufferEmpty = TRUE;
	OddBufferEmpty = TRUE;
	EBToDisk = FALSE;
	OBToDisk = FALSE;
	EBToDiskPending = FALSE;
	OBToDiskPending = FALSE;
	u16CBufferPutter = 0x0000; // putter to start of buffer buffer
	u16CBufferGetter = 0x0000; // getter to start of buffer buffer
	u16CBufferBytes = 0x0000; // buffer is empty
	ReadDataStop = FALSE;

	/* Vibration Controller Board System Status */
	System_Status = NOT_SAMPLING; // Sample switch is not set
	TakeSample = FALSE; // A sample is not due   

	/* Configure SPI module to handle a SD card. Return OK if successful. */
	u8sd_status = SD_Init();
	if (u8sd_status != OK) {
		u8sd_status -= 0x10;
		while (1) {
			ErrorPulse2(u8sd_status);
			ErrorPulse3(1);
		}
	}

	ErrorPulse2(1); // SD_Init OK
	
	for(;;){
		ErrorPulse1(1);
		ErrorPulse2(1);
	}
	/* Check that the SD Card is present and it contains the file "FILE.TXT"
	 and initialise the File System (FAT)  */
	vfnSDwelcome();
	
	ErrorPulse3(2);

	/* Initialise the sample number */
	RecordNumber = 0x0001;
	InterruptNumber = 0x0001;
	Counter = 0;

	ErrorPulse1(3); // SD_Init OK

	/* Initialise SPI1 as Master device to communicate with the Sensor board */
	SPI1_Init();

}

/***********************************************************************
 *
 *********** ErrorPulse1 ***********
 *
 *  Description: Pulse LED1 to indicate an error.
 *               
 *                  
 *  Input:       The number of pulses required.                
 *                  
 *
 *               
 *
 *************************************************************************/
void ErrorPulse1(UINT8 pulses) {
	UINT16 Count, j;

	while (pulses) {

		/* j = 200 gives 10mSec delay */

		/* LED1 off for 1 Sec */
		LED1 = OFF;
		for (j = 0; j < 1000; j++) {
			for (Count = 0; Count < 250; Count++) {
				//        i += 1;
			}
		}

		/* LED1 ON for 0.1 sec */LED1 = ON;
		for (j = 0; j < 200; j++) {
			for (Count = 0; Count < 500; Count++) {
				//        i += 1;
			}
		}
		LED1 = OFF;

		pulses--;
	}

}

/***********************************************************************
 *
 *********** ErrorPulse2 ***********
 *
 *  Description: Pulse LED2 to indicate an error.
 *               
 *                  
 *  Input:       The number of pulses required.                
 *                  
 *
 *               
 *
 *************************************************************************/
void ErrorPulse2(UINT8 pulses) {
	UINT16 Count, j;

	while (pulses) {

		/* j = 200 gives 10mSec delay */

		/* LED2 off for 1 Sec */
		LED2 = OFF;
		for (j = 0; j < 1000; j++) {
			for (Count = 0; Count < 250; Count++) {
				//        i += 1;
			}
		}

		/* LED2 ON for 0.1 sec */LED2 = ON;
		for (j = 0; j < 200; j++) {
			for (Count = 0; Count < 500; Count++) {
				//        i += 1;
			}
		}
		LED2 = OFF;

		pulses--;
	}

}

/***********************************************************************
 *
 *********** ErrorPulse3 ***********
 *
 *  Description: Pulse LED3 to indicate an error.
 *               
 *                  
 *  Input:       The number of pulses required.                
 *                  
 *
 *               
 *
 *************************************************************************/
void ErrorPulse3(UINT8 pulses) {
	UINT16 Count, j;

	while (pulses) {

		/* j = 200 gives 10mSec delay */

		/* LED3 off for 1 Sec */
		LED3 = OFF;
		for (j = 0; j < 1000; j++) {
			for (Count = 0; Count < 250; Count++) {
				//        i += 1;
			}
		}

		/* LED3 ON for 0.1 sec */LED3 = ON;
		for (j = 0; j < 200; j++) {
			for (Count = 0; Count < 500; Count++) {
				//        i += 1;
			}
		}
		LED3 = OFF;

		pulses--;
	}

}

/***********************************************************************
 *
 *********** ErrorPulse4 ***********
 *
 *  Description: Pulse LED4 to indicate an error.
 *               
 *                  
 *  Input:       The number of pulses required.                
 *                  
 *
 *               
 *
 *************************************************************************/
void ErrorPulse4(UINT8 pulses) {
	UINT16 Count, j;

	while (pulses) {

		/* j = 200 gives 10mSec delay */

		/* LED4 off for 1 Sec */
		LED4 = OFF;
		for (j = 0; j < 1000; j++) {
			for (Count = 0; Count < 250; Count++) {
				//        i += 1;
			}
		}

		/* LED4 ON for 0.1 sec */LED4 = ON;
		for (j = 0; j < 200; j++) {
			for (Count = 0; Count < 500; Count++) {
				//        i += 1;
			}
		}
		LED4 = OFF;

		pulses--;
	}

}

/***********************************************************************
 *
 *********** VibrationControl ***********
 *
 *  Description: Main operating function for the Vibration Controller Board.
 *               
 *                  
 *                  
 *                  
 *
 *               Called from main() in SD_Card_Writer.c
 *
 *************************************************************************/
void VibrationControl(void) {
	UINT8 count = 0;

	for (;;) {
		/* If the Sample Switch is active the system should be sampling */
		if (SAMPLE) {

			/* Sampling switch is active */
			/* If this is the start of sampling a new file must be created & opened */
			if (System_Status == NOT_SAMPLING) {

				/* Initiate Sampling */
				for (InterruptNumber = 0; InterruptNumber < 2550;
						InterruptNumber++) {
					count++;
					if (count == 200) {
						count = 0;
					}
				}
				/* Don't open the file any more as we do direct write to sector */
//          u8sd_status = FAT_FileOpen("FILE.TXT",MODIFY);  // Open file for writing
//          TakeSample = FALSE;   // clear the flag
				/* Initialise the sample number */
//          RecordNumber = 0x0001; 
				InterruptNumber = 0x03E8; // 1000
//          LostReads = 0;
				BufferEmpty = 0;
				System_Status = SAMPLING; // Start sampling
				LED1 = 1;

			}

		} else { // if(SAMPLE)
			/* Check if sampling has finished */
			if (System_Status == SAMPLING) {
				System_Status = NOT_SAMPLING; // Shut down the sampling          
				/* Close off the file: Not done anymore */
//          FAT_FileClose();

//          count = count +1;
				LED1 = 0;
			}

		} // if(SAMPLE)

		/* If there is a full sector (BLOCK_SIZE = 512 bytes) in the 
		 circular buffer send it to the SD Card */

//         if(u16CBufferBytes >= BLOCK_SIZE)
		__DI();
		u16TempCBufferBytes = u16CBufferBytes;
		__EI();
		if (u16TempCBufferBytes >= BLOCK_SIZE) {
			ReadDataStop = TRUE; // Stop data read while writing to SD card
			SD_CBufferDataSectorWrite(); // write block to SD card
			ReadDataStop = FALSE;

		} //if(u16CBufferBytes >= BLOCK_SIZE)

	}

}

/***********************************************************************
 *
 *    vfnSDwelcome
 *
 *  Description: Checks if SD Card present - if not ?????
 *               If Card is present: 
 *                  If file "FILE.TXT" exists it is opened,
 *                  If file does not exist it is created and opened.
 *                  File is then closed.
 *
 *               Called from main() in SD_Card_Writer.c
 *
 *************************************************************************/
void vfnSDwelcome(void) {
	UINT8 result;
	UINT16 i;

	/* Check if SD card present - if not ????? */
	if (u8sd_status == NO_SD_CARD) // Status set by SD_Init() in SD.c.
			{
		/* SD Card not present */
		Cpu_DisableInt(); // Disable interrupts
		ErrorPulse3(4);
		for (;;)
			; // Debug trap
	} else {
		ErrorPulse1(1);
		/* SD Card present, read the Master Block (Boot Sector) of the memory */
		FAT_Read_Master_Block(); // in Fat.c

		ErrorPulse2(1);
		/* Make sure file "FILE.TXT" exists */
		u8sd_status = FAT_FileOpen("FILE.TXT", MODIFY); // Attempt to open file

		ErrorPulse3(1);
		
		/* Does file exist? */
		if (u8sd_status == FILE_NOT_FOUND) {
			/* If the file does not exist, it is created */
			u8sd_status = FAT_FileOpen("FILE.TXT", CREATE);
			// Valid u8sd_status == FILE_CREATE_OK

			/*** Set up the FAT to a file of 11,000 clusters which is approx
			 11,000 clusters * 32768 bytes/cluster = 360,448kB, or
			 360,448kB / 31 bytes/sample = 11,627,354 samples, or
			 11,627,354 samples / 2,000 samples/sec = 5,814 seconds of sampling, or
			 approx 1.61 hours of samples. ***/

			/* Set "u32DataSector" to the first data sector to be written.
			 u32DataSector is used in function FAT_DataSectorWrite() in Fat.c
			 for writing a block of data to the SD card. */

			/* Create the file FAT */
			for (i = 0; i < 11000; i++) {

				(void) FAT_Entry(WHandler.CurrentFatEntry,
						WHandler.CurrentFatEntry + 1, WRITE_ENTRY);
				WHandler.CurrentFatEntry++;
				WHandler.File_Size += 32768; // update the file size, 32768 bytes per cluster
			}

		}

		/* Use this to append to an existing file */
//       u32DataSector= u16FAT_Data_BASE + WHandler.ClusterIndex + (WHandler.CurrentFatEntry-2)*u16FAT_Cluster_Size;
		/* Use this to place the first data block at the start of the SD Card data region. */
		u32DataSector = u16FAT_Data_BASE;

		/* Erase the SD Card data area. Return OK if successful. */
//    result = SD_Erase_Blocks(u16FAT_Data_BASE,u16FAT_Data_BASE + 8192); // 4MB
		result = SD_Erase_Blocks(u16FAT_Data_BASE, u16FAT_Data_BASE + 2000000); // 1GB
//    result = SD_Erase_Blocks(0x4000,0x4000 + 500);
		if (result != OK) {
			FAT_FileClose();
			ErrorPulse3(5);
			for (;;)
				;
		}

		/* Close file */
		FAT_FileClose();

		//This part sets the initial values of these variables
		u16buffer_index = 0;
		u8sd_command = SD_INACTIVE;
	}
}

/***********************************************************************
 *
 *    vfnWriteFile
 *
 *  Description: Writes the contents of the buffer "u8buffer" to the file "FILE.TXT".
 *                
 *
 *************************************************************************/
void vfnWriteFile(void) {
	UINT8 i = 0;

	//u16buffer_index = 0;

	u8sd_status = FAT_FileOpen("FILE.TXT", MODIFY);
	if (u8sd_status == FILE_FOUND) {

		//Write the u8buffer to the file
		FAT_FileWrite(u8buffer, u16buffer_index);
		FAT_FileClose();

		//Sets these variables to inactive states 
		u16buffer_index = 0;
		u8sd_command = SD_INACTIVE;
	} else {
		//If the file was not found, debug trap
		i += 1;
		;
	}

}

/***********************************************************************
 *
 *    StringToFile
 *
 *  Description: Writes the contents of the buffer "u8buffer" to the file "FILE.TXT".
 *                
 *
 *************************************************************************/
void StringToFile(void) {

	UINT8 i = 0, *str = "Hello World"; // String to write to file

	/* Transfer the string to the u8buffer */
	u16buffer_index = 0; // Zero buffer pointer

	while (str[i]) {
		u8buffer[u16buffer_index++] = str[i++];
	}

	/* Write string to file */
	vfnWriteFile();

}

/***********************************************************************
 *
 *    Uint16ToString
 *
 *  Description: Converts Num to a 5 char string and places it in 
 *                 the buffer "u8bufferEvan/u8bufferOdd".
 *               0.1mSec execution time.
 *
 *  Input:       Num - the UINT16 number to convert.
 *               pu8Buffer - the buffer where the data is to be saved.
 *               putter    - pointer to the buffer putter.
 *                
 *
 *************************************************************************/
void Uint16ToString(UINT16 Num, UINT8 *pu8Buffer, UINT16 *putter) {

	UINT8 result8, digit;
	UINT16 result16;

	result16 = Num;

	/* 10 thousands */
	digit = result16 / 10000;
	pu8Buffer[(*putter)++] = (digit + 0x30);
	result16 = result16 % 10000;

	/* thousands */
	digit = result16 / 1000;
	pu8Buffer[(*putter)++] = (digit + 0x30);
	result16 = result16 % 1000;

	/* hundreds */
	digit = result16 / 100;
	pu8Buffer[(*putter)++] = (digit + 0x30);
	result8 = result16 % 100;

	/* tens */
	digit = result8 / 10;
	pu8Buffer[(*putter)++] = (digit + 0x30);
	digit = result8 % 10;

	/* units */
	pu8Buffer[(*putter)++] = (digit + 0x30);

}

/***********************************************************************
 *
 *    ToBuffer
 *
 *  Description: Places Digit in the buffer "u8buffer".
 *               u16buffer_index is post incremented.
 *                
 *
 *************************************************************************/
void ToBuffer(UINT8 Digit) {

	u8buffer[u16buffer_index++] = Digit;

}

/***********************************************************************
 *
 *    *** ReadData ***
 *
 *  Description: Read sampled data from the sensor boards and save in
 *               the circular buffer "u8CircularBuffer".
 *               Only if there is space in the buffer and 
 *               not writing to the SD card.
 *
 *  Calling fn:  Called from SamplingDelay_OnInterrupt() in Events.c 
 *               as part of the SamplingDelay Interrupt Service Routine.
 *                
 *
 *************************************************************************/
void ReadData(void) {

	/* Check there is space in the buffer for a sample ("SampleSize" bytes) */
//  if((u16CBufferBytes < (CBufferSize - SampleSize)) && (ReadDataStop == FALSE))
	if (u16CBufferBytes < (CBufferSize - SampleSize))

	{

		/* Place a sample line in the circular buffer */
		DataTest();
	}
//  else
//  {
//    LostReads++;
//  }

}

/***********************************************************************
 *
 *    *** DataTest ***
 *
 *  Description: Place a sample line of demo accelerometer data in the
 *               circular buffer "u8CircularBuffer".
 *               Uses accelerometer data from Sensor board 1, rest
 *               is dummy data.
 *
 *  Input:       void.
 *                
 *
 *************************************************************************/
void DataTest(void) {
	UINT8 i, j, LSB, MSB;
	UINT16 ADCResult;

	/* Load demo accelerometer result data */
	Accel[0] = InterruptNumber; // 16 bit S/H pulse sequential counter

//  for(i=1;i<6;i++)
//  {
//    u8Sensor1Rx[i]=0;
//  }

	/* Read Sensor board 1 results */
	/* Receive bytes until you get a 0xAA */LED2 = ON; // LED2 ON while reading Sensor board 1.
	do {
		SS1 = ON;
		(void) SPI1DL; // Clear the receive data register
		SPI1DL = 0xFF; // Transmit dummy byte (0xFF)
		while (!SPI1S_SPRF) {
		}; // Wait for byte to be received.
		LSB = SPI1DL; // Read received byte
		SS1 = OFF;

	} while (LSB != 0xAA);

	for (i = 1; i < 4; i++) {
		SS1 = ON;
		(void) SPI1DL; // Clear the receive data register
		SPI1DL = 0xFF; // Transmit dummy byte (0xFF)
		while (!SPI1S_SPRF) {
		}; // Wait for byte to be received.
		Accel[i] = SPI1DL << 8; // MSB
		SS1 = OFF;
		SS1 = ON;
		(void) SPI1DL; // Clear the receive data register
		SPI1DL = 0xFF; // Transmit dummy byte (0xFF)
		while (!SPI1S_SPRF) {
		}; // Wait for byte to be received.
		Accel[i] = Accel[i] | (UINT16) SPI1DL; // MSB
		SS1 = OFF;

	}
	LED2 = OFF;

	/* Read Sensor board 2 results */
	/* Receive bytes until you get a 0xAA */LED3 = ON; // LED3 ON while reading Sensor board 2.
	do {
		SS2 = ON;
		(void) SPI1DL; // Clear the receive data register
		SPI1DL = 0xFF; // Transmit dummy byte (0xFF)
		while (!SPI1S_SPRF) {
		}; // Wait for byte to be received.
		LSB = SPI1DL; // Read received byte
		SS2 = OFF;

	} while (LSB != 0xAA);

	for (i = 4; i < 7; i++) {
		SS2 = ON;
		(void) SPI1DL; // Clear the receive data register
		SPI1DL = 0xFF; // Transmit dummy byte (0xFF)
		while (!SPI1S_SPRF) {
		}; // Wait for byte to be received.
		Accel[i] = SPI1DL << 8; // MSB
		SS2 = OFF;
		SS2 = ON;
		(void) SPI1DL; // Clear the receive data register
		SPI1DL = 0xFF; // Transmit dummy byte (0xFF)
		while (!SPI1S_SPRF) {
		}; // Wait for byte to be received.
		Accel[i] = Accel[i] | (UINT16) SPI1DL; // MSB
		SS2 = OFF;

	}
	LED3 = OFF;

	/* Read Sensor board 3 results */
	/* Receive bytes until you get a 0xAA */LED4 = ON; // LED4 ON while reading Sensor board 3.
	do {
		SS3 = ON;
		(void) SPI1DL; // Clear the receive data register
		SPI1DL = 0xFF; // Transmit dummy byte (0xFF)
		while (!SPI1S_SPRF) {
		}; // Wait for byte to be received.
		LSB = SPI1DL; // Read received byte
		SS3 = OFF;

	} while (LSB != 0xAA);

	for (i = 7; i < 10; i++) {
		SS3 = ON;
		(void) SPI1DL; // Clear the receive data register
		SPI1DL = 0xFF; // Transmit dummy byte (0xFF)
		while (!SPI1S_SPRF) {
		}; // Wait for byte to be received.
		Accel[i] = SPI1DL << 8; // MSB
		SS3 = OFF;
		SS3 = ON;
		(void) SPI1DL; // Clear the receive data register
		SPI1DL = 0xFF; // Transmit dummy byte (0xFF)
		while (!SPI1S_SPRF) {
		}; // Wait for byte to be received.
		Accel[i] = Accel[i] | (UINT16) SPI1DL; // MSB
		SS3 = OFF;

	}
	LED4 = OFF;

////  Accel[1] = 0x0024;     // Board 1 Accel 1 - 00036
////  Accel[2] = 0x1338;     // Board 1 Accel 2 - 04920
////  Accel[3] = 0x1771;     // Board 1 Accel 3 - 06001

////
////  Accel[4] = 0x84D0;     // Board 2 Accel 1 - 34000
////  Accel[5] = 0x0002;     // Board 2 Accel 2 - 00002
////  Accel[6] = 0x027F;     // Board 2 Accel 3 - 00639
////  Accel[7] = 0x14B3;     // Board 3 Accel 1 - 05299
////  Accel[8] = 0x037C;     // Board 3 Accel 2 - 00892
////  Accel[9] = 0x000A;     // Board 3 Accel 3 - 00010
////  
	/* Write accelerometer data to circular buffer in csv format */
	for (i = 0; i <= 8; i++) { // write first 8 data words + "," to buffer
		ToCBuffer((UINT8) (Accel[i] >> 8)); // MSB
		ToCBuffer((UINT8) (Accel[i] & 0xFF)); // LSB
		ToCBuffer(','); // add comma seperator
	}
	// write last data word to buffer
	ToCBuffer((UINT8) (Accel[9] >> 8)); // MSB
	ToCBuffer((UINT8) (Accel[9] & 0xFF)); // LSB

	ToCBuffer(CR); // add Carriage Return terminator 0x0D

	ToCBuffer(LF); // add Line Feed terminator 0x0A

	RecordNumber++; // Increment the record number count

}

/********************************************************************
 *
 *          *** ToCBuffer ***
 *
 *   Description: Place the byte "u8byte"in the circular buffer 
 *                 "u8CircularBuffer" at "u16CBufferPutter".
 *                Already know there is space in the buffer
 *
 *   Exit:        Byte in the buffer
 *                u16CBufferPutter incremented.
 *                # of bytes in the buffer "u16CBufferBytes" incremented
 *
 **********************************************************************/
void ToCBuffer(UINT8 u8byte) {
	if (u16CBufferPutter >= CBufferSize) // check for buffer wrap around
		u16CBufferPutter = 0x0000;

	u8CircularBuffer[u16CBufferPutter++] = u8byte;
	u16CBufferBytes++; // inc # bytes in buffer
}

/********************************************************************
 *
 *          *** FromCBuffer ***
 *
 *   Description: Get the next byte from the circular buffer
 *                 "u8CircularBuffer" at "u16CBufferGetter".
 *                Already know there is a byte available in the buffer
 *
 *   Exit:        Byte returned
 *                u16CBufferGetter incremented.
 *                # of bytes in the buffer "u16CBufferBytes" decremented
 *
 **********************************************************************/
UINT8 FromCBuffer(void) {
	UINT8 returnbyte;

	__DI();

	if (u16CBufferGetter >= CBufferSize) // check for buffer wrap around
		u16CBufferGetter = 0x0000;

	u16CBufferBytes--; // dec # bytes in the buffer

//  if(u16CBufferBytes == 0x0000)
//     BufferEmpty++;

	returnbyte = u8CircularBuffer[u16CBufferGetter++];

	__EI();

	return (returnbyte);
}

/*
 ** ===================================================================
 **     Method      :  SampleDelay_Init (component TimerInt)
 **
 **     Description :
 **         Initializes Timer 1 Channel 0. 
 **         Provides a 100uSec sampling delay. 
 **         Output Compare mode with interrupt on match. No external pins.
 **         Timer stopped.
 **         
 ** ===================================================================
 */
void SampleDelay_Init(void) {
	/* Stop HW; disable overflow interrupt and set prescaler to 1 */
	/* TPM1SC: TOF=0,TOIE=0,CPWMS=0,CLKSB=0,CLKSA=0,PS2=0,PS1=0,PS0=0 */
	setReg8(TPM1SC, 0x00);
	/* Set output compare mode and enable compare interrupt */
	/* TPM1C0SC: CH0F=0,CH0IE=1,MS0B=0,MS0A=1,ELS0B=0,ELS0A=0,??=0,??=0 */
	setReg8(TPM1C0SC, 0x50);
//  EnUser = TRUE;                       /* Enable device */
//  SampleDelay_EnEvent = TRUE;          /* Enable event */
	/* Initialize the compare/modulo/reload register foe a 100uSec period*/TPM1C0V =
			(word) (0x095FU); // Set compare value
	TPM1MOD = (word) (0x095FU); // Set modulo value
//  clrSetReg8Bits(TPM1SC, 0x07, 0x00);    /* Set prescaler */
	/* TPM1CNTH: BIT15=0,BIT14=0,BIT13=0,BIT12=0,BIT11=0,BIT10=0,BIT9=0,BIT8=0 */
	setReg8(TPM1CNTH, 0x00);
	/* Reset HW Counter */
//  HWEnDi();
}


