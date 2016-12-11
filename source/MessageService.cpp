#include "MicroBit.h"
#include "MessageService.h"


/**
  * Helper function which converts an unsigned integer into an array of bytes
  */
void int_to_bytes(uint32_t value, uint8_t *buffer) {
    for (int i = 0; i < 4; i++) buffer[3-i] = (value >> (i * 8));
}


/**
  * Helper function that converts an array of 4 bytes into an unsigned integer
  */
uint32_t bytes_to_uint(uint8_t *buffer) {
  return buffer[3] | (buffer[2] << 8) | (buffer[1] << 16) | (buffer[0] << 24);
}


/**
  *Message bus listener which is invoked whenever the radio receives a new packet
  */
void MessageService::onRadioPacketReceived(MicroBitEvent) {
  // Get the packet
  PacketBuffer packet = this->radio->datagram.recv();
  int packet_len = packet.length();

  // If the packet is too small then ignore it
  if (packet_len < PACKET_HEADER_SIZE+1) {
    return;
  }

  // Get the payload of the packet
  uint8_t *payload = packet.getBytes();

  // Get the sender and reciever and resize the payload
  uint32_t sender = bytes_to_uint(&payload[0]);
  uint32_t receiver = bytes_to_uint(&payload[4]);
  payload += 8; packet_len -=  8;

  // If this packet for this reciever then process it
  if (receiver == microbit_serial_number() || receiver == 0) {
    // Invoke the callback if it exists
    if (this->callback != NULL) {
      (*(this->callback))(sender, payload, packet_len);
    }
  }
}


/**
  * Sends some data to the micro:bit with the serial number receiver. This has the
  * option to specify the sender which should usually be the serial number of this micro:bit.
  */
void MessageService::send(uint32_t sender, uint32_t receiver, uint8_t *buffer, int len) {
  // Create memory for the packet
  uint8_t packet[len+PACKET_HEADER_SIZE];

  // Store the sender and reciever in the packet
  int_to_bytes(sender, &packet[0]);
  int_to_bytes(receiver, &packet[4]);

  // Copy the data to be transmitted
  memcpy(&packet[8], &buffer[0], len);

  // Send the packet over the radio
  this->radio->datagram.send(&packet[0], len+PACKET_HEADER_SIZE);
}


/**
  * Constructor: Initialise the messages service.
  */
MessageService::MessageService(MicroBit *uBit)
{
  // Store reference to the radio module
  this->radio = &(uBit->radio);
  this->callback = NULL;

  // Subscribe to the message bus for when packets are recieved
  uBit->messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, this, &MessageService::onRadioPacketReceived);

  // Print out the serial number to the serial port
  unsigned int serial_number = (unsigned int)microbit_serial_number();
  printf("My serial number is %u[0x%x]\n", serial_number, serial_number);
}


/**
  * Sends some data to the micro:bit with the serial number receiver.
  */
void MessageService::send(uint32_t receiver, uint8_t *buffer, int len) {
  send(microbit_serial_number(), receiver, buffer, len);
}


/**
  * Sends a string to the micro:bit with the serial number receiver.
  */
void MessageService::send(uint32_t receiver, ManagedString string) {
  send(receiver, (uint8_t*)string.toCharArray(), string.length());
}


/**
  * Sets the callack that is invoked whenever a new message has been received.
  */
void MessageService::setCallback(void (*callback)(uint32_t, uint8_t*, int)) {
  this -> callback = callback;
}
