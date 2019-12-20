<<<<<<< HEAD
#include <Servo.h>

Servo motservo;
Servo lservo_one;
Servo lservo_two;
Servo lservo_three;
Servo lservo_four;


int pos = 0;    // variable to store the servo position
int mot = 3;   // motor variable
void setup() {
  
  motservo.attach(10); // attaches the servo on pin 9 to the servo object
  lservo_one.attach(9);
  lservo_two.attach(6);
  lservo_three.attach(5);
  lservo_four.attach(3);
  
  pinMode(mot, OUTPUT);  // makes the motor an output for the controller 
}

void loop() {

  chargeplate();
  launcher(1);
  chargeplate();
  launcher(2);
  chargeplate();
  launcher(3);
  chargeplate();
  launcher(4);
}

int chargeplate(){
for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    motservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 5ms for the servo to reach the position
  }
  delay(1000); // Give motor a second to make contact with the plate
  digitalWrite(mot, HIGH);   // turn the motor on
  delay(3000); //keep motor running for 3 seconds

  digitalWrite(mot, LOW);    // turn the motor off

  
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    motservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 5ms for the servo to reach the position
  }
delay(10000);

}

int launcher(int x){
  
    for (pos = 0; pos <= 45; pos += 1) { // goes from 0 degrees to 45 degrees
      if (x == 1){
        lservo_one.write(pos);              // tell servo to go to position in variable 'pos'
        delay(25);     // Launch speed
      }
      if (x == 2){
        lservo_two.write(pos);              // tell servo to go to position in variable 'pos'
        delay(25);     // Launch speed
      }
      if (x == 3){
        lservo_three.write(pos);              // tell servo to go to position in variable 'pos'
        delay(25);     // Launch speed
      }
      if (x == 4){
        lservo_four.write(pos);              // tell servo to go to position in variable 'pos'
        delay(25);     // Launch speed
      }
  
  
}
}
=======
#include <Servo.h>

Servo motservo;
Servo lservo_one;
Servo lservo_two;
Servo lservo_three;
Servo lservo_four;


int pos = 0;    // variable to store the servo position
int mot = 3;   // motor variable
void setup() {
  
  motservo.attach(10); // attaches the servo on pin 9 to the servo object
  lservo_one.attach(9);
  lservo_two.attach(6);
  lservo_three.attach(5);
  lservo_four.attach(3);
  
  pinMode(mot, OUTPUT);  // makes the motor an output for the controller 
}

void loop() {

  chargeplate();
  launcher(1);
  chargeplate();
  launcher(2);
  chargeplate();
  launcher(3);
  chargeplate();
  launcher(4);
}

int chargeplate(){
for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    motservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 5ms for the servo to reach the position
  }
  delay(1000); // Give motor a second to make contact with the plate
  digitalWrite(mot, HIGH);   // turn the motor on
  delay(3000); //keep motor running for 3 seconds

  digitalWrite(mot, LOW);    // turn the motor off

  
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    motservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5);                       // waits 5ms for the servo to reach the position
  }
delay(10000);

}

int launcher(int x){
  
    for (pos = 0; pos <= 45; pos += 1) { // goes from 0 degrees to 45 degrees
      if (x == 1){
        lservo_one.write(pos);              // tell servo to go to position in variable 'pos'
        delay(25);     // Launch speed
      }
      if (x == 2){
        lservo_two.write(pos);              // tell servo to go to position in variable 'pos'
        delay(25);     // Launch speed
      }
      if (x == 3){
        lservo_three.write(pos);              // tell servo to go to position in variable 'pos'
        delay(25);     // Launch speed
      }
      if (x == 4){
        lservo_four.write(pos);              // tell servo to go to position in variable 'pos'
        delay(25);     // Launch speed
      }
  
  
}
}
>>>>>>> aec722351dfbb2279cec0ff48ec4b28d581f259e
