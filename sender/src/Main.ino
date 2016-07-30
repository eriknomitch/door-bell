// ===============================================
// SENDER->MAIN ==================================
// ===============================================
#include "env.h"
#include "utility.h"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

#include <Button.h>

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
RF24 radio(9,10);

Button button = Button(SIG_BUTTON, 0);

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
  Serial.println("Sending 'pressed' payload...");
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
  delay(3000);
}

// -----------------------------------------------
// LOOP ------------------------------------------
// -----------------------------------------------
// http://shanes.net/another-nrf24l01-sketch-string-sendreceive/
void loop() {
  if (button.uniquePress()) {
    sendPressedPayload();
  }
}

