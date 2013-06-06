#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

// Number of for-count cycles in a millisecond
int TIME_FACTOR = 2;

// Data directions for IO pins
byte OUTPUT = 1;
byte INPUT = 0;

// Digital data states
const byte HIGH = 1;
const byte LOW = 0;

// LED power states. Pins are active low.
const byte ON = LOW;
const byte OFF = HIGH;

// Power LED configuration
#define powerLed PTGD_PTGD1;
#define powerLedPinMode PTGDD_PTGDD1;

// Status LED configuration
#define statusLed PTGD_PTGD0;
#define statusLedPinMode PTGDD_PTGDD0;

// Transmit LED configuration
#define transmitLed PTFD_PTFD0;
#define transmitLedPinMode PTFDD_PTFDD0;

// Receive LED configuration
#define receiveLed PTFD_PTFD1;
#define receiveLedPinMode PTFDD_PTFDD1;

/**
 * Pause the program for a specified amount of time.
 * @param milliseconds Delay period in milliseconds.
 */
void delay(unsigned long milliseconds) {
	int i;

	for (i = 0; i < (milliseconds * TIME_FACTOR); i++) {
	}
}

/**
 * Set pin modes and default states for the LED indicators.
 * All LEDs will be on with the exception of the power indicator.
 */
void initialiseLEDs() {
	powerLedPinMode = OUTPUT;
	statusLedPinMode = OUTPUT;
	transmitLedPinMode = OUTPUT;
	receiveLedPinMode = OUTPUT;

	performLedCheck();
	resetLeds();
}

/**
 * Consecutively turn on each LED
 */
void performLedCheck() {
	powerLed = ON;
	delay(200);
	statusLed = ON;
	delay(200);
	transmitLed = ON;
	delay(200);
	receiveLed = ON;
	delay(1000);
}

/**
 * Switch all LEDs off.
 */
void resetLeds() {
	powerLed = OFF;
	statusLed = OFF;
	transmitLed = OFF;
	receiveLed = OFF;
}

/**
 * Cycle the power LED continuously. There is a one second delay between transitions.
 */
void testPowerLED() {
	powerLedPinMode = OUTPUT;

	while (1) {
		powerLed = ON;
		delay(1000);

		powerLed = OFF;
		delay(1000);
	}
}

/**
 * Main Loop - Functional code
 */
void main(void) {

	EnableInterrupts;

	for (;;) {
		testPowerLED();
		__RESET_WATCHDOG(); /* feeds the dog */
	} /* loop forever */
}
