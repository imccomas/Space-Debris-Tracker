// Rocksat-x 2019 Primary Motherboard software
// ---
// Written by: Ryan Wade
// ---
// This program controls CCofCO's rocksat-x payload, as well as telemeters data to the rocket

#include <Wire.h>
#include <Servo.h>

/*-- Actuator values --*/
#define LAUNCHER_HOME_POS 90 // Home posistion for servos (retaining ball)
#define LAUNCHER_FINAL_POS_RIGHT 145 // Final posistion for launcher servos on right
#define LAUNCHER_FINAL_POS_LEFT 35 // Final posistion for launcher servos on left
#define LAUNCHER_VELOCITY_DELAY 25 // Delay between angle changes on launchers to achive desired velocity (in ms)
#define CHARGER_HOME_POS 60 // Home posistion for charging assembly servo
#define CHARGER_ACTIVE_POS 145 // Charging posistion for motor servo
#define CHARGE_POS 90 // Charger servo posistion for active charging
#define CHARGE_TIME 5000 // Time to run charger aparatus (in ms)
/*-----------------------*/

/*-- uC values  --*/
#define SAMPLE_RATE 500 // Rate at which probes are read (in ms)
#define TEENSY_ADR 0x66 // I2C address of Teensy board
#define SLAVE_ADR 0x10 // Slave address used by arduino in slave mode
#define BAUD_RATE 19200 // Rocket telemetry rate
#define PROCESSING_TIME 15000 // Time delay before asking PIs for data after launch (in ms)
#define PI_BOOT_TIME 5000 // Time for the R PIs to boot and stop doing weird things on the GPIO PINS
/*-----------------------*/

/*-- Pin definitions --*/
#define START 2 // TE-1 signal line
#define PWM_S0 3 // Servo 0 signal pin (motor servo)
#define RBF 4 // Remove before flight pin
#define PWM_S1 5 // Servo 1 signal pin
#define PWM_S2 6 // Servo 2 signal pin
#define PI2_TRIG 7 // PI Zero #2 I2C trigger line
#define PI1_TRIG 8 // PI Zero #1 I2C trigger line
#define PWM_S3 9 // Servo 3 signal line
#define PWM_S4 10 // Servo 4 signal line
#define LIGHTS 11 //  Light relay line
#define SERVO4 12 // Servo 4 powerline control pin
#define SERVO3 13 // Servo 3 powerline control pin
#define SERVO2 A0 // Servo 2 powerline control pin
#define SERVO1 A1 // Servo 1 powerline control pin
#define MTR_SRVO A2 // Motor position servo powerline control pin
#define MTR A3 // Motor power control pin
/*-----------------------*/

typedef union
{
  int32_t number;
  uint8_t bytes[4];
} FLOATUNION_t;

const int SERVOS[5] = { MTR_SRVO, SERVO1, SERVO2, SERVO3, SERVO4 };
Servo myServos[5];
uint8_t sequence = 0, launchNum = 0;
unsigned long lastTempSend, timeCompleted = 0, nextEvent = 0;
bool started = false, rbf = true;

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
  pinMode(PWM_S0, OUTPUT);
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

  // Attach Servos
  myServos[0].attach(PWM_S0); // Motor Servo
  myServos[1].attach(PWM_S1); // Tube 1
  myServos[2].attach(PWM_S2); // Tube 2
  myServos[3].attach(PWM_S3); // Tube 3
  myServos[4].attach(PWM_S4); // Tube 4
  // Set all servos to home posistion
  myServos[0].write(CHARGER_HOME_POS);
  myServos[1].write(LAUNCHER_HOME_POS);
  myServos[2].write(LAUNCHER_HOME_POS);
  myServos[3].write(LAUNCHER_HOME_POS);
  myServos[4].write(LAUNCHER_HOME_POS);
  // ---------------

  if (digitalRead(RBF) == HIGH) { // Remove Beofre Flight pin is present at power on
    rbf = false;
    Serial.println(F("-- RBF Inhibitor PRESENT at POWERON --"));
  }

  lastTempSend = 0;
}

void loop() {

  /*/ ---------------------
  //   TEMP PROBE READ/TELEMETRY OUT
  // --------------------
  if (lastTempSend + SAMPLE_RATE <= millis() ) {

    float probeData[5];

    for (uint8_t i = 1; i <= 5; ++i) { // Loop through all 5 Temp Probes

      FLOATUNION_t data;

      Wire.beginTransmission(TEENSY_ADR); // Request from Teensy
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

  // ----------------
  // Check if event timer has ended
  // ----------------
  else*/ if (nextEvent != 0 && nextEvent <= millis()) { // Time has elapsed for next event

    switch (sequence) { // Sequence of events that occur.

      case 1: // Launch Control tube #1
        launch(1);
        nextEvent = millis() + PROCESSING_TIME; // Time (in ms) until next experiment event begins
        ++sequence;
        break;

      case 2: // Read data from PIs
        digitalWrite(LIGHTS, LOW); //Turn off chamber lighting
        //getPiData();
        nextEvent = millis() + 100;
        ++sequence;
        break;

      case 3: // Begin charging plate for first lauch
        beginCharging();
        nextEvent = millis() + CHARGE_TIME;
        ++sequence;
        break;

      case 4: // End Charging
        endCharging();
        nextEvent = millis() + 100;
        ++sequence;
        break;

      case 5: // Launch tube #2
        launch(2);
        nextEvent = millis() + PROCESSING_TIME;
        ++sequence;
        break;

      case 6: // Read Pi data
        digitalWrite(LIGHTS, LOW);
        //getPiData();
        nextEvent = millis() + 100;
        ++sequence;
        break;

      case 7: // Recharge plate
        beginCharging();
        nextEvent = millis() + CHARGE_TIME;
        ++sequence;
        break;

      case 8: // End Charging
        endCharging();
        nextEvent = millis() + 100;
        ++sequence;
        break;

      case 9: // Launch tube #3
        launch(3);
        nextEvent = millis() + PROCESSING_TIME;
        ++sequence;
        break;

      case 10: // Read Pi data
        digitalWrite(LIGHTS, LOW);
        //getPiData();
        nextEvent = millis() + 100;
        ++sequence;
        break;

      case 11: // Recharge plate
        beginCharging();
        nextEvent = millis() + CHARGE_TIME;
        ++sequence;
        break;

      case 12: // End Charging
        endCharging();
        nextEvent = millis() + 100;
        ++sequence;
        break;

      case 13: // Launch tube #4
        launch(4);
        nextEvent = millis() + PROCESSING_TIME;
        ++sequence;
        break;

      case 14: // Read Pi data
        digitalWrite(LIGHTS, LOW);
        //getPiData();
        nextEvent = 0; // End of sequence
        break;

      default:
        // Something very bad has happened if execution hits here
        Serial.println(F("My world has been shattered."));
        break;
    }
  }

  // ------------------
  // Experiment START code
  // ------------------
  else if (!started && millis() > PI_BOOT_TIME/* && digitalRead(START) == HIGH*/) { // Experiment start condition met

    started = true;

    Serial.println(F("*************************"));
    Serial.print(F("START Command received at "));
    Serial.print(millis());
    Serial.println(F("ms."));
    Serial.println(F("*************************"));
    sequence = 1; // Set flag to start value

    digitalWrite(LIGHTS, HIGH);
    delay(1000);
    digitalWrite(LIGHTS, LOW);
    delay(1000);
    digitalWrite(LIGHTS, HIGH);
    delay(1000);
    digitalWrite(LIGHTS, LOW);
    
    nextEvent = millis() + 1000; // Begin next event in 1 second
  }
}

// --------------------------------------------------------------------
// Sends single formatted stream of data seperated by tabs with a timestamp then all 5 temps followed by a new line
// --------------------------------------------------------------------
void sendTempTelem(float * temps) {

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

// --------------------
// Runs the sequence of commands required to launch a tube
// --------------------
void launch (int servo) {

  // Signal PIs that we are beginning the experiment,
  // and they should begin recording for debris launch
  digitalWrite(PI1_TRIG, HIGH);
  digitalWrite(PI2_TRIG, HIGH);
  delay(10);
  digitalWrite(PI1_TRIG, LOW);
  digitalWrite(PI2_TRIG, LOW);

  digitalWrite(LIGHTS, HIGH); // Turn on chamber lights
  myServos[servo].write(LAUNCHER_HOME_POS); // Make sure servo wants to travel to home posistion
  Serial.println(servo);
  digitalWrite(SERVOS[servo], HIGH); // Arm launcher servo

  if (/*rbf || digitalRead(RBF == HIGH)*/true) { // Only move if inhibitor is not present at power on or now
    Serial.println("if");
    if (launchNum == 0 || launchNum == 2)
    {
      for (int i = LAUNCHER_HOME_POS; i < LAUNCHER_FINAL_POS_RIGHT; ++i) { // Move servo from home posistion to launched posistion
        Serial.println(i);
  
        myServos[servo].write(i);
        delay(LAUNCHER_VELOCITY_DELAY);
      }
    }
    else
    {
      for (int i = LAUNCHER_HOME_POS; i > LAUNCHER_FINAL_POS_LEFT; --i) { // Move servo from home posistion to launched posistion
        Serial.println(i);
  
        myServos[servo].write(i);
        delay(LAUNCHER_VELOCITY_DELAY);
      }
    }
  }
  Serial.println(launchNum);

  digitalWrite(SERVOS[servo], LOW); // Disarm launcher servo
  ++launchNum;
  Serial.println(launchNum);

}

void beginCharging() {
  digitalWrite(MTR, HIGH); // Turn on motor
  digitalWrite(MTR_SRVO, HIGH); // Turn on power to motor servo
  myServos[0].write(CHARGER_ACTIVE_POS); // Move servo to charging posistion
}

void endCharging() {
  myServos[0].write(CHARGER_HOME_POS); // Move servo to home posistion
  delay(100); // Wait for servo to move
  digitalWrite(MTR, LOW); // Turn off motor
  digitalWrite(MTR_SRVO, LOW); // Turn off motor servo

}

// --------------------
// Pulls the data from both Raspberry Pi Zeros by switching to I2C slave mode, and signaling
// the Pi that the Arduino is ready to receive data
// --------------------
void getPiData() {

  Serial.println(F("Ending TWI"));
  Wire.end(); // End current I2C Bus
  Wire.begin(SLAVE_ADR); // Open I2C bus as a slave
Serial.println(F("In slave mode"));
  digitalWrite(PI1_TRIG, HIGH); // Tell Pi #1 we are ready for data
  delay(20);
  digitalWrite(PI1_TRIG, LOW); // Bring trigger line back low

  while (!Wire.available()) { // Wait for data in buffer
    delay(1);
  }
  Serial.print(F("******** BEGIN LAUNCH #"));
  Serial.print(launchNum);
  Serial.println(F(" PI #1 DATA ********"));

  while (Wire.available()) { // Read data in buffer
    Serial.print(Wire.read()); // Move buffer data directly to telem line
  }
  Serial.println();
  Serial.println(F("******** END PI #1 DATA ********"));

  digitalWrite(PI2_TRIG, HIGH); // Tell Pi #2 we are ready for data
  delay(20);
  digitalWrite(PI2_TRIG, LOW); // Bring trigger line back low

  while (!Wire.available()) { // Wait for data in buffer
    delay(1);
  }
  Serial.print(F("******** BEGIN LAUNCH #"));
  Serial.print(launchNum);
  Serial.println(F(" PI #2 DATA ********"));

  while (Wire.available()) { // Read data in buffer
    //TODO: Format Telemetry OUTPUT once Pi data transmission structure is known
    Serial.print(Wire.read()); // Move buffer data directly to telem line
  }
  Serial.println();
  Serial.println(F("******** END PI #2 DATA ********"));

  Wire.end(); // End slave mode
  Wire.begin(); // Return to master mode
}
