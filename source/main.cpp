#include "MicroBit.h"
#include "MessageService.h"

// Functions
ManagedString getSerial();
void onMessageReceived(uint32_t, uint8_t*, int);


// Microbit components
MicroBit uBit;
MessageService messages(&uBit);


/**
  * Entry point to the program
	*/
int main() {
	// Initialise the micro:bit
	uBit.init();

	// Set the callback for the messages module
	messages.setCallback(onMessageReceived);

	// Start the radio module
	printf("Attempting to start radio module: ");
	int result = uBit.radio.enable();
	if (result == MICROBIT_OK) {
		printf("MICROBIT_OK\n");
	} else if (result == MICROBIT_NOT_SUPPORTED) {
		printf("MICROBIT_NOT_SUPPORTED\n");
	}

	// Print the serial number
	printf("My Serial number: %s\n", getSerial().toCharArray());

	// Send out an example message
	uBit.serial.send("Sending example radio message.\n");
	ManagedString hello = "Hello, World!\n";
	messages.send(2, hello);
	uBit.serial.send("Message has been sent.\n");

	// Terminate the main fiber
  release_fiber();
}


/**
  * Returns the serial of the device as a string (Correction on the default getSerial() function)
	*/
ManagedString getSerial() {
	// Convert the serial number to a string
	char str[10];
	sprintf(str, "%u", (unsigned int)microbit_serial_number());

	// Return the resulting string
	return ManagedString(str);
}


/**
  * Invoked whenever a message is recieved from the messages system
	*/
void onMessageReceived(uint32_t from, uint8_t *buffer, int len) {
	// Log the invokation
	printf("Invoked: onMessageReceived(%u, %p, %d)\n", (unsigned int)from, buffer, len);


	// Send the message to the serial port
	uBit.serial.send(buffer, len);

}
