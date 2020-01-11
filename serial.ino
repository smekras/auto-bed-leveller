void detectScan(String com) {
  // start and end reading values
  if (com.equals(parseStart)) {
    Serial.println("Scanning...");
    isScanning = true;
  }
  if (com.equals(parseEnd)) {
    Serial.println("Saved BLTouch values.");
    isScanning = false;
  }
}

void parseInput(String com) {
  // sample text:
  // "Recv: // probe at 35.000,15.000 is z=-1.47000"
  // "Recv: // probe at 235.000,15.000 is z=2.42000"
  // "Recv: // probe at 235.000,215.000 is z=-1.48000"
  // "Recv: // probe at 35.000,215.000 is z=-5.37000"
  String comDir = com.substring(0, com.indexOf(":"));
  String comVal = com.substring(com.indexOf(":") + 2);
  String junk = comVal.substring(0, 11);
  String xValue = comVal.substring(12, comVal.indexOf(","));
  String yValue = comVal.substring(comVal.indexOf(",") + 1, comVal.indexOf("i"));
  String zValue = comVal.substring(comVal.indexOf("=") + 1);

  // parse command
  detectScan(com);

  int position = 0;
  if (comDir.equals("Recv")) {
    // reseting all flags and BLZ values
    if (comVal.equalsIgnoreCase("reset")) {
      resetValues();
    } else {
      if (isScanning == true) {
        if (arraySearch(corners, pass) == true) {
          blX[position] = xValue.toFloat();
          blY[position] = yValue.toFloat();
          blZ[position] = zValue.toFloat();
          position++;
        }
        pass++;
      } else {
        Serial.println("Reset values to scan again.");
      }
    }
  }
}

void resetValues() {
  Serial.println("Reseting values");
  for (int i = 0; i < POINTS; i++) {
    blZ[i] = 0;
  }
  isParallel = false;
  motor1.setCurrentPosition(-1000);
  motor2.setCurrentPosition(-1000);
  motor3.setCurrentPosition(-1000);
  motor4.setCurrentPosition(-1000);
  pass = 0;
}
