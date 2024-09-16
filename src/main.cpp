// main.cpp

#include <Arduino.h>
#include <Wire.h>
#include "Motor.h"
#include "Encoder_count.h"

// Define I2C buses for encoders
TwoWire I2Cbus0 = TwoWire(0); // I2C bus for EncoderLeft
TwoWire I2Cbus1 = TwoWire(1); // I2C bus for EncoderRight

// Motor objects
// Define pins for LPWM, RPWM, L_EN, R_EN, and PWM channels
Motor motorLeft(16, 17, 4, 5, 0, 1);    // Left motor
Motor motorRight(18, 19, 27, 23, 2, 3); // Right motor

// Encoder objects
EncoderCount encoderLeft(&I2Cbus0, '-', 0x36);  // Left encoder on I2C bus 0
EncoderCount encoderRight(&I2Cbus1, '+', 0x36); // Right encoder on I2C bus 1

int leftMotorPWM = 0;
int rightMotorPWM = 0;
char leftMotorDirection = 'S'; // 'F' for forward, 'R' for reverse, 'S' for stop
char rightMotorDirection = 'S';

bool safetyStop = true;
unsigned long safetyStopTimeout = 1000; // Timeout in milliseconds
String incomingRosData;
String dataToRos;

unsigned long lastDataTime;

void setup() {
  Serial.begin(115200);

  // Initialize I2C buses for the encoders
  I2Cbus0.begin(21, 22); // SDA, SCL pins for bus 0 (encoderLeft)
  I2Cbus1.begin(25, 26); // SDA, SCL pins for bus 1 (encoderRight)
  I2Cbus0.setClock(400000); // Set I2C clock speed to 400kHz
  I2Cbus1.setClock(400000); // Set I2C clock speed to 400kHz

  Serial.println("Two-Wheel Differential Drive Robot Initialized");
  lastDataTime = millis();
}

void loop() {
  // 1. Read All Encoder Data
  dataToRos = String(encoderRight.getValue()) + "," + String(encoderLeft.getValue());

  // 2. Read Valid Data From ROS Node with Start and End marker
  if (Serial.available() > 0) {
    incomingRosData = Serial.readStringUntil('G');
    if (incomingRosData.startsWith("K")) {
      // 3. Extract Right and Left Motor Command Data from ROS
      leftMotorDirection = incomingRosData.charAt(1);
      leftMotorPWM = incomingRosData.substring(2, 5).toInt();
      rightMotorDirection = incomingRosData.charAt(5);
      rightMotorPWM = incomingRosData.substring(6, 9).toInt();

      // 4. Send Encoder Tick Data to ROS Node
      Serial.println(dataToRos);

      safetyStop = false;
      lastDataTime = millis();
    }
  }

  // 5. Check Safety and Write to Motors
  if (!safetyStop) {
    motorLeft.Drive(leftMotorDirection, leftMotorPWM);
    motorRight.Drive(rightMotorDirection, rightMotorPWM);
  } else {
    motorLeft.Stop();
    motorRight.Stop();
  }

  // Implement safety stop if no data received within the timeout period
  if (millis() - lastDataTime > safetyStopTimeout) {
    safetyStop = true;
  }
}
