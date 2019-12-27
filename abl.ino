#include <AccelStepper.h>
#include <Bounce2.h>

#define POINTS 4
#define SCREWS 4
#define STEPS 200
#define THREAD 0.7
#define BUTTON 2
#define ZLIMIT1 3
#define ZLIMIT2 14
#define ZLIMIT3 18
#define M1STEP A0
#define M1DIR A1
#define M2STEP A6
#define M2DIR A7
#define M3STEP 46
#define M3DIR 48
#define M4STEP 26
#define M4DIR 28

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
Bounce debouncer = Bounce();

// initialize stepper library
AccelStepper motor1 = AccelStepper(1, M1STEP, M1DIR);
AccelStepper motor2 = AccelStepper(1, M2STEP, M2DIR);
AccelStepper motor3 = AccelStepper(1, M3STEP, M3DIR);
AccelStepper motor4 = AccelStepper(1, M4STEP, M4DIR);

void setup() {
  // initialise serial communication at 9600 bps
  Serial.begin(9600);
  Serial.println("Serial Initialized (9600)");

  // setup pins
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(ZLIMIT1, INPUT);
  pinMode(ZLIMIT2, INPUT);
  pinMode(ZLIMIT3, INPUT);
  pinMode(M1STEP, OUTPUT);
  pinMode(M1DIR, OUTPUT);
  pinMode(M2STEP, OUTPUT);
  pinMode(M2DIR, OUTPUT);
  pinMode(M3STEP, OUTPUT);
  pinMode(M3DIR, OUTPUT);
  pinMode(M4STEP, OUTPUT);
  pinMode(M4DIR, OUTPUT);

  // set maximum speed for motors, over 1000 is unreliable
  motor1.setMaxSpeed(500);
  motor2.setMaxSpeed(500);
  motor3.setMaxSpeed(500);
  motor4.setMaxSpeed(500);

  // set acceleration for motors (steps per second)
  motor1.setAcceleration(20);
  motor2.setAcceleration(20);
  motor3.setAcceleration(20);
  motor4.setAcceleration(20);
  
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
