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
#define PWM_S1 10 // Servo 1 signal pin
#define PWM_S2 9 // Servo 2 signal pin
#define PI2_TRIG 7 // PI Zero #2 I2C trigger line
#define PI1_TRIG 8 // PI Zero #1 I2C trigger line
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
  pinMode(RBF, INPUT);
  pinMode(PI2_TRIG, OUTPUT);
  pinMode(PI1_TRIG, OUTPUT);
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
  //delay(1000);
  //launch(0);
  beginCharging();
  endCharging();
  //launch(2);
  //delay(1000);
  //launch(3);
  //delay(1000);
  //launch(4);
  //delay(1000);
}


// --------------------
// Runs the sequence of commands required to launch a tube
// --------------------
void launch (int inServo) {

  //digitalWrite(LIGHTS, HIGH); // Turn on chamber lights
  myServos[inServo].write(LAUNCHER_HOME_POS); // Make sure servo wants to travel to home posistion

  Serial.println(inServo);

  digitalWrite(SERVOS[inServo], HIGH); // Arm launcher servo

  if (/*rbf || digitalRead(RBF == HIGH)*/true) { // Only move if inhibitor is not present at power on or nown
    if (inServo == 2 || inServo == 4)
    {
      for (int i = LAUNCHER_HOME_POS; i < LAUNCHER_FINAL_POS_RIGHT; ++i) { // Move servo from home posistion to launched posistion
        myServos[inServo].write(i);
        delay(LAUNCHER_VELOCITY_DELAY);
      }
    }
    else
    {
      for (int i = LAUNCHER_HOME_POS; i > LAUNCHER_FINAL_POS_LEFT; --i) { // Move servo from home posistion to launched posistion
        //Serial.println(i);

        myServos[inServo].write(i);
        delay(LAUNCHER_VELOCITY_DELAY);
      }
    }
  }

  digitalWrite(SERVOS[inServo], LOW); // Disarm launcher servo
  ++launchNum;

}
void beginCharging() {
  digitalWrite(MTR, HIGH); // Turn on motor
  delay(200);
  digitalWrite(MTR_SRVO, HIGH); // Turn on power to motor servo
  
  myServos[0].write(CHARGER_ACTIVE_POS); // Move servo to charging posistion
}

void endCharging() {
  myServos[0].write(CHARGER_HOME_POS); // Move servo to home posistion
  delay(100); // Wait for servo to move
  digitalWrite(MTR, LOW); // Turn off motor
  delay(200);
  digitalWrite(MTR_SRVO, LOW); // Turn off motor servo

}
