// Motor.cpp

#include "Arduino.h"
#include "Motor.h"

#ifdef ESP32
#include "esp32-hal-ledc.h"
#endif

Motor::Motor(uint8_t lpwm_pin, uint8_t rpwm_pin, uint8_t l_en_pin, uint8_t r_en_pin, uint8_t lpwm_channel, uint8_t rpwm_channel) {
  _lpwm_pin = lpwm_pin;
  _rpwm_pin = rpwm_pin;
  _l_en_pin = l_en_pin;
  _r_en_pin = r_en_pin;
  _lpwm_channel = lpwm_channel;
  _rpwm_channel = rpwm_channel;

  pinMode(_lpwm_pin, OUTPUT);
  pinMode(_rpwm_pin, OUTPUT);
  pinMode(_l_en_pin, OUTPUT);
  pinMode(_r_en_pin, OUTPUT);
  
  digitalWrite(_l_en_pin, HIGH);
  digitalWrite(_r_en_pin, HIGH);

#ifdef ESP32
  ledcSetup(_lpwm_channel, 5000, 8);
  ledcSetup(_rpwm_channel, 5000, 8);
  ledcAttachPin(_lpwm_pin, _lpwm_channel);
  ledcAttachPin(_rpwm_pin, _rpwm_channel);
#endif
}

void Motor::Drive(char direction, int pwmValue) {
  pwmValue = constrain(pwmValue, 0, 255);

#ifdef ESP32
  if (direction == 'F') {
    ledcWrite(_lpwm_channel, 0);
    ledcWrite(_rpwm_channel, pwmValue);
  } else if (direction == 'R') {
    ledcWrite(_lpwm_channel, pwmValue);
    ledcWrite(_rpwm_channel, 0);
  } else {
    Stop();
  }
#else
  // Fallback for non-ESP32 boards (using standard Arduino functions)
  if (direction == 'F') {
    analogWrite(_lpwm_pin, 0);
    analogWrite(_rpwm_pin, pwmValue);
  } else if (direction == 'R') {
    analogWrite(_lpwm_pin, pwmValue);
    analogWrite(_rpwm_pin, 0);
  } else {
    Stop();
  }
#endif
}

void Motor::Stop() {
#ifdef ESP32
  ledcWrite(_lpwm_channel, 0);
  ledcWrite(_rpwm_channel, 0);
#else
  analogWrite(_lpwm_pin, 0);
  analogWrite(_rpwm_pin, 0);
#endif
}
