#define LED_STATUS 7
#define SIG_BUTTON 8
#include "env.h"
#include "utility.h"
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int lastmsg = 1;
String theMessage = "";

void ring() {
  blink(LED_STATUS, 100, 50); 
}

void setup() {
  pinMode(LED_STATUS, OUTPUT);
  pinMode(SIG_BUTTON, INPUT);

  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
}

// http://shanes.net/another-nrf24l01-sketch-string-sendreceive/
void loop() {
  /*if (pinIsHigh(SIG_BUTTON)) {*/
    /*ring();*/
  /*}*/

  /*delay(10);*/

    if (radio.available()){
    bool done = false;  
      done = radio.read(msg, 1); 
      char theChar = msg[0];
      if (msg[0] != 2){
        theMessage.concat(theChar);
        }
      else {
       Serial.println(theMessage);
       theMessage= ""; 
      }
   }

}
