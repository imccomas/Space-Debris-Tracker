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
#define LAUNCHER_VELOCITY_DELAY 15 // Delay between angle changes on launchers to achive desired velocity (in ms)
#define CHARGER_HOME_POS 80 // Home posistion for charging assembly servo
#define CHARGER_ACTIVE_POS 110 // Charging posistion for motor servo
//#define CHARGE_POS 90 // Charger servo posistion for active charging
#define CHARGE_TIME 2500 // Time to run charger aparatus (in ms)
/*-----------------------*/

/*-- uC values  --*/
#define SAMPLE_RATE 500 // Rate at which probes are read (in ms)
#define TEENSY_ADR 0x66 // I2C address of Teensy board
#define SLAVE_ADR 0x10 // Slave address used by arduino in slave mode
#define BAUD_RATE 19200 // Rocket telemetry rate
<<<<<<< HEAD
#define PROCESSING_TIME 15000 // Time delay before asking PIs for data after launch (in ms)
=======
#define PROCESSING_TIME 5000 // Time delay before asking PIs for data after launch (in ms)
>>>>>>> aec722351dfbb2279cec0ff48ec4b28d581f259e
#define PI_BOOT_TIME 60000 // Time for the R PIs to boot and stop doing weird things on the GPIO PINS
/*-----------------------*/

/*-- Pin definitions --*/
#define START 2 // TE-1 signal line
#define PWM_S0 3 // Servo 0 signal pin (motor servo)
#define RBF 4 // Remove before flight pin
#define PWM_S1 10 // Servo 1 signal pin
#define PWM_S2 9 // Servo 2 signal pin
<<<<<<< HEAD
#define PI2_TRIG 7 // PI Zero #2 I2C trigger line
#define PI1_TRIG 8 // PI Zero #1 I2C trigger line
=======
#define PI2_TRIG 8 // PI Zero #2 I2C trigger line
#define PI1_TRIG 7 // PI Zero #1 I2C trigger line
>>>>>>> aec722351dfbb2279cec0ff48ec4b28d581f259e
#define PWM_S3 6 // Servo 3 signal line
#define PWM_S4 5 // Servo 4 signal line
#define LIGHTS 11 //  Light relay line
#define SERVO4 12 // Servo 4 powerline control pin
#define SERVO3 13 // Servo 3 powerline control pin
#define SERVO2 A3 // Servo 2 powerline control pin
#define SERVO1 A2 // Servo 1 powerline control pin
#define MTR_SRVO A1 // Motor position servo powerline control pin
#define MTR A0 // Motor power control pin
/*-----------------------*/

<<<<<<< HEAD
=======
/*--Pi telemetry globals--*/
int piNum;
bool waitState;
/*------------------------*/
>>>>>>> aec722351dfbb2279cec0ff48ec4b28d581f259e
typedef union
{
  int32_t number;
  uint8_t bytes[4];
} FLOATUNION_t;

const int SERVOS[5] = { MTR_SRVO, SERVO1, SERVO2, SERVO3, SERVO4 };
const int myServos[5] = { PWM_S0, PWM_S1, PWM_S2, PWM_S3, PWM_S4 };
Servo myServo;
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
  //pinMode(SERVO4, OUTPUT);
  //pinMode(SERVO3, OUTPUT);
  //pinMode(SERVO2, OUTPUT);
  //pinMode(SERVO1, OUTPUT);
  //pinMode(MTR_SRVO, OUTPUT);
  pinMode(MTR, OUTPUT);
  // ------------------

  // Set default states
  digitalWrite(PI2_TRIG, LOW);
  digitalWrite(PI1_TRIG, LOW);
  digitalWrite(LIGHTS, LOW);
  digitalWrite(MTR, LOW);
  // ---------------

  /* Removed for testing
    // Attach Servos
    myServos[0].attach(PWM_S0); // Motor Servo
    myServos[1].attach(PWM_S1); // Tube 1
    myServos[2].attach(PWM_S2); // Tube 2
    myServos[3].attach(PWM_S3); // Tube 3
    myServos[4].attach(PWM_S4); // Tube 4
<<<<<<< HEAD

=======
>>>>>>> aec722351dfbb2279cec0ff48ec4b28d581f259e
    // Set all servos to home posistion
    myServos[0].write(CHARGER_HOME_POS);
    myServos[1].write(LAUNCHER_HOME_POS);
    myServos[2].write(LAUNCHER_HOME_POS);
    myServos[3].write(LAUNCHER_HOME_POS);
    myServos[4].write(LAUNCHER_HOME_POS);
    // ---------------
  */

  if (digitalRead(RBF) == HIGH) { // Remove Beofre Flight pin is present at power on
    rbf = false;
    Serial.println(F("-- RBF Inhibitor PRESENT at POWERON --"));
  }

  lastTempSend = 0;
}

void loop() {

  // ---------------------
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
  else if (nextEvent != 0 && nextEvent <= millis()) { // Time has elapsed for next event

    switch (sequence) { // Sequence of events that occur.

      case 1: // Launch Control tube #1
        delay(20);
        launch(1);
        nextEvent = millis() + PROCESSING_TIME; // Time (in ms) until next experiment event begins
        ++sequence;
        break;

      case 2: // Read data from PIs
        digitalWrite(LIGHTS, LOW); //Turn off chamber lighting
<<<<<<< HEAD
        //getPiData();
=======
        getPiData();
>>>>>>> aec722351dfbb2279cec0ff48ec4b28d581f259e
        nextEvent = millis() + 250;
        ++sequence;
        break;

      case 3: // Begin charging plate for first lauch
        beginCharging();
        nextEvent = millis() + CHARGE_TIME;
        ++sequence;
        break;

      case 4: // End Charging
        endCharging();
        nextEvent = millis() + 500;
        ++sequence;
        break;

      case 5: // Launch tube #2
        launch(2);
        nextEvent = millis() + PROCESSING_TIME;
        ++sequence;
        break;

      case 6: // Read Pi data
        digitalWrite(LIGHTS, LOW);
<<<<<<< HEAD
        //getPiData();
=======
        getPiData();
>>>>>>> aec722351dfbb2279cec0ff48ec4b28d581f259e
        nextEvent = millis() + 500;
        ++sequence;
        break;

      case 7: // Recharge plate
        beginCharging();
        nextEvent = millis() + CHARGE_TIME;
        ++sequence;
        break;

      case 8: // End Charging
        endCharging();
        nextEvent = millis() + 500;
        ++sequence;
        break;

      case 9: // Launch tube #3
        launch(3);
        nextEvent = millis() + PROCESSING_TIME;
        ++sequence;
        break;

      case 10: // Read Pi data
        digitalWrite(LIGHTS, LOW);
<<<<<<< HEAD
        //getPiData();
=======
        getPiData();
>>>>>>> aec722351dfbb2279cec0ff48ec4b28d581f259e
        nextEvent = millis() + 500;
        ++sequence;
        break;

      case 11: // Recharge plate
        beginCharging();
        nextEvent = millis() + CHARGE_TIME;
        ++sequence;
        break;

      case 12: // End Charging
        endCharging();
        nextEvent = millis() + 500;
        ++sequence;
        break;

      case 13: // Launch tube #4
        launch(4);
        nextEvent = millis() + PROCESSING_TIME;
        ++sequence;
        break;

      case 14: // Read Pi data
        digitalWrite(LIGHTS, LOW);
<<<<<<< HEAD
        //getPiData();
=======
        getPiData();
>>>>>>> aec722351dfbb2279cec0ff48ec4b28d581f259e
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
  else if (!started && digitalRead(START) == HIGH) { // Experiment start condition met

    started = true;

    //Serial.println(F("*************************"));
    Serial.print(F("START Command received at "));
    Serial.print(millis());
    Serial.println(F("ms."));
    //Serial.println(F("*************************"));
    sequence = 1; // Set flag to start value
    nextEvent = millis() + 2500; // Begin next event in 1 second
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
void launch (uint8_t servo) {

  // Signal PIs that we are beginning the experiment,
  // and they should begin recording for debris launch
<<<<<<< HEAD
=======
  digitalWrite(LIGHTS, HIGH); // Turn on chamber lights
  delay(25);
>>>>>>> aec722351dfbb2279cec0ff48ec4b28d581f259e
  digitalWrite(PI1_TRIG, HIGH);
  delay(10);
  digitalWrite(PI1_TRIG, LOW);
  delay(25);
  digitalWrite(PI2_TRIG, HIGH);
  delay(10);
  digitalWrite(PI2_TRIG, LOW);
  delay(25);

  Serial.print(F("Launching tube #"));
  Serial.println(servo);
<<<<<<< HEAD
  
  digitalWrite(LIGHTS, HIGH); // Turn on chamber lights
  delay(250);
=======


  delay(250);

>>>>>>> aec722351dfbb2279cec0ff48ec4b28d581f259e
  myServo.attach(myServos[servo]);
  myServo.write(LAUNCHER_HOME_POS); // Make sure servo wants to travel to home posistion
  //digitalWrite(SERVOS[servo], HIGH); // Arm launcher servo

  if (rbf || digitalRead(RBF == HIGH)) { // Only move if inhibitor is not present at power on or now

    if (servo == 2 || servo == 4)
    {
      for (int i = LAUNCHER_HOME_POS; i < LAUNCHER_FINAL_POS_RIGHT; ++i) { // Move servo from home posistion to launched posistion
        myServo.write(i);
        delay(LAUNCHER_VELOCITY_DELAY);
      }
    }
    else
    {
      for (int i = LAUNCHER_HOME_POS; i > LAUNCHER_FINAL_POS_LEFT; --i) { // Move servo from home posistion to launched posistion
        //Serial.println(i);

        myServo.write(i);
        delay(LAUNCHER_VELOCITY_DELAY);
      }
    }
  }

  //digitalWrite(SERVOS[servo], LOW); // Disarm launcher servo
  myServo.detach(); // Detach Servo output
<<<<<<< HEAD
  
=======

>>>>>>> aec722351dfbb2279cec0ff48ec4b28d581f259e
  ++launchNum;

}

void beginCharging() {
  Serial.println(F("Begining Charge."));
  myServo.attach(myServos[0]); // Attach Motor Servo
  //digitalWrite(MTR_SRVO, HIGH); // Turn on power to motor servo
  delay(250);
  myServo.write(CHARGER_ACTIVE_POS); // Move servo to charging posistion
  delay(250);
  digitalWrite(MTR, HIGH); // Turn on motor
  myServo.detach(); // Detach Motor Servo

}

void endCharging() {
  Serial.println(F("Ending Charge"));
  myServo.attach(myServos[0]); // Attach Motor Servo
  digitalWrite(MTR, LOW); // Turn off motor
  delay(250);

  myServo.write(CHARGER_HOME_POS); // Move servo to home posistion
  delay(250);// Wait for servo to move
  //digitalWrite(MTR_SRVO, LOW); // Turn off motor servo
  myServo.detach(); // Detach Motor Servo
}

// --------------------
// Pulls the data from both Raspberry Pi Zeros by switching to I2C slave mode, and signaling
// the Pi that the Arduino is ready to receive data
// --------------------
void getPiData() {
<<<<<<< HEAD
=======
  waitState = false;
  piNum = 1;
>>>>>>> aec722351dfbb2279cec0ff48ec4b28d581f259e

  Serial.println(F("Ending TWI"));
  Wire.end(); // End current I2C Bus
  Wire.begin(SLAVE_ADR); // Open I2C bus as a slave
<<<<<<< HEAD
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
=======
  Wire.onReceive(receiveEvent);
  Serial.println(F("In slave mode"));

  digitalWrite(PI1_TRIG, HIGH); // Tell Pi #1 we are ready for data
  Serial.println(F("PI1_TRIG Fired"));
  delay(20);
  digitalWrite(PI1_TRIG, LOW); // Bring trigger line back low
  Serial.println(F("PI1_TRIG off"));
  while (!waitState)
  {
    delay(1);
  }
  waitState = false;
  digitalWrite(PI2_TRIG, HIGH); // Tell Pi #1 we are ready for data
  Serial.println(F("PI2_TRIG Fired"));
  delay(20);
  digitalWrite(PI2_TRIG, LOW); // Bring trigger line back low
  Serial.println(F("PI2_TRIG off"));
  while (!waitState)
  {
    delay(1);
  }


  Wire.end(); // End slave mode
  delay(2);
  Wire.begin(); // Return to master mode
}

void receiveEvent(int dataSet) {
  //Serial.print(F("******** BEGIN LAUNCH #"));
  //Serial.print(launchNum);
  //Serial.print(F(" for pi #"))
  //Serial.print(piNum);
  //Serial.println(F(" DATA ********"));

  while (Wire.available()) { // Read data in buffer
    Serial.println(Wire.read()); // Move buffer data directly to telem line
  }
  Serial.println();
  Serial.print(F("******** END PI #"));
  Serial.print(piNum);
  Serial.println(F(" DATA ********"));         // print the integer
  waitState = true;
  piNum += 1;

>>>>>>> aec722351dfbb2279cec0ff48ec4b28d581f259e
}
