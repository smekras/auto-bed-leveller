bool detectScan(String com) {
  // start and end reading values
  if (com.equals(parseStart)) {
    return true;
  }
  if (com.equals(parseEnd)) {
    return false;
  }
}

void parseInput(String com) {
  // sample text:
  // "Recv: // probe at 35.000,15.000 is z=-1.47000"
  // "Recv: // probe at 235.000,15.000 is z=2.42000"
  // "Recv: // probe at 235.000,215.000 is z=-1.48000"
  // "Recv: // probe at 35.000,215.000 is z=1.67000"
  String comDir = com.substring(0, com.indexOf(":"));
  String comVal = com.substring(com.indexOf(":") + 2);
  String junk = comVal.substring(0, 11);
  String xValue = comVal.substring(12, comVal.indexOf(","));
  String yValue = comVal.substring(comVal.indexOf(",") + 1, comVal.indexOf("i"));
  String zValue = comVal.substring(comVal.indexOf("=") + 1);
  char buff[10];
  char *pBuff;

  Serial.println(com);
  Serial.println(xValue);
  Serial.println(yValue);
  Serial.println(zValue);
  Serial.println(pass);
  // parse command
  if (comDir.equals("Recv")) {
    // reseting all flags and BLZ values
    if (comVal.equalsIgnoreCase("reset")) {
      resetValues();
    } else {
      if (detectScan(comVal) == true) {
        if (pass < POINTS) {
          xValue.toCharArray(buff, 10);
          pBuff = buff;
          blX[pass] = atof(pBuff);
          yValue.toCharArray(buff, 10);
          pBuff = buff;
          blY[pass] = atof(pBuff);
          zValue.toCharArray(buff, 10);
          pBuff = buff;
          blZ[pass] = atof(pBuff);
          pass++;
          Serial.println(pass);
        } else {
          Serial.println("BLTouch Values saved.");
          Serial.println(pass);
          pass = 0;
        }
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
  zAxisHomed = false;
  pass = 0;
}

