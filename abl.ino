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
char c;
String command;
String parseStart = "Send: G29";
String parseEnd = "Recv: Mesh Bed Leveling Complete";
int pass = 0;
float blX[POINTS] = {0};
float blY[POINTS] = {0};
float blZ[POINTS] = {0};
float scX[SCREWS] = {15, 220, 220, 15};
float scY[SCREWS] = {15, 15, 220, 220};
float scZ[SCREWS] = {0};
bool isScanning = false;
bool isParallel = false;
bool zAxisHomed = false;
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
  if (Serial.available() > 0) {
    command = Serial.readString();
    command.trim();
    parseInput(command);
  }

  if (pass >= POINTS) {
    if (isParallel == false) {
      getPlanes();
    }
  }

  if (value == LOW) {
    Serial.println("G29");
  }
}
