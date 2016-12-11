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
  * Constructor.
  *
  * Initialise the messages service.
  *
  */
MessageService::MessageService(MicroBit *uBit)
{
  // Store reference to the radio module
  this->radio = &(uBit->radio);

  // Subscribe to the message bus for when packets are recieved
  uBit->messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, this, &MessageService::onRadioPacketReceived);
}




void MessageService::onRadioPacketReceived(MicroBitEvent) {
  printf("onRadioPacketReceived) invoked.\n");

  // Get the packet
  PacketBuffer packet = this->radio->datagram.recv();
  int rssi = packet.getRSSI();
  int len = packet.length();
  uint8_t *payload = packet.getBytes();

  // Sender and reciever
  uint8_t sender = bytes_to_uint(&payload[0]);
  uint8_t receiver = bytes_to_uint(&payload[4]);

  // Resize the payload
  payload = payload+8;
  len = len - 8;


  // Logging
  printf("sender = %u\n", sender);
  printf("receiver = %u\n", receiver);
  printf("len = %u\n", len);

  for (int i = 0; i < len; i++) {
    printf("%c", payload[i]);

  }
}

void MessageService::send(uint32_t sender, uint32_t receiver, uint8_t *buffer, int len) {
  // Create memory for the packet
  uint8_t packet[len+PACKET_HEADER_SIZE];

  // Store the sender and reciever in the packet
  int_to_bytes(sender, &packet[0]);
  int_to_bytes(receiver, &packet[4]);

  // Copy the data to be transmitted
  memcpy(&packet[8], &buffer[0], len);

  // Send the packet over the radio
  int result = this->radio->datagram.send(&packet[0], len+PACKET_HEADER_SIZE);
  if (result != MICROBIT_OK) {
    printf("sddasd");
  }
}


//void MessageService::send(uint32_t receiver, uint8_t *data, int len) {
//  printf("send(%u, %d, %p) invoked.\n", receiver, len, data);
//}
