/** ###################################################################
 **     Filename    : ProcessorExpert.c
 **     Project     : ProcessorExpert
 **     Processor   : MC9S08JM60CLHE
 **     Version     : Driver 01.12
 **     Compiler    : CodeWarrior HCS08 C Compiler
 **     Date/Time   : 2013-05-27, 11:04, # CodeGen: 0
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/* MODULE ProcessorExpert */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "powerLED.h"
#include "statusLED.h"
#include "Accelerometer1.h"
#include "sampleTimer.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#define NUM_ACCELEROMETERS 3

// Number of records in the sample buffer.
#define SAMPLE_BUFFER_SIZE 250

/**
 * Accelerometer packet
 */
typedef struct {
	uint16_t accelerometerData[NUM_ACCELEROMETERS];
} SampleData;

/**
 * Circular sample buffer
 */
typedef struct {
	int size; // Size of buffer in samples
	int reader; // Reader pointer position
	int writer; // Writer pointer position
	long readCount; // Total number of reads performed
	long writeCount; // Total number of writes performed
	SampleData sample[SAMPLE_BUFFER_SIZE];
	uint16_t recordNumber[SAMPLE_BUFFER_SIZE];
	bool isHigh; // Flag set if there is more than a set minimum in the buffer.
} SampleBuffer;

SampleBuffer sampleBuffer;
SampleData currentSample;

uint16_t sampleCounter;

/**
 * Increment the reader position in the buffer.
 * The reader should never overtake the writer.
 *
 * @param cb Circular buffer that the reader belongs to.
 */
void incrementSampleReader(SampleBuffer *cb) {
	cb->reader = (cb->reader + 1) % cb->size;
	cb->readCount++;

	// Check if the last read ha caused the buffer to become low.
//			if (isInBufferLow(cb)) {
//				cb->isLow = 1;

}

/** 
 * Increment the writer position in the buffer.
 * The writer should never overtake the reader.
 *
 * @param cb Circular buffer that the reader belongs to.
 */
void incrementSampleWriter(SampleBuffer * cb) {
	cb->writer = (cb->writer + 1) % cb->size;
	cb->writeCount++;

//    if (isInBufferLow(cb) == 0) {
//        cb->isLow = 0;
//        switchOffLED2();
//    }
}

/** 
 * Write sample data to the current position in the buffer.
 *
 * @param cb	Circular buffer to write to.
 * @param sample	Sample data to be written.
 */
void writeSampleToInBuffer(SampleBuffer *cb, SampleData * sample) {
    cb->sample[cb->writer] = *sample;
}

void main(void) {
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */

	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
	/*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
	for (;;) {
	}
	/*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END ProcessorExpert */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.0 [05.03]
 **     for the Freescale HCS08 series of microcontrollers.
 **
 ** ###################################################################
 */
