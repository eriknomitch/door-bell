#define LED_STATUS 7
#define SIG_BUTTON 8
#include "env.h"
#include "utility.h"

void ring() {
  blink(LED_STATUS, 100, 50); 
}

void setup() {
  pinMode(LED_STATUS, OUTPUT);
  pinMode(SIG_BUTTON, INPUT);
}

void loop() {
  if (pinIsHigh(SIG_BUTTON)) {
    ring();
  }

  delay(10);
}
