// Encoder_count.h

#ifndef Encoder_count_h
#define Encoder_count_h

#include "Arduino.h"
#include <Wire.h>

class EncoderCount {
public:
  EncoderCount(TwoWire *I2Cbus, char encoder_direction, uint8_t i2c_address = 0x36);
  int32_t getValue();

private:
  uint16_t readAngle();
  TwoWire *_I2Cbus;
  char _encoder_direction;
  uint8_t _i2c_address;
  uint16_t prevAngle = 0;
  int32_t tickCount = 0;
  const uint16_t ticksPerRevolution = 1024; // Adjust as needed
};

#endif
