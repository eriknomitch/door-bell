// ===============================================
// SENDER->MAIN ==================================
// ===============================================
#include "env.h"
#include "utility.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

// -----------------------------------------------
// CONSTANTS -------------------------------------
// -----------------------------------------------
#define SERIAL_BAUD 9600
#define LED_STATUS 7
#define SIG_BUTTON 8

const uint64_t pipe = 0xE8E8F0F0E1LL;

// -----------------------------------------------
// GLOBALS ---------------------------------------
// -----------------------------------------------
//int msg[1];
RF24 radio(9,10);

unsigned long pressedLastSent = 0;

// -----------------------------------------------
// UTILITY ---------------------------------------
// -----------------------------------------------

void sendPressedPayload() {
  Serial.println("@sendPressedPayload()");
  int payload[2];
  payload[0] = 100;
  payload[0] = 2;

  radio.write(payload, 2);
}

void sendPressed() {

  if (millis() - pressedLastSent <= 1000) {
    Serial.println("Pressed too soon. Not sending.");
    return;
  }
 
  pressedLastSent = millis();

  Serial.println("Sending 'pressed'...");

  // It's pressed. Send the message.
  // ---------------------------------------------
  sendPressedPayload();

  Serial.println("Done.");
}

// -----------------------------------------------
// SETUP -----------------------------------------
// -----------------------------------------------
void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println("+ setup()");

  pinMode(LED_STATUS, OUTPUT);
  pinMode(SIG_BUTTON, INPUT);

  radio.begin();
  radio.openWritingPipe(pipe);
  //radio.printDetails();
  
  Serial.println("- setup()");
}

// -----------------------------------------------
// LOOP ------------------------------------------
// -----------------------------------------------
// http://shanes.net/another-nrf24l01-sketch-string-sendreceive/
void loop() {
 
  // Wait until the button is pressed
  //while (pinIsLow(SIG_BUTTON)) { delay(10); }

  delay(5000);
  sendPressedPayload();

  /*delay sending for a short period of time.  radio.powerDown()/radio.powerupp
  //with a delay in between have worked well for this purpose(just using delay seems to
  //interrupt the transmission start). However, this method could still be improved
  as I still get the first character 'cut-off' sometimes. I have a 'checksum' function
  on the receiver to verify the message was successfully sent.
  */
  /*
  radio.powerDown();
  delay(1000);
  radio.powerUp();
  */
}

