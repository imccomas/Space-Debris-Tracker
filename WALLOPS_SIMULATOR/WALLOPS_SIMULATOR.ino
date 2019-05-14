// Ryan Wade did this
#include <SD.h>

#define BAUD_RATE 19200
#define GSE 2
#define RA 3
#define RB 4
#define TE1 5
#define CHIP_SELECT 8

typedef union {
  uint32_t number;
  uint8_t bytes[4];
} ULONGUNION_t;

typedef union {
  uint8_t number;
  bool bools;
} BOOLUNION_t;


ULONGUNION_t configs[8], timers[4], timerStart[4], timerEnd[4];
BOOLUNION_t logging;
bool launched, relays[4];
File file;
char filename[16];

void setup() {

  Serial.begin(BAUD_RATE); // Computer terminal IO
  Serial1.begin(BAUD_RATE); // Experiment Telemetry in

  pinMode(GSE, OUTPUT);
  pinMode(RA, OUTPUT);
  pinMode(RB, OUTPUT);
  pinMode(TE1, OUTPUT);
  pinMode(CHIP_SELECT, OUTPUT);
  digitalWrite(GSE, HIGH);
  digitalWrite(RA, HIGH);
  digitalWrite(RB, HIGH);
  digitalWrite(TE1, HIGH);

  if (!SD.begin(CHIP_SELECT, 11, 12, 13)) { // SPI pins on mega default to 50, 51, 52
    Serial.println(F("SD card failed, or not present"));
    return;
  }

  if (loadConfig()) {// reads config file
    Serial.println(F("Config file loaded"));
  }
  else {
    Serial.println(F("Failed to load config file."));
  }


  relays[0] = false;
  relays[1] = false;
  relays[2] = false;
  relays[3] = false;
  launched = false;

  printHeader();
  Serial.println();
  printMenu();
  menuLoop();
}

void loop() {

  // Telemetry Logging code
  if (logging.bools) {
    // Code to check serial buffer goes here.
    if (Serial1.available()) {
      file = SD.open(filename, FILE_WRITE);
      while (Serial1.available()) {
        uint8_t f = Serial1.read();
        file.print(f);
        if (liveFeed) Serial.print(f);
      }
      file.close();
    }
  }

  if (launched)
  {
    unsigned long timeNow = millis();
    for (int i = 0; i < 4; ++i) {
      if (timeNow > timerStart[i].number && relays[i] == false) { // Time to turn on relay
        digitalWrite(GSE + i, LOW);
        relays[i] = true;
      }
      else if (timeNow > timerEnd[i].number && relays[i] == true) { // Time to turn off relay
        digitalWrite(GSE + i, HIGH);
        relays[i] = true;
      }
    }
  }
}

// Menus are an ugly necessity in console based applications, working on capacitive thought sensor to replace in the future
void printMenu() {
  Serial.println(F("1. FIRE ZE ROCKET!"));
  Serial.println(F("2. View Timer Event Settings"));
  Serial.println(F("3. EDIT Timer Event Settings"));
  Serial.println(F("4. View Telemetry Logging Settings"));
  Serial.println(F("5. EDIT Telemetry Logging Settings"));
  Serial.println(F("6. MANUAL OVERRIDE MODE"));
  Serial.println(F("7. SAVE Flight Profile to SD"));
  Serial.println(F("8. LOAD Flight Profile from SD"));
  Serial.print(F("\nMake a selection: "));
}
void menuLoop() {

  bool loopin = true;
  while (loopin) {

    while (Serial.available() == 0) // Wait for fat finger input
    {
      delay(5);
    }
    char input = Serial.read(); // Store first byte in seial buffer
    serialFlush();

    switch (input) {
      case '1': // Go for launch
        launch();
        loopin = false;
        break;

      case '2': // print TE settings
        displayTimers();
        printMenu();
        break;

      case '3': // Modify TE settings
        setTimerEvents();
        printMenu();
        break;

      case '4': // View Telem settings
        displayTelem();
        Serial.println();
        printMenu();
        break;

      case '5': //Edit telem settings
        editTelem();
        break;

      case '6': //manual controls
        Serial.println(F("manaual override mode activa.....ERRRRRRRRRRR"));
        break;

      case '7': // save fp
        if (writeConfig()) {
          Serial.println(F("\nConfig saved.\n"));
        }
        else Serial.println(F("\nSomething went wrong saving\n"));
        printMenu();
        break;

      case '8': // load fp
        Serial.println(F("I can't let you load that Hal"));
        break;

      default:
        Serial.println(F("\n\nYou're trying to shoot something into space and this is what you give me?"));
        delay(750);
        for (int i = 0; i < 5; ++i) {
          Serial.print('.');
          delay(750);
        }
        Serial.println();
        delay(750);
        Serial.println(F("Do not disappoint me again\n"));
        break;
    }

  }

}
void serialFlush() {
  while (Serial.available() > 0) {
    char fooflush = Serial.read();
  }
}

void launch() {

  unsigned long timeNow = millis();

  for (int i = 0; i < 4; ++i) {
    timerEnd[i].number = timeNow + timerStart[i].number + timers[i].number;
    timerStart[i].number = timeNow + timerStart[i].number;
  }

  if (logging.bools) { // Create a unique file name for log
    int n = 1;
    snprintf(filename, sizeof(filename), "Telem%03d.txt", n); // includes a three-digit sequenced number in the file name
    while (SD.exists(filename)) {
      n++;
      snprintf(filename, sizeof(filename), "Telem%03d.txt", n);
    }
    file = SD.open(filename, FILE_WRITE);
    file.close();
    Serial.print(F("Telemetry file started as: "));
    Serial.println(filename);
  }
  launched = true;
}

bool loadConfig() {
  if (SD.exists("setup.bin")) {
    file = SD.open("setup.bin", FILE_READ);

    for (int i = 0; i < 8; ++i) {
      for (int z = 0; z < 4; ++z) {
        configs[i].bytes[z] = file.read();
      }
    }
    for (int i = 0; i < 4; ++i) {
      timers[i].number = configs[i].number;
      timerStart[i].number = configs[i + 4].number;
    }
    logging.number = file.read();

    file.close();
    return true;
  }
  else {
    return false;
  }
}

bool writeConfig() {
  file = SD.open("setup.bin", (O_WRITE | O_CREAT | O_TRUNC));
  if (!file) {
    return false;
  }
  for (int i = 0; i < 4; ++i) {
    configs[i].number = timers[i].number;
    configs[i + 4].number = timerStart[i].number;
  }
  for (int i = 0; i < 8; ++i) {
    for (int z = 0; z < 4; ++z) {
      file.write(configs[i].bytes[z]);
    }
  }
  file.write(logging.number);
  file.close();
  return true;
}
bool setTimerEvents() {
  Serial.println(F("\nSet any dwell time to 0 to disable."));
  Serial.print(F("\n\n Enter GSE dwell time in seconds: "));
  timers[0].number = getIntInput() * 1000L;
  Serial.println(timers[0].number / 1000);
  Serial.print(F("Enter redundant dwell time in seconds: "));
  timers[1].number = getIntInput() * 1000L;
  timers[2].number = timers[1].number;
  Serial.println(timers[2].number / 1000);
  Serial.print(F("Enter TE-1 dwell time in seconds: "));
  timers[3].number = getIntInput() * 1000L;
  Serial.println(timers[3].number / 1000);
  Serial.println(F("Enter event activation times, nothing less than 0"));
  Serial.print(F("Enter GSE activation time in seconds: "));
  timerStart[0].number = getIntInput() * 1000L;
  Serial.println(timerStart[0].number / 1000);
  Serial.print(F("Enter redundant activation time in seconds: "));
  timerStart[1].number = getIntInput() * 1000L;
  timerStart[2].number = timerStart[1].number;
  Serial.println(timerStart[2].number / 1000);
  Serial.print(F("Enter TE-1 activation time in seconds: "));
  timerStart[3].number = getIntInput() * 1000L;
  Serial.println(timerStart[3].number / 1000);
  Serial.println(F("\nNew times set.\n\n"));
}
int getIntInput() {

  String inString = "";
  while (Serial.available() == 0) delay(5); // Wait for fat finger input
  delay(20); // Let the buffer get there
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) inString += (char)inChar; // convert the incoming byte to a char and add it to the string
  }
  return inString.toInt();
}

void editTelem() {
  String inString = "";
  Serial.print(F("\nEnable telemetry logging? (Y/N): "));

  while (Serial.available() == 0) delay(5); // Wait for fat finger input
  delay(20); // Let the buffer get there
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isAlpha(inChar)) inString += (char)inChar; // convert the incoming byte to a char and add it to the string
  }
  inString.toUpperCase();
  if (inString[0] == 'Y') {
    Serial.println('Y');
    logging.bools = true;
  }
  else if (inString[0] == 'N') {
    Serial.println('N');
    logging.bools = false;
  }
}
void displayTimers() {
  Serial.println(F("\nCurrent timer settings (in seconds):\n"));
  Serial.println(F("\tGSE  \t\t\tREDUNDANT\t\tTE-1"));
  Serial.println(F("\t---\t\t\t---------\t\t----"));
  Serial.print(F("\tDwell time: "));
  Serial.print(timers[0].number / 1000L);
  Serial.print(F("\t\tDwell time: "));
  Serial.print(timers[1].number / 1000L);
  Serial.print(F("\t\tDwell time: "));
  Serial.println(timers[2].number / 1000L);
  Serial.print(F("\tStart time: "));
  Serial.print(timerStart[0].number / 1000L);
  Serial.print(F("\t\tStart time: "));
  Serial.print(timerStart[1].number / 1000L);
  Serial.print(F("\t\tStart time: "));
  Serial.println(timerStart[2].number / 1000L);
  Serial.println('\n');
}

void displayTelem() {
  Serial.print(F("\nBaud rate: "));
  Serial.println(BAUD_RATE);
  Serial.print(F("LOGGING: "));
  if (logging.bools) Serial.println(F("ENABLED"));
  else Serial.println(F("DISABLED"));
}

void printHeader() {
  Serial.println(F("                                                   ,:"));
  Serial.println(F("                                                 ,' |"));
  Serial.println(F("                                                /   :"));
  Serial.println(F("                                             --'   /"));
  Serial.println(F("                                             \\/ />/"));
  Serial.println(F("                                             / <//_\\"));
  Serial.println(F("                                          __/   /"));
  Serial.println(F("                                          )'-. /"));
  Serial.println(F("                                          ./  :\\"));
  Serial.println(F("                                           /.' '"));
  Serial.println(F("                                         '/'"));
  Serial.println(F("                                         +"));
  Serial.println(F("                                        '"));
  Serial.println(F("                                      `."));
  Serial.println(F("                                  .-\"-"));
  Serial.println(F("                                 (    |"));
  Serial.println(F("                              . .-'  '."));
  Serial.println(F("                             ( (.   )8:"));
  Serial.println(F("                         .'    / (_  )"));
  Serial.println(F("                          _. :(.   )8P  `"));
  Serial.println(F("                      .  (  `-' (  `.   ."));
  Serial.println(F("                       .  :  (   .a8a)"));
  Serial.println(F("                      /_`( \"a `a. )\"'"));
  Serial.println(F("                  (  (/  .  ' )=='"));
  Serial.println(F("                 (   (    )  .8\"   +"));
  Serial.println(F("                   (`'8a.( _(   ("));
  Serial.println(F("                ..-. `8P    ) `  )  +"));
  Serial.println(F("              -'   (      -ab:  )"));
  Serial.println(F("            '    _  `    (8P\"Ya"));
  Serial.println(F("          _(    (    )b  -`.  ) +"));
  Serial.println(F("         ( 8)  ( _.aP\" _a   \\( \\   *"));
  Serial.println(F("       +  )/    (8P   (88    )  )"));
  Serial.println(F("          (a:f   \"     `\"       `"));

  Serial.println(F("*****************************************************"));
  Serial.println(F("**   Wallops Timer Event and Telemetry Simulator   **"));
  Serial.println(F("**                 By: Ryan Wade                   **"));
  Serial.println(F("**          Community College of Aurora            **"));
  Serial.println(F("*****************************************************"));
}
