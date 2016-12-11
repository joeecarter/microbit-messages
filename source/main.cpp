#include "MicroBit.h"
#include "MessageService.h"

void onRadioRecieved(MicroBitEvent);

MicroBit uBit;
MessageService messages(&uBit);

int main() {
	// Initialise the micro:bit
	uBit.init();

	// Start the radio module
	printf("Attempting to start radio module: ");
	int result = uBit.radio.enable();
	if (result == MICROBIT_OK) {
		printf("MICROBIT_OK\n");
	} else if (result == MICROBIT_NOT_SUPPORTED) {
		printf("MICROBIT_NOT_SUPPORTED\n");
	}


	// Read the serial number
	uint32_t serial_number = microbit_serial_number();

	char serial_number_str[10];
	sprintf(serial_number_str, "%u", (unsigned int)serial_number);

	uBit.serial.send("Serial number: ");
	uBit.serial.send(ManagedString(serial_number_str));
	uBit.serial.send("\n");

	// Send out an example message
	uBit.serial.send("Sending example radio message.\n");
	ManagedString hello = "Hello, World!\n";
	uint8_t *bytes = (uint8_t*)hello.toCharArray();

	// Send the data
	messages.send(1, 2, bytes, hello.length());
	uBit.serial.send("Message has been sent.\n");

	// Terminate the main fiber
  release_fiber();
}
