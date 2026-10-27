
#include <Arduino.h>

const int displaySegments[10][7] = {
  {1,1,1,0,1,1,1}, // 0
  {0,1,0,0,1,0,0}, // 1
  {1,0,1,1,1,1,0}, // 2
  {0,1,1,1,1,1,0}, // 3
  {0,1,0,1,1,0,1}, // 4
  {0,1,1,1,0,1,1}, // 5
  {1,1,1,1,0,1,1}, // 6
  {0,1,0,0,1,1,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {0,1,1,1,1,1,1}  // 9
};

// Water sensor
constexpr uint8_t WATER_PIN = 7;

// RGB LED
constexpr uint8_t RED_PIN = 4;
constexpr uint8_t GREEN_PIN = 5;
constexpr uint8_t BLUE_PIN = 6;

// Buzzer
constexpr uint8_t BUZZER_PIN = 16;

// 7-segment display: a, b, c, d, e, f, g
const uint8_t SEGMENT_PINS[] = {8, 9, 10, 11, 12, 13, 14};

void setup() {
    Serial.begin(115200);
    analogReadResolution(12);

    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);

    for (uint8_t pin : SEGMENT_PINS) {
        pinMode(pin, OUTPUT);
    }

    pinMode(BUZZER_PIN, OUTPUT);
}

void light_control(uint16_t waterLevel);
void buzzer_high();
void display_water_level(uint16_t waterLevel);

void loop() {
    uint16_t water = analogRead(WATER_PIN);
    Serial.printf("Water RAW: %u\n", water);
    display_water_level(water);
    light_control(water);
    if (water > 1400) {
      buzzer_high();
    }

}

void light_control(uint16_t waterLevel) {
  if (waterLevel < 700) {
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, LOW);
    digitalWrite(RED_PIN, LOW);
  } else if (waterLevel < 1400) {
    digitalWrite(BLUE_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(RED_PIN, LOW);
  } else {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(BLUE_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
  }
}

void buzzer_high() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(500);
  digitalWrite(BUZZER_PIN, LOW);
}

uint8_t calculateWaterLevel(uint16_t waterLevel) {
    return constrain(waterLevel / 200, 0, 9);
}

void display_water_level(uint16_t waterLevel) {
  for (uint8_t pin : SEGMENT_PINS) {
    digitalWrite(pin, displaySegments[calculateWaterLevel(waterLevel)][pin-8]);
  }
}
