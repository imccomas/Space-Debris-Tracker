#include <i2c_t3.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_MAX31865.h>
#include <pt100rtd.h>

#define RBF_RUNTIME 60000
#define RREF 400.0
#define C2F(c) ((9 * c / 5) + 32)
#define RATE 250
#define TRIG 7
#define U1 8
#define U2 9
#define RBF 10

typedef union
{
  float number;
  uint8_t bytes[4];
} FLOATUNION_t;

FLOATUNION_t temps[5];
char filename[16];
uint8_t probeSelect;
unsigned long timeStamp, rbfTime;
bool triggered, rbf;

Adafruit_MAX31865 max_1 = Adafruit_MAX31865(2, 11, 12, 13);
Adafruit_MAX31865 max_2 = Adafruit_MAX31865(3, 11, 12, 13);
Adafruit_MAX31865 max_3 = Adafruit_MAX31865(4, 11, 12, 13);
Adafruit_MAX31865 max_4 = Adafruit_MAX31865(5, 11, 12, 13);
Adafruit_MAX31865 max_5 = Adafruit_MAX31865(6, 11, 12, 13);

File myFile;
pt100rtd PT100 = pt100rtd() ;
//const uint8_t chipSelect = BUILTIN_SDCARD;




void setup() {

  Serial.begin(9600);
  Wire.begin(I2C_SLAVE, 0x66, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  pinMode(U1, OUTPUT);
  digitalWrite(U1, LOW);
  pinMode(U2, OUTPUT);
  digitalWrite(U2, LOW);
  pinMode(TRIG, INPUT);
  triggered = false;
  rbf = false;
  pinMode(RBF, INPUT);

  max_1.begin(MAX31865_2WIRE);
  max_2.begin(MAX31865_2WIRE);
  max_3.begin(MAX31865_2WIRE);
  max_4.begin(MAX31865_2WIRE);
  max_5.begin(MAX31865_2WIRE);

  Serial.print("Initializing SD card...");

  if (!SD.begin()) {
    Serial.println("initialization failed!");
    return;
  }

  /******Writing Column Headers******/
  int n = 0;
  snprintf(filename, sizeof(filename), "temp%03d.txt", n); // includes a three-digit sequence number in the file name
  while (SD.exists(filename)) {
    n++;
    snprintf(filename, sizeof(filename), "temp%03d.txt", n);
  }
  myFile = SD.open(filename, FILE_WRITE);
  myFile.print("T1(C)"); myFile.print("\t"); myFile.print("T2(C)"); myFile.print("\t"); myFile.print("T3(C)"); myFile.print("\t"); myFile.print("T4(C)"); myFile.print("\t"); myFile.print("T5(C)"); myFile.print("\t"); myFile.println("Time(ms)");
  myFile.close();
  /***********************************/

  Serial.print(F("Initialization done. File Name: "));
  Serial.println(filename);

}

void loop() {

  // ****************
  // Power Controls
  // ****************
  if (!triggered && digitalRead(TRIG) == HIGH) { // Power switch condition met

    triggered = true; // update flag

    if (digitalRead(RBF) == LOW)  { // Nominal condition

      digitalWrite(U1, HIGH);
      digitalWrite(U2, HIGH);
      Serial.println(F("Power Switchover Complete"));
    }
    else { // Flight Inhibit condition

      rbf = true;
      rbfTime = millis() + RBF_RUNTIME; // Set shutdown time
      digitalWrite(U1, HIGH);
      digitalWrite(U2, HIGH);

      Serial.print(F("RBF Power Switchover Complete. Expiremnt will run for "));
      Serial.print(RBF_RUNTIME / 1000);
      Serial.println(F(" seconds."));
    }
  }
  
  if (rbf && millis() >= rbfTime) { // Shutdown time elapsed

    digitalWrite(U2, LOW); // Turn off battery relay
  }
  // ********************************************************

  // *************************
  // Probe Reads + File Write
  // *************************
  if (timeStamp <= millis() + RATE) {

    readProbes();

    myFile = SD.open(filename, FILE_WRITE); // Open file for writing
    timeStamp = millis();

    for (int i = 0; i < 5; ++i) { //Write out 5 temps spaced with a tab
      myFile.print(temps[i].number);
      myFile.print('\t');
    }
    myFile.println(timeStamp);
    myFile.close();
    Serial.println("Line written to file.\n");
  }
}

// ----------------------------
// Sends temp stored in temps[]
// ----------------------------
void requestEvent() {
  for (int i = 0; i < 4; ++i) {
    Wire.write(temps[probeSelect - 1].bytes[i]); // write 4 bytes for float value
  }
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------
// This function is called whenever the master sends this device data. In this case, the master is telling this device which RTD probe temp it wants to read.
// ----------------------------------------------------------------------------------------------------------------------------------------------------------
void receiveEvent(unsigned int numBytes) {
  while (Wire.available()) { //Pull all bytes from buffer
    probeSelect = Wire.read(); //receive byte
  }
}
// ------------------------------------------------------
// Pulls temp from all 5 probes and stores to temps[]
// ------------------------------------------------------
void readProbes () {

  uint16_t rtd, ohmsx100 ;
  uint32_t dummy ;

  /*********PROBE1*********/
  rtd = max_1.readRTD();
  dummy = ((uint32_t)(rtd << 1)) * 100 * ((uint32_t) floor(RREF)) ;
  dummy >>= 16 ;
  ohmsx100 = (uint16_t) (dummy & 0xFFFF) ;
  temps[0].number = PT100.celsius(ohmsx100) ;
  /*************************/

  /********PROBE2**********/
  rtd = max_2.readRTD();
  dummy = ((uint32_t)(rtd << 1)) * 100 * ((uint32_t) floor(RREF)) ;
  dummy >>= 16 ;
  ohmsx100 = (uint16_t) (dummy & 0xFFFF) ;
  temps[1].number = PT100.celsius(ohmsx100);
  /********************/

  /********PROBE3**********/
  rtd = max_3.readRTD();
  dummy = ((uint32_t)(rtd << 1)) * 100 * ((uint32_t) floor(RREF)) ;
  dummy >>= 16 ;
  ohmsx100 = (uint16_t) (dummy & 0xFFFF) ;
  temps[2].number = PT100.celsius(ohmsx100) ;
  /********************/

  /********PROBE4**********/
  rtd = max_4.readRTD();
  dummy = ((uint32_t)(rtd << 1)) * 100 * ((uint32_t) floor(RREF)) ;
  dummy >>= 16 ;
  ohmsx100 = (uint16_t) (dummy & 0xFFFF) ;
  temps[3].number = PT100.celsius(ohmsx100) ;
  /********************/

  /*******PROBE5*********/
  rtd = max_5.readRTD();
  dummy = ((uint32_t)(rtd << 1)) * 100 * ((uint32_t) floor(RREF)) ;
  dummy >>= 16 ;
  ohmsx100 = (uint16_t) (dummy & 0xFFFF) ;
  temps[4].number = PT100.celsius(ohmsx100) ;
  /*********************/
}
