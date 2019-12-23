float calculateTurns(float lead) {
  float turns = lead / THREAD;
  return turns;
}

void getScrewPositions() {
  // read current positions
  scZ[0] = motor1.currentPosition();
  scZ[1] = motor2.currentPosition();
  scZ[2] = motor3.currentPosition();
  scZ[3] = motor4.currentPosition();
  Serial.println("Got screw positions.");
}

void homeStepper(AccelStepper motor, int zlimit) {
  while (motor.currentPosition() != 0) {
    motor.setSpeed(100);
    motor.runSpeed();
  }
  
  if (digitalRead(zlimit) == HIGH) {
    motor.setCurrentPosition(0);
    Serial.println("Motor homed");
  }
}

void moveStepper(AccelStepper motor, int position) {
  motor.moveTo(position);
  motor.runToPosition();
}

