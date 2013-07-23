/******************************************************************************
 *                                                  
 *
 *  File Name:   Vib_Cont_01.c
 *
 *  Project:     Thrill vibration sensor
 *               
 *                                                                          
 *  Description: Main Operating Functions.
 *
 *  Processor:   MC9S08JM60CLD 
 *                                            
 *  Version:     1.0                                                         
 *                                                                                                                                                         
 *  Author:      Karl Mohring                                                                                                                                   
 *                                                  
 * UPDATED HISTORY:
 *
 * REV   YYYY.MM.DD  AUTHOR        DESCRIPTION OF CHANGE
 * ---   ----------  ------        --------------------- 
 * 0.0   2011.01.15  Peter Grabau  Initial version
 * 1.0	 2013.07.09	 Karl Mohring  Adapted code
 *
 ******************************************************************************/

#include "Fat.h"
#include "Vib_Cont_01.h"
#include "Cpu.h"   // for Cpu_DisableInt()
#include "IO_Map.h"  // for port operation
#include "SD.h"

/******************** CONSTANTS ******************************/
#define CBufferSize 2048  // # bytes in the circular buffer 
// Samples per second (all channels). Note: this value is not generated or used apart from FAT calculations
const UINT16 SAMPLE_RATE = 2000;

// Number of accelerometer channels being sampled.
const UINT8 NUM_CHANNELS = 3;

// Size of record counter in bytes
const UINT8 RECORD_COUNTER_SIZE = 2;

// Size of divisor (comma) in bytes
const UINT8 DIVISOR_SIZE = 1;

// Size of line delimiter characters (CR,LF) in bytes
const UINT8 LINE_DELIMETER_SIZE = 2;

// Size of an accelerometer sample from a single channel in bytes
const UINT8 SAMPLE_SIZE = 2;

// Size of each record in CSV format in bytes
const UINT8 RECORD_SIZE = RECORD_COUNTER_SIZE + DIVISOR_SIZE
		+ NUM_CHANNELS * (SAMPLE_SIZE + DIVISOR_SIZE) + LINE_DELIMETER_SIZE;

const UINT8 RUNTIME_HOURS = 0;
const UINT8 RUNTIME_MINUTES = 0;
const UINT8 RUNTIME_SECONDS = 10;

// Runtime of the sampling session in seconds
const UINT32 TOTAL_RUNTIME = 3600 * RUNTIME_HOURS + 60 * RUNTIME_MINUTES
		+ RUNTIME_SECONDS;

//Number of bytes per cluster for the project file
const UINT16 CLUSTER_SIZE = 32768;

/*** Set up the FAT to a file of 11,000 clusters which is approx
 11,000 clusters * 32768 bytes/cluster = 360,448kB, or
 360,448kB / 31 bytes/sample = 11,627,354 samples, or
 11,627,354 samples / 2,000 samples/sec = 5,814 seconds of sampling, or
 approx 1.61 hours of samples. ***/
// Number of clusters for project file.
const UINT32 NUM_CLUSTERS = ((TOTAL_RUNTIME * SAMPLE_RATE * RECORD_SIZE)
		/ CLUSTER_SIZE) + 10;

/******************** USED VARIABLES *************************/

UINT8 u8sd_status; /* Variable to indicate SD status */
UINT8 u8sd_command; /* Variable to indicate commands for SD */
UINT8 u8irda_status; /* Variable to indicate commands for IrDA */
UINT8 u8buffer[512]; /* Array to store information of the SD */
UINT16 u16buffer_index; /* Variable to indicate position of buffer */
UINT16 u16file_counter; /* Variable to indicate remaining bytes of a file */

UINT16 recordNumber; /* Record Number: the first column of a sample record */
UINT32 sessionRuntime;

/* Variables for controlling the sampling operation */
//UINT8  Sampling;          /* True when sampling is in progress */
UINT8 samplingStatus;
UINT8 u8CircularBuffer[CBufferSize]; // Sample buffer
UINT16 u16CBufferPutter; // points to next vacant byte location in the buffer
UINT16 u16CBufferGetter; // points to the next available byte in the buffer
UINT16 u16CBufferBytes; // # bytes in the buffer
UINT16 u16TempCBufferBytes;
UINT8 ReadDataStop; // Blocks reading sensor data while writing to the SD card

//UINT16 LostReads;

UINT8 u8AccelSamples[6]; // 3 x 16 bit results, MSB first
UINT16 samples[3];
UINT8 u8Channel; // The channel that is being sampled

extern UINT16 u16FAT_Data_BASE;
extern WriteRHandler WHandler;
extern UINT32 u32DataSector;
extern UINT16 u16FAT_Data_BASE;
extern UINT16 u16FAT_Cluster_Size;

/**
 * Initialise the thrill sensor board and associated components.
 * The SD card and interface are also configured and a project file is created.
 */
void initialiseSensorBoard(void) {
	// Disable timer until after sampling starts
	sampleTimer_Disable();

	initialiseLEDs();
	pulsePowerLED(1);
	statusLED = ON;

	/* Initialise sampling buffers */
	u16CBufferPutter = 0;
	u16CBufferGetter = 0;
	u16CBufferBytes = 0;
	ReadDataStop = FALSE;

	receiveLED = ON;
	/* Configure SPI module to handle a SD card. Return OK if successful. */
	u8sd_status = SD_Init();

	// Continuously loop error code if SD initialisation is unsuccessful
	if (u8sd_status != OK) {
		u8sd_status -= 0x10;
		for (;;) {
			pulseReceiveLED(u8sd_status);
			pulseTransmitLED(1);
		}
	}
	pulseReceiveLED(1); //SD init OK

	// Check that the SD Card is present and it contains the file "FILE.TXT" and initialise the File System (FAT)
	vfnSDwelcome();
	pulseReceiveLED(1);

	statusLED = OFF;

	/* Initialise the sample number */
	recordNumber = 0;
	sessionRuntime = 1;
	powerLED = ON;
}

/**
 * Set up the boards LED indicators
 */
void initialiseLEDs() {
	/* Set Sensor 1 LED pin as an output */
	_powerLED = _OUT; // DDR to output
	powerLED = OFF; // pin low, LED off

	/* Set Sensor 2 LED pin as an output */_statusLED = _OUT; // DDR to output
	statusLED = OFF; // pin low, LED off

	/* Set Sensor 3 LED pin as an output */_receiveLED = _OUT; // DDR to output
	receiveLED = OFF; // pin low, LED off

	/* Set Sensor 4 LED pin as an output */_transmitLED = _OUT; // DDR to output
	transmitLED = OFF; // pin low, LED off
}

/**
 * Pulse the power LED a specified number of times
 * @pulses number of times to pulse LED
 */
void pulsePowerLED(UINT8 pulses) {
	UINT16 Count, j;

	while (pulses) {

		/* j = 20 gives 1mSec delay */

		/* LED1 off for 1 Sec */
		powerLED = OFF;
		for (j = 0; j < 1000; j++) {
			for (Count = 0; Count < 250; Count++) {
			}
		}

		/* LED1 ON for 0.1 sec */powerLED = ON;
		for (j = 0; j < 200; j++) {
			for (Count = 0; Count < 500; Count++) {
			}
		}

		powerLED = OFF;

		pulses--;
	}

}

/**
 * Pulse the status LED a specified number of times
 * @pulses number of times to pulse LED
 */
void pulseStatusLED(UINT8 pulses) {
	UINT16 Count, j;

	while (pulses) {

		/* j = 200 gives 10mSec delay */

		/* LED2 off for 1 Sec */
		statusLED = OFF;
		for (j = 0; j < 1000; j++) {
			for (Count = 0; Count < 250; Count++) {
				//        i += 1;
			}
		}

		/* LED2 ON for 0.1 sec */statusLED = ON;
		for (j = 0; j < 200; j++) {
			for (Count = 0; Count < 500; Count++) {
				//        i += 1;
			}
		}
		statusLED = OFF;

		pulses--;
	}

}

/**
 * Pulse the receive LED a specified number of times
 * @pulses number of times to pulse LED
 */
void pulseReceiveLED(UINT8 pulses) {
	UINT16 Count, j;

	while (pulses) {

		/* j = 200 gives 10mSec delay */

		/* LED3 off for 1 Sec */
		receiveLED = OFF;
		for (j = 0; j < 1000; j++) {
			for (Count = 0; Count < 250; Count++) {
				//        i += 1;
			}
		}

		/* LED3 ON for 0.1 sec */receiveLED = ON;
		for (j = 0; j < 200; j++) {
			for (Count = 0; Count < 500; Count++) {
				//        i += 1;
			}
		}
		receiveLED = OFF;

		pulses--;
	}

}

/**
 * Pulse the transmit LED a specified number of times
 * @pulses number of times to pulse LED
 */
void pulseTransmitLED(UINT8 pulses) {
	UINT16 Count, j;

	while (pulses) {

		/* j = 200 gives 10mSec delay */

		/* LED4 off for 1 Sec */
		transmitLED = OFF;
		for (j = 0; j < 1000; j++) {
			for (Count = 0; Count < 250; Count++) {
				//        i += 1;
			}
		}

		/* LED4 ON for 0.1 sec */transmitLED = ON;
		for (j = 0; j < 200; j++) {
			for (Count = 0; Count < 500; Count++) {
				//        i += 1;
			}
		}
		transmitLED = OFF;

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
void startSampling(void) {
	samplingStatus = SAMPLING;

	sampleTimer_Enable();

	while (samplingStatus == SAMPLING) {
		;

		__DI();
		u16TempCBufferBytes = u16CBufferBytes;
		__EI();
		if (u16TempCBufferBytes >= BLOCK_SIZE) {
			transmitLED = ON;
			//ReadDataStop = TRUE; // Stop data read while writing to SD card
			SD_CBufferDataSectorWrite(); // write block to SD card
			ReadDataStop = FALSE;
			transmitLED = OFF;
		}

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
		for (;;) {
			pulseReceiveLED(1);
		}

	} else {
		/* SD Card present, read the Master Block (Boot Sector) of the memory */
		FAT_Read_Master_Block(); // in Fat.c

		/* Make sure file "FILE.TXT" exists */
		u8sd_status = FAT_FileOpen("FILE.TXT", MODIFY); // Attempt to open file

		pulseReceiveLED(1);

		/* Does file exist? */
		if (u8sd_status == FILE_NOT_FOUND) {

			transmitLED = ON;

			/* If the file does not exist, it is created */
			u8sd_status = FAT_FileOpen("FILE.TXT", CREATE);

			/* Set "u32DataSector" to the first data sector to be written.
			 u32DataSector is used in function FAT_DataSectorWrite() in Fat.c
			 for writing a block of data to the SD card. */

			// Create the FAT for the project data file
			for (i = 0; i < NUM_CLUSTERS; i++) {
				(void) FAT_Entry(WHandler.CurrentFatEntry,
						WHandler.CurrentFatEntry + 1, WRITE_ENTRY);
				WHandler.CurrentFatEntry++;
				WHandler.File_Size += CLUSTER_SIZE; // update the file size, 32768 bytes per cluster
			}
			transmitLED = OFF;

		}

		/* Use this to append to an existing file */
//       u32DataSector= u16FAT_Data_BASE + WHandler.ClusterIndex + (WHandler.CurrentFatEntry-2)*u16FAT_Cluster_Size;
		/* Use this to place the first data block at the start of the SD Card data region. */
		u32DataSector = u16FAT_Data_BASE;

		/* Erase the SD Card data area. Return OK if successful. */
//    result = SD_Erase_Blocks(u16FAT_Data_BASE,u16FAT_Data_BASE + 8192); // 4MB
		result = SD_Erase_Blocks(u16FAT_Data_BASE, u16FAT_Data_BASE + 2000000); // 1GB
		if (result != OK) {
			FAT_FileClose();
			for (;;) {
				pulseTransmitLED(1);
			}
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
	//if ((u16CBufferBytes < (CBufferSize - RECORD_SIZE))
	if (ReadDataStop != TRUE) {
		/* Place a sample line in the circular buffer */
		writeRecordToCBuffer();
	}
}

/**
 * Write the latest record to the circular buffer in CSV format.
 * The format is as follows: [Record number],[Z1],[Z2],[Z3][CR][LF]
 */
void writeRecordToCBuffer() {
	UINT8 i;
	UINT8 recordUpper;
	UINT8 recordLower;

// Write record number to buffer, MSB first
	recordUpper = recordNumber >> 8;
	recordLower = recordNumber & 0x00FF;
	ToCBuffer(recordUpper);
	ToCBuffer(recordLower);

// Write accelerometer data to buffer in csv format. (again, MSB first)
	for (i = 0; i < NUM_CHANNELS; i++) {
		ToCBuffer(',');
		ToCBuffer(samples[i] >> 8);
		ToCBuffer(samples[i] & 0xFF);
	}

// New line
	ToCBuffer(CR); // add Carriage Return terminator 0x0D
	ToCBuffer(LF); // add Line Feed terminator 0x0A
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

	if (u16CBufferGetter >= CBufferSize) { // check for buffer wrap around
		u16CBufferGetter = 0x0000;
	}

	u16CBufferBytes--;

	returnbyte = u8CircularBuffer[u16CBufferGetter++];
	__EI();
	return (returnbyte);
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
void Sample_Accel(void) {
	static uint16_t sampleResult;
	UINT8 adc_result;
	UINT8 i;

// Sequentially perform ADC conversions on each channel
	receiveLED = ON;
	for (u8Channel = 0; u8Channel < NUM_CHANNELS; u8Channel++) {
		//adc_result = AD1_MeasureChan(TRUE, u8Channel);
		//AD1_GetChanValue16(u8Channel, &sampleResult);
		//samples[u8Channel] = sampleResult;
		
		samples[u8Channel] = recordNumber + 1;
	}

	receiveLED = OFF;
	recordNumber++;
}

/**
 * Interrupt service routine for ADC conversion complete
 * Data is copied to a temporary buffer
 */
void ADC_ISR(void) {
	static uint16_t sampleResult;
	AD1_GetChanValue16(u8Channel, &sampleResult);

	samples[u8Channel] = recordNumber;
	//samples[u8Channel] = sampleResult;

	//u8AccelSamples[u8Channel << 1] = ADCRH; // MSB
	//u8AccelSamples[(u8Channel << 1) + 1] = ADCRL; // LSB
}

/**
 * Toggle the power LED (LED1)
 */
void togglePowerLED(void) {
	if (powerLED == ON) {
		powerLED = OFF;
	} else {
		powerLED = ON;
	}
}

/**
 * Toggle the status LED (LED2)
 */
void toggleStatusLED(void) {
	if (statusLED == ON) {
		statusLED = OFF;
	} else {
		statusLED = ON;
	}
}

/**
 * Toggle the receive LED (LED3)
 */
void toggleReceiveLED() {
	if (receiveLED == ON) {
		receiveLED = OFF;
	} else {
		receiveLED = ON;
	}
}

/**
 * Toggle the transmit LED (LED4)
 */
void toggleTransmitLED() {
	if (transmitLED == ON) {
		transmitLED = OFF;
	} else {
		transmitLED = ON;
	}
}

/**
 * Increment the time on the session clock by one second.
 * If the session limits are reached, sampling is stopped
 */
void incrementRunningClock(void) {
	sessionRuntime++;

	if (sessionRuntime > TOTAL_RUNTIME) {
		stopSampling();
	}
}

/**
 * Stop recording samples by stopping periodic interrupts.
 */
void stopSampling(void) {
	samplingStatus = NOT_SAMPLING;
	powerLED = OFF;
	sampleTimer_Disable();
}
