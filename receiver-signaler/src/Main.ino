#define LED_STATUS 7
#define SIG_BUTTON 8
#include "env.h"
#include "utility.h"

#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

#include "FastLED.h"

FASTLED_USING_NAMESPACE

bool isGreen = true;

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    6
//#define CLK_PIN   4
#define LED_TYPE    WS2812
#define COLOR_ORDER RGB
#define NUM_LEDS    24
#define PIN_BUZZER 2

#define SERIAL_BAUD 9600

CRGB leds[NUM_LEDS];

#define BRIGHTNESS          30

#define RING_DELAY 25
#define RING_TIMES 30

int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int lastmsg = 1;
String theMessage = "";

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void ringTimes(int times) {
  for (int i = 0; i < times; i ++) {
    for (int l = 0; l < NUM_LEDS; l++) {
      if (!isGreen) {
        leds[l] = CRGB::Green;
      } else {
        leds[l] = CRGB::Blue;
      }
    }
    isGreen = !isGreen;

    pinLow(PIN_BUZZER);

    FastLED.show();
    delay(RING_DELAY);
    // Now turn the LED off, then pause
    for (int l = 0; l < NUM_LEDS; l++) {
      leds[l] = CRGB::Black;
    }
    pinHigh(PIN_BUZZER);
    FastLED.show();
    delay(RING_DELAY);
  }
  pinLow(PIN_BUZZER);
}

void ring() {
  return ringTimes(RING_TIMES);
}

void setup() {
  delay(3000); // 3 second delay for recovery

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  pinMode(LED_STATUS, OUTPUT);
  pinMode(SIG_BUTTON, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  Serial.begin(SERIAL_BAUD);
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();

  ringTimes(5);
}

// http://shanes.net/another-nrf24l01-sketch-string-sendreceive/
void loop() {

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
       ring();
      }
   }

}
