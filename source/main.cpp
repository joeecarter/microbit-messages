#include "MicroBit.h"

MicroBit uBit;

int main() {
	uBit.serial.send("Hello, World!\n");
}

void helloworld() {
	printf("Hello, World!\n");
}
