
#include <Arduino.h>

const uint8_t displaySegments[10][7] = {
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
constexpr uint32_t BUZZER_INTERVAL = 500;
uint32_t lastBuzzerTime = 0;
bool buzzerState = false;

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
void buzzer_control(uint16_t waterLevel);
void display_water_level(uint16_t waterLevel);

void loop() {
    uint16_t water = analogRead(WATER_PIN);
    Serial.printf("Water RAW: %u\n", water);
    display_water_level(water);
    light_control(water);
    buzzer_control(water);
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

void buzzer_control(uint16_t waterLevel) {
    if (waterLevel < 1400) {
      buzzerState = false;
      digitalWrite(BUZZER_PIN, LOW);
      lastBuzzerTime = millis();
      return;
    }

    uint32_t currentTime = millis();

    if (currentTime - lastBuzzerTime >= BUZZER_INTERVAL) {
      lastBuzzerTime = currentTime;
      buzzerState = !buzzerState;
      digitalWrite(BUZZER_PIN, buzzerState ? HIGH : LOW);
    }
}

uint8_t calculateWaterLevel(uint16_t waterLevel) {
  return constrain(waterLevel / 200, 0, 9);
}

void display_water_level(uint16_t waterLevel) {
  uint8_t level = calculateWaterLevel(waterLevel);
  for (uint8_t i = 0; i < 7; i++) {
      digitalWrite(SEGMENT_PINS[i], displaySegments[level][i]);
    }
}
