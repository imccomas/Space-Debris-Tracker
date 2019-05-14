// Ryan Wade did this
#include <SD.h>

#define BAUD_RATE 19200
#define GSE 2
#define RA 3
#define RB 4
#define TE1 5

unsigned long timers[3], timerStart[3], timerEnd[3];
bool launched, relays[3];

void setup() {

  Serial.begin(BAUD_RATE); // Computer terminal IO
  Serial1.begin(BAUD_RATE); // Experiment Telemetry in

  pinMode(GSE, OUTPUT);
  pinMode(RA, OUTPUT);
  pinMode(RB, OUTPUT);
  pinMode(TE1, OUTPUT);
  digitalWrite(GSE, HIGH);
  digitalWrite(RA, HIGH);
  digitalWrite(RB, HIGH);
  digitalWrite(TE1, HIGH);

  timers[0] = 15 * 60 * 1000;
  timers[1] = 10 * 60 * 1000;
  timers[2] = 5 * 60 * 1000;

  timerStart[0] = 0;
  timerStart[1] = 5 * 60 * 1000;
  timerStart[2] = 7 * 60 * 1000;

  relays[0] = false;
  relays[1] = false;
  relays[2] = false;

  launched = false;

  printHeader();
  Serial.println();
  printMenu();
}

void loop() {

  if (launched)
  {
    Serial.print("Launched");
  }

}

// Prints a header because why not?
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

// Menus are an ugly necessity in console based applications, working on capacitive thought sensor to replace in the future
void printMenu() {
  Serial.println(F("1. FIRE ZE ROCKET!"));
  Serial.println(F("2. View Timer Event Settings"));
  Serial.println(F("3. EDIT Timer Event Dettings"));
  Serial.println(F("4. View Telemetry Logging Settings"));
  Serial.println(F("5. EDIT Telemetry Logging Settings"));
  Serial.println(F("6. MANUAL OVERRIDE MODE"));
  Serial.println(F("7. SAVE Flight Profile to SD"));
  Serial.println(F("8. LOAD Flight Profile from SD"));
  Serial.println(F("\nMake a selection: "));

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
        Serial.println(F("WoooSSShhhhHHH!"));
        Serial.println(F("JK This will be finished later"));
        loopin = false;
        break;

      case '2': // print TE settings
        Serial.println(F("Timer Event Settings go here"));
        break;

      case '3': // Modify TE settings
        Serial.println(F("Make changes to your TEs here"));
        break;

      case '4': // View Telem settings
        Serial.println(F("Imangine your telemetry settings here for now"));
        break;

      case '5': //Edit telem settings
        Serial.println(F("Edit Telem Settings"));
        break;

      case '6': //manual controls
        Serial.println(F("manaual override mode activa.....ERRRRRRRRRRR"));
        break;

      case '7': // save fp
        Serial.println(F("I wish I could save your flight profile"));
        break;

      case '8': // load fp
        Serial.println(F("I can't let you load that Hal"));
        break;

      default:
        Serial.println(F("\n\nYou're trying to shoot something into space and this is what you give me?"));
        delay(750);
        Serial.println();
        delay(750);
        Serial.println();
        delay(750);
        Serial.println(F("Do not disappoint me again\n"));
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

  for (int i = 0; i < 3; ++i) {
    timerEnd[i] = timeNow + timerStart[i] + timers[i];
    timerStart[i] = timeNow + timerStart[i];
  }
  launched = true;
}
