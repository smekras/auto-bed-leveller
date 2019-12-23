void adjustPlane() {
  moveStepper(motor1, scZ[0]);
  moveStepper(motor2, scZ[1]);
  moveStepper(motor3, scZ[2]);
  moveStepper(motor4, scZ[3]);
}

void calculatePlane(float plane[4], float x[4], float y[4], float z[4]) {
  float p1[3] = {x[0], y[0], z[0]};
  float p2[3] = {x[1], y[1], z[1]};
  float p3[3] = {x[2], y[2], z[2]};
  float p4[3] = {x[3], y[3], z[3]};

  float a1 = p2[0] - p1[0];
  float a2 = p3[0] - p1[0];
  float b1 = p2[1] - p1[1];
  float b2 = p3[1] - p1[1];
  float c1 = p2[2] - p1[2];
  float c2 = p3[2] - p1[2];

  float a = b1 * c2 - b2 * c1;
  float b = a2 * c1 - a1 * c2;
  float c = a1 * b2 - b1 * a2;
  float d = (- a * p1[0] - b * p1[1] - c * p1[2]);

  plane[0] = a;
  plane[1] = b;
  plane[2] = c;
  plane[3] = d;

  showPlaneEquation(plane);

  validatePoint(plane, p4);
}

void checkPlanes (float *u, float *v) {
  // calculate angles between planes
  float dot = (u[0] * v[0]) + (u[1] * v[1]) + (u[2] * v[2]);
  float magU = u[0] * u[0] + u[1] * u[1] + u[2] * u[2];
  float magV = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];

  double angle = (180 / PI) * acos(dot / sqrt(magU * magV));
  Serial.print("Angle: ");
  Serial.println(angle);

  if (angle < 0.5) {
    Serial.println("The planes are parallel.");
    isParallel = true;
  } else {
    Serial.println("The planes are not parallel.");
    isParallel = false;
  }
  Serial.println();
}

void getPlanes() {
  float blPlane[4] = {0};
  float scPlane[4] = {0};
  // calculations for BLTouch plane
  float blAvg = arrayAverage(blZ, POINTS);
  float diff[SCREWS] = {0};
  for (int i = 0; i < SCREWS; i++) {
    diff[i] = blZ[i] - blAvg;
  }

  Serial.println("Calculating extruder plane");
  calculatePlane(blPlane, blX, blY, blZ);

  // calculations for screw/bed plane
  getScrewPositions();

  float scAvg = arrayAverage(scZ, SCREWS);

  Serial.println("Calculating screw plane");
  calculatePlane(scPlane, scX, scY, scZ);

  if (isParallel == false) {
    //check if planes are parallel
    checkPlanes(blPlane, scPlane);

    // change position values
    for (int i = 0; i < SCREWS; i++) {
      Serial.print("Original position: ");
      Serial.println(scZ[i]);
      Serial.print("Offset: ");
      Serial.println(diff[i]);
      scZ[i] = scAvg + diff[i];
      Serial.print("New position: ");
      Serial.println(scZ[i]);
    }
    Serial.println();
    adjustPlane();

    Serial.println("Calculating new screw plane");
    calculatePlane(scPlane, scX, scY, scZ);
    checkPlanes(blPlane, scPlane);

  } else {
    Serial.println("All good");
  }
}

void showPlaneEquation(float plane[4]) {
  Serial.print("Plane equation: ");
  Serial.print(plane[0]);
  Serial.print("x + ");
  Serial.print(plane[1]);
  Serial.print("y + ");
  Serial.print(plane[2]);
  Serial.print("z + ");
  Serial.println(plane[3]);
  Serial.println();

}

void validatePoint(float plane[4], float point[3]) {
  // validate fourth point to prevent issues
  float p4Z = (-(plane[0] * point[0]) - (plane[1] * point[1]) - plane[3]) / plane[2];
  if (abs(point[2] - p4Z) >= 0.1) {
    Serial.print("Fourth point z value should be close to: ");
    Serial.println(p4Z);
    Serial.println();
  }
}

