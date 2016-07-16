#define LED_STATUS 7
#define SIG_BUTTON 8

void setup() {
  pinMode(LED_STATUS, OUTPUT);
  pinMode(SIG_BUTTON, INPUT);
}

void loop() {
  digitalWrite(LED_STATUS, digitalRead(SIG_BUTTON));

  delay(100);
}
