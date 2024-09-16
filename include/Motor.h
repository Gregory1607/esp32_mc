// Motor.h

#ifndef Motor_h
#define Motor_h

#include "Arduino.h"

class Motor {
public:
  Motor(uint8_t lpwm_pin, uint8_t rpwm_pin, uint8_t l_en_pin, uint8_t r_en_pin, uint8_t lpwm_channel, uint8_t rpwm_channel);
  void Drive(char direction, int pwmValue);
  void Stop();

private:
  uint8_t _lpwm_pin;
  uint8_t _rpwm_pin;
  uint8_t _l_en_pin;
  uint8_t _r_en_pin;
  uint8_t _lpwm_channel;
  uint8_t _rpwm_channel;
};

#endif
