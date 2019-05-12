#include <Wire.h>

#define SAMPLE_RATE 500 //Rate at which probes are read
#define TEENSY_ADR 0x66 // I2C address of Teensy board

typedef union
{
  float number;
  uint8_t bytes[4];
} FLOATUNION_t;



unsigned long lastTempSend = 0;

void setup() {
  Wire.begin();
  Serial.begin(19200); // Rocket telemetry rate
}

void loop() {

  // ---------------------
  //   TEMP PROBE READ/TELEMETRY
  // --------------------
  if (lastTempSend <= millis() + SAMPLE_RATE) {

    float probeData[5];

    for (uint8_t i = 1; i <= 5; ++i) { // Loop through all 5 Temp Probes

      FLOATUNION_t data;

      Wire.beginTransmission(TEENSY_ADR); // request 8 bytes from Teensy
      Wire.write(i); //Tell Tensy which probe temp we want
      Wire.endTransmission(); // Send data
      Wire.requestFrom(TEENSY_ADR, 4); // Request temp data from teensy



      for (int z = 0; z < 4; ++z) {
        data.bytes[z] = Wire.read(); //Read 4 bytes
      }

      probeData[i - 1] = data.number; // Save read float into data array for telemetry
    }

    lastTempSend = millis(); //update last temp send
    sendTempTelem(probeData);
  }

}

// Sends single line of data seperated by tabs with all 5 temps followed by a new line
void sendTempTelem(float *temps) {
  for (int i = 0; i < 5; ++i) { 
    Serial.print(F("T"));
    Serial.print(i - 1);
    Serial.print(temps[i]);
    Serial.print(F("\t"));
  }
  Serial.println();
}
