void parseInput(String com) {
  // insert code
  // sample text: "Recv: // probe at 35.000,15.000 is z=-1.47000"
  String banner = "// probe at";
  String labels = "*";
  String comDir = com.substring(0, com.indexOf(":"));
  String comVal = com.substring(com.indexOf(":") + 2);
  String xValue = comVal.substring(0, comVal.indexOf(","));
  String yValue = comVal.substring(comVal.indexOf(",") + 1, comVal.indexOf("="));
  String zValue = comVal.substring(comVal.indexOf("=") + 1)
  char buff[20];
  char *pBuff;
  int p1, p2;

  // parse command
  if (comDir.equals("Recv")) {
    // reseting all flags and BLZ values
    if (comVal.equalsIgnoreCase("reset")) {
      Serial.println("Reseting values");
      for (int i = 0; i < POINTS; i++) {
        blZ[i] = 0;
      }
      isParallel = false;
      zAxisHomed = false;
      hasReadBL = false;
      hasLabels = false;
      hasBanner = false;
    }

    // normal parsing
    if (hasBanner == false) {
      if (comVal.equals(banner)) {
        hasBanner = true;
      }
    } else {
      if (hasLabels == false) {
        if (comVal.equals(labels)) {
          hasLabels = true;
        }
      } else {     
        rowVal.toCharArray(buff, 20);
        pBuff = strtok(buff, " ");
        
        if (rowInd.equals("0")) {
          p1 = 0;
          p2 = 1;
        }
        if (rowInd.equals("1")) {
          p1 = 2;
          p2 = 3;
        }
          blZ[p1] = atof(pBuff);
          pBuff = strtok(NULL, " ");
          blZ[p2] = atof(pBuff);
      }
      if (blZ[3] != 0) {
        Serial.println("Got BLTouch values");
        hasReadBL = true;
      }
    }
  }
}
