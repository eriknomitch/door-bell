#define LED_STATUS 7
#define SIG_BUTTON 8
#include "env.h"
#include "utility.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

#define SERIAL_BAUD 9600

int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

void ring() {
  blink(LED_STATUS, 100, 50); 
}

void setup() {
  pinMode(LED_STATUS, OUTPUT);
  pinMode(SIG_BUTTON, INPUT);

  Serial.begin(SERIAL_BAUD);

  radio.begin();
  radio.openWritingPipe(pipe);
}

// http://shanes.net/another-nrf24l01-sketch-string-sendreceive/
void loop() {
 
  // Wait until the button is pressed
  while (pinIsLow(SIG_BUTTON)) { delay(1); }

  // It's pressed. Send the message.
  String theMessage = "Hello there!";
  int messageSize = theMessage.length();
  for (int i = 0; i < messageSize; i++) {
    int charToSend[1];
    charToSend[0] = theMessage.charAt(i);
    radio.write(charToSend,1);
  }

  //send the 'terminate string' value...  
  msg[0] = 2; 
  radio.write(msg,1);

  /*delay sending for a short period of time.  radio.powerDown()/radio.powerupp
  //with a delay in between have worked well for this purpose(just using delay seems to
  //interrupt the transmission start). However, this method could still be improved
  as I still get the first character 'cut-off' sometimes. I have a 'checksum' function
  on the receiver to verify the message was successfully sent.
  */
  radio.powerDown();
  delay(1000);
  radio.powerUp();
}
