#include <Bounce2.h>
//#include <OctoPrintAPI.h>
#include <Stepper.h>

#define POINTS 4
#define SCREWS 4
#define STEPS 200
#define THREAD 0.7

#define BUTTON 2
#define ZLIMIT 7 // subject to change
#define M1PIN1 8
#define M1PIN2 9
#define M1PIN3 10
#define M1PIN4 11


// global variables
String command;
String startLab = "Send: G29";
String endLabel = "Recv: Mesh Bed Leveling Complete";
char c;
float blX[POINTS] = {-8, 8, 8, -8};
float blY[POINTS] = {-10, -10, 10, 10};
float blZ[POINTS] = {0};
float scX[SCREWS] = {15, 215, 215, 15};
float scY[SCREWS] = {35, 35, 235, 235};
float scZ[SCREWS] = {0};
bool readValues = false;
bool zAxisHomed = false;
bool hasBanner = false;
bool hasLabels = false;
bool hasReadBL = false;
bool isParallel = false;
Bounce debouncer = Bounce();

// initialize stepper library
Stepper motor1(STEPS, M1PIN1, M1PIN2, M1PIN3, M1PIN4);

void setup() {
  // set motor speed at whatever value
  motor1.setSpeed(60);
  
  // initialise serial communication at 9600 bps
  Serial.begin(9600);
  Serial.println("Serial Initialized (9600)");

  // setup pins
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(ZLIMIT, INPUT);
  pinMode(M1PIN1, OUTPUT);
  pinMode(M1PIN2, OUTPUT);
  pinMode(M1PIN3, OUTPUT);
  pinMode(M1PIN4, OUTPUT);

  // after setting up the button, setup the Bounce instance :
  debouncer.attach(BUTTON);
  debouncer.interval(5); // interval in ms
}

void loop() {
  // Update the Bounce instance :
  debouncer.update();

  // Get the updated value :
  int value = debouncer.read();
  
  // reading BLTouch values from Octoprint
  c = Serial.read();
  if (Serial.available()>0) {
    if (c == '\n') {
      // start and end reading values
      if (command.equals(startLab)) {
        readValues = true;
      }
      if (command.equals(endLabel)) {
        readValues = false;
      }
      if ((readValues) == true) {
        parseInput(command);
      }
      command = "";
    } else {
      // only include normal characters
      if (' ' <= c && c <= '~') {
        command += c;
      }
    }
  }
  
  if (hasReadBL == true) {
    if (isParallel == false) {
      getPlanes();
    }
  }

  if (value == LOW) {
    Serial.println("G28");
  }
}
