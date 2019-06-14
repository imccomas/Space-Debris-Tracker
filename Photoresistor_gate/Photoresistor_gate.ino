

const int pResistor1 = A0; // Photoresistor1 at Arduino analog pin A0
const int pResistor2 = A1; // Photoresistor2 at analog pin A1
int value1;          // Store value from photoresistor (0-1023)
int value2;
int sensitivity;
double velocity;
unsigned long timeNow, passTime;
bool gate1;

void setup() {
  Serial.begin(9600);
  pinMode(pResistor1, INPUT);// Set pResistor - A0 pin as an input (optional)
  pinMode(pResistor2, INPUT);
  gate1 = false; // reset flag
  sensitivity = 500;
}

void loop() {
  value1 = analogRead(pResistor1);
  value2 = analogRead(pResistor2);
  //Serial.println(value1);
  
  if (value1 < sensitivity)
  {
    timeNow = millis(); //save current time
    gate1 = true; // set flag for passing first gate
    
  }

  if (value2 < sensitivity && gate1) // check we registered on the first gate and started the timer
  {
    passTime = millis();
    velocity = (0.5/((passTime-timeNow)/1000));
    Serial.println("Total time: " + String((passTime-timeNow)) + "ms");
    Serial.println("velocity: ");
    Serial.print(velocity);
    gate1 = false;
  }
}
