# micobit-messages
Small library built on top of the micro:bit radio driver which allows for packets to be address to individual micro:bit recievers.

Example of use:
```
#include "MicroBit.h"
#include "MessageService.h"

// Microbit components
MicroBit uBit;
MessageService messages(&uBit);

// Invoked whenever a message is recieved from the messages system
void onMessageReceived(uint32_t from, uint8_t *buffer, int len) {
	uBit.serial.send("Message received: ");
	uBit.serial.send(buffer, len);
}

// Entry point to the program
int main() {
	// Initialise the micro:bit
	uBit.init();

	// Set the callback for the messages module
	messages.setCallback(onMessageReceived);

	// Start the radio module - First make sure that bluetooth is disabled or
	// this is not going to work.
	uBit.serial.send("Attempting to start radio module: ");
	int result = uBit.radio.enable();
	if (result == MICROBIT_OK) {
		uBit.serial.send("MICROBIT_OK\n");
	} else if (result == MICROBIT_NOT_SUPPORTED) {
		uBit.serial.send("MICROBIT_NOT_SUPPORTED\n");
	}

	// Broadcast an example message
	// NOTE: Instead of 0 you could enter the serial number of any micro:bit in the area.
	messages.send(0, "Hello, World!");

	// Terminate the main fiber gracefully so messages can be received after main ends.
	release_fiber();
}
```
