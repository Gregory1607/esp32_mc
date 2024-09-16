// Encoder_count.cpp

#include "Arduino.h"
#include "Encoder_count.h"
#include <Wire.h>

EncoderCount::EncoderCount(TwoWire *I2Cbus, char encoder_direction, uint8_t i2c_address) {
  _I2Cbus = I2Cbus;
  _encoder_direction = encoder_direction;
  _i2c_address = i2c_address;
  prevAngle = readAngle(); // Initialize previous angle
}

int32_t EncoderCount::getValue() {
  uint16_t currentAngle = readAngle();

  int16_t angleDiff = currentAngle - prevAngle;

  // Handle rollover
  if (angleDiff > 2047) {
    angleDiff -= 4096;
  } else if (angleDiff < -2047) {
    angleDiff += 4096;
  }

  // Convert angle difference to tick difference
  int32_t tickDiff = (int32_t)angleDiff * ticksPerRevolution / 4096;

  // Update tick count based on direction
  if (_encoder_direction == '+') {
    tickCount += tickDiff;
  } else if (_encoder_direction == '-') {
    tickCount -= tickDiff;
  }

  prevAngle = currentAngle;

  return tickCount;
}

uint16_t EncoderCount::readAngle() {
  // Begin transmission to the encoder's I2C address
  _I2Cbus->beginTransmission(_i2c_address);
  _I2Cbus->write(0x0C); // RAW ANGLE register address (confirm with datasheet)
  
  // End transmission and check for errors
  uint8_t error = _I2Cbus->endTransmission(false); // false to send a repeated start
  if (error != 0) {
    Serial.print("I2C Transmission Error: "); Serial.println(error);
    return prevAngle; // Return previous angle on error
  }

  // Request 2 bytes from the encoder
  uint8_t bytesReceived = _I2Cbus->requestFrom((int)_i2c_address, 2);
  if (bytesReceived != 2) {
    Serial.print("I2C Read Error, bytes received: "); Serial.println(bytesReceived);
    return prevAngle; // Return previous angle on error
  }

  // Read the two bytes of angle data
  uint8_t highByte = _I2Cbus->read();
  uint8_t lowByte = _I2Cbus->read();

  // Combine the two bytes into a 16-bit value
  uint16_t angle = ((uint16_t)highByte << 8) | lowByte;

  // Mask to 12 bits if necessary
  angle &= 0x0FFF;

  return angle;
}
