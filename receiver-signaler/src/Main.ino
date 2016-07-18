// ===============================================
// RECEIVER->MAIN ================================
// ===============================================
#include "env.h"
#include "utility.h"

#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define SERIAL_BAUD 9600

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define LED_STATUS  7
#define SIG_BUTTON  8
#define DATA_PIN    6
#define LED_TYPE    WS2812
#define COLOR_ORDER RGB
#define NUM_LEDS    24
#define PIN_BUZZER  3

CRGB leds[NUM_LEDS];

#define BRIGHTNESS 5

#define RING_DELAY 25
#define RING_TIMES 30

bool isGreen = true;

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
    analogWrite(PIN_BUZZER, 500);

    FastLED.show();
    delay(RING_DELAY);
  }
  pinLow(PIN_BUZZER);
}

void ring() {
  return ringTimes(RING_TIMES);
}

// -----------------------------------------------
// SETUP -----------------------------------------
// -----------------------------------------------
void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println("+ setup()");

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  pinMode(LED_STATUS, OUTPUT);
  pinMode(SIG_BUTTON, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  
  ringTimes(5);
  Serial.println("- setup()");

  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
}

// http://shanes.net/another-nrf24l01-sketch-string-sendreceive/
void loop() {
  if (radio.available()) {
    int payload[2];
    radio.read(payload, 2);
    Serial.println("Received:");
    //ring();
  }
}
