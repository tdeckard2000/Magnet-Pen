/*
  magnetPen - HiLetgo ESP8266 NodeMCU (ESP-12E)

  Using the HiLetgo ESP8266:
  1. Download the Arduino IDE, the latest version.
  2. Install the IDE
  3. Set up your Arduino IDE as: Go to File->Preferences and copy the URL below to get the ESP board manager extensions: http://arduino.esp8266.com/stable/package_esp8266com_index.json Placing the http:// before the URL lets the Arduino IDE use it...otherwise it gives you a protocol error.
  4. Go to Tools > Board > Board Manager> Type "esp8266" and download the Community esp8266 and install.
  5. Set up your chip as:
    Tools -> Board -> NodeMCU 1.0 (ESP-12E Module)
    Tools -> Flash Size -> 4M (3M SPIFFS)
    Tools -> CPU Frequency -> 80 Mhz
    Tools -> Upload Speed -> 921600
    Tools-->Port--> (whatever it is)

  Sequence after power-up:
    1. Assert the self-hold output.
    2. Disable Wi-Fi, then pulse the magnet output five times.
    3. Leave the magnet off and release the self-hold output.

  NodeMCU GPIO outputs are 3.3 V. Use GPIO pins through the board labels below;
  do not apply 5 V to ESP8266 GPIO pins.
*/

#include <ESP8266WiFi.h>

// NodeMCU labels map to GPIO numbers: D1 = GPIO5, D2 = GPIO4.
// These pins are not ESP8266 boot-mode strapping pins.
const uint8_t SELF_HOLD_PIN = D1;
const uint8_t MAGNET_PIN    = D2;

const unsigned long MAGNET_ON_MS  = 500;
const unsigned long MAGNET_OFF_MS = 500;
const uint8_t MAGNET_PULSE_COUNT = 5;

void setup() {
  // Keep the onboard blue LED lit (active-low on D4 on this board).
  // pinMode(D4, OUTPUT);
  // digitalWrite(D4, LOW);
  // Establish the power hold as early as the sketch can run.
  digitalWrite(SELF_HOLD_PIN, HIGH);
  pinMode(SELF_HOLD_PIN, OUTPUT);
  digitalWrite(SELF_HOLD_PIN, LOW);

  // Keep the magnet MOSFET off while Wi-Fi is shut down.
  digitalWrite(MAGNET_PIN, LOW);
  pinMode(MAGNET_PIN, OUTPUT);

  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();

  for (uint8_t pulse = 0; pulse < MAGNET_PULSE_COUNT; ++pulse) {
    digitalWrite(MAGNET_PIN, HIGH);
    delay(MAGNET_ON_MS);
    digitalWrite(MAGNET_PIN, LOW);
    delay(MAGNET_OFF_MS);
  }

  // Explicitly leave the magnet off, then drop the self-hold output.
  digitalWrite(MAGNET_PIN, LOW);
  digitalWrite(SELF_HOLD_PIN, HIGH);
}

void loop() {
  // The sequence runs once per power-up/reset.
}
