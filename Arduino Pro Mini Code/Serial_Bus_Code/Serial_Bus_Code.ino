#include <Wire.h>

#define SAMPLE_RATE 500 // Rate at which probes are read (in ms)
#define TEENSY_ADR 0x66 // I2C address of Teensy board
#define BAUD_RATE 19200 // Rocket telemetry rate

//-- Pin definitions --
#define START 2 // TE-1 signal line
#define PWM_S1 3 // Servo 1 signal pin
#define RBF 4 // Remove before flight pin
#define PWM_S2 5 // Servo 2 signal pin
#define PWM_S3 6 // Servo 3 signal pin
#define PI2_TRIG 7 // PI Zero #2 I2C trigger line
#define PI1_TRIG 8 // PI Zero #1 I2C trigger line
#define PWM_S4 9 // Servo 4 signal line
#define PWM_S5 10 // Servo 5 signal line
#define LIGHTS 11 //  Light relay line
#define SERVO4 12 // Servo 4 powerline control pin
#define SERVO3 13 // Servo 3 powerline control pin
#define SERVO2 14 // Servo 2 powerline control pin
#define SERVO1 15 // Servo 1 powerline control pin
#define MTR_SRVO 16 // Motor position servo powerline control pin
#define MTR 17 // Motor power control pin
//-----------------------

typedef union
{
  float number;
  uint8_t bytes[4];
} FLOATUNION_t;

unsigned long lastTempSend;

void setup() {
  Wire.begin();
  Serial.begin(BAUD_RATE);

  Serial.println(F("Connected."));

  // Pin declerations
  pinMode(START, INPUT);
  pinMode(PWM_S1, OUTPUT);
  pinMode(RBF, INPUT);
  pinMode(PWM_S2, OUTPUT);
  pinMode(PWM_S3, OUTPUT);
  pinMode(PI2_TRIG, OUTPUT);
  pinMode(PI1_TRIG, OUTPUT);
  pinMode(PWM_S4, OUTPUT);
  pinMode(PWM_S5, OUTPUT);
  pinMode(LIGHTS, OUTPUT);
  pinMode(SERVO4, OUTPUT);
  pinMode(SERVO3, OUTPUT);
  pinMode(SERVO2, OUTPUT);
  pinMode(SERVO1, OUTPUT);
  pinMode(MTR_SRVO, OUTPUT);
  pinMode(MTR, OUTPUT);
  // ------------------

  // Set default states
  digitalWrite(PI2_TRIG, LOW);
  digitalWrite(PI1_TRIG, LOW);
  digitalWrite(LIGHTS, LOW);
  digitalWrite(SERVO4, LOW);
  digitalWrite(SERVO3, LOW);
  digitalWrite(SERVO2, LOW);
  digitalWrite(SERVO1, LOW);
  digitalWrite(MTR_SRVO, LOW);
  digitalWrite(MTR, LOW);
  // ---------------

  lastTempSend = 0;
}

void loop() {

// ------------------
// Experiment START code
// ------------------
  if (digitalRead(START) == HIGH) { // Experiment start condition met

    Serial.print(F("START Command received at "));
    Serial.print(millis());
    Serial.println(F("ms."));

    // Do your thing chicken wing
    
  }
  // ---------------------
  //   TEMP PROBE READ/TELEMETRY OUT
  // --------------------
  if (lastTempSend <= millis() + SAMPLE_RATE) {

    float probeData[5];

    for (uint8_t i = 1; i <= 5; ++i) { // Loop through all 5 Temp Probes

      FLOATUNION_t data;

      Wire.beginTransmission(TEENSY_ADR); // Request 8 bytes from Teensy
      Wire.write(i); // Tell Tensy which probe temp we want
      Wire.endTransmission(); // Send data
      Wire.requestFrom(TEENSY_ADR, 4); // Request temp data from teensy

      for (int z = 0; z < 4; ++z) {
        data.bytes[z] = Wire.read(); //Read 4 bytes
      }
      probeData[i - 1] = data.number; // Save float into data array for telemetry
    }
    lastTempSend = millis(); // Update last temp send
    sendTempTelem(probeData);
  }
}
// --------------------------------------------------------------------
// Sends single formatted stream of data seperated by tabs with a timestamp then all 5 temps followed by a new line
// --------------------------------------------------------------------
void sendTempTelem(float *temps) {

  Serial.print(millis());

  for (int i = 0; i < 5; ++i) {
    Serial.print(F("\t"));
    Serial.print(F("T"));
    Serial.print(i + 1);
    Serial.print(F(": "));
    Serial.print(temps[i]);
  }
  Serial.println();
}
