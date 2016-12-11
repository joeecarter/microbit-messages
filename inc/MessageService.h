#ifndef MESSAGES_H
#define MESSAGES_H

#include "MicroBit.h"


/**
  * TODO: Comment!
  *
  */
#define PACKET_HEADER_SIZE 8
#define PACKET_PAYLOAD_MAX_SIZE 200
#define PACKET_MAX_SIZE PACKET_HEADER_SIZE + PACKET_PAYLOAD_MAX_SIZE


/**
  * Helper function which converts an unsigned integer into an array of bytes
  */
void uint_to_bytes(uint32_t value, uint8_t *buffer);


/**
  * Helper function that converts an array of 4 bytes into an unsigned integer
  */
uint32_t bytes_to_uint(uint8_t *buffer);


/**
  * Provides a simple message service abstraction built ontop of the radio drivers.
  * This system takes control of the radio and provides a small unicast messaging
  * service to allow communication between the microbits in the surrounding area.
  *
  * You are going to need to know the
  *
  */
class MessageService
{
  private:
  MicroBitRadio *radio;

  /**
    * Receive buffer
    */
  uint8_t buffer[PACKET_MAX_SIZE];

  /**
    * Handler for listening to the messages that the radio module has picked up
    */
  void onRadioPacketReceived(MicroBitEvent);

  public:

  /**
    * Constructor.
    *
    * Initialise the messages service.
    *
    */
  MessageService(MicroBit *uBit);


  /**
    * TODO: Comment and make this provate again later.
    */
  void send(uint32_t sender, uint32_t receiver, uint8_t *buffer, int len);

  /**
    * Sends some data to the micro:bit with the serial number receiver.
    */
  void send(uint32_t receiver, uint8_t *buffer, int len);

};



#endif
