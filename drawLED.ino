void drawLEDAltSpd(){
  int alt = 0;
  int spd = 0;
  if (!showBaro){
    if (!itemsMain[selItem].option){ // MSL    
      alt = connectorRX.getIndicatedAltitude();
    } else { // AGL
      if (!connectorRX.getOnGround()){
        alt = connectorRX.getFeetAboveGround();
      } else {
        alt = 0;
      }
    }
  } else {
    alt = connectorRX.getKohlmanAltimeter();
  }    
  if (!showGS){  
    spd = connectorRX.getIndicatedAirspeed();
  } else {
    spd = connectorRX.getIndicatedGPSGroundspeed();
  }
  lc.setChar(0,7,' ',false);
  lc.setChar(0,6,(spd/100)%10,false);
  lc.setChar(0,5,(spd/10)%10,false);
  lc.setChar(0,4,spd%10,false);     
  if (alt < 10000){
    lc.setChar(0,3,(alt/1000)%10,false);
    if (!showBaro){
      lc.setChar(0,2,(alt/100)%10,false);
    } else { // decimal point for barometer
      lc.setChar(0,2,(alt/100)%10,true);
    }
    lc.setChar(0,1,(alt/10)%10,false);
    lc.setChar(0,0,alt%10,false);
  } else {    
    lc.setChar(0,3,'F',false);
    lc.setChar(0,2,(alt/10000)%10,false);
    lc.setChar(0,1,(alt/1000)%10,false);
    lc.setChar(0,0,(alt/100)%10,false);
  } 
}

void drawLEDHdgGPS(){
  int hdg = connectorRX.getApHeadingLock();
  int gps = connectorRX.getGpsCourseToSteer();
  if(!connectorRX.getAPMasterOn()){
    lc.setChar(0,7,' ',false);
  } else {
    lc.setChar(0,7,'A',false);
  }
  lc.setChar(0,6,(hdg/100)%10,false);
  lc.setChar(0,5,(hdg/10)%10,false);
  lc.setChar(0,4,hdg%10,connectorRX.getAPHeadingLockOn());
  lc.setChar(0,3,' ',false);
  lc.setChar(0,2,(gps/100)%10,false);
  lc.setChar(0,1,(gps/10)%10,false);
  lc.setChar(0,0,gps%10,false);    
}

void drawLEDCom(){
  if (!itemsMain[selItem].option){ // 1
    displayLEDfreqCOM(connectorRX.getStandbyCom1());
  } else { // 2
    displayLEDfreqCOM(connectorRX.getStandbyCom2());
  }
}

void drawLEDNav(){
    if (!itemsMain[selItem].option){ // 1
      displayLEDfreqNAV(connectorRX.getStandbyNav1());
    } else { // 2
      displayLEDfreqNAV(connectorRX.getStandbyNav2());
    }
}

void displayLEDfreqCOM(long freq){
  lc.setChar(0,7,' ',false);
  lc.setChar(0,6,(freq/100000)%10,false);
  lc.setChar(0,5,(freq/10000)%10,false);
  lc.setChar(0,4,(freq/1000)%10,true);
  lc.setChar(0,3,(freq/100)%10,false);
  lc.setChar(0,2,(freq/10)%10,false);
  lc.setChar(0,1,freq%10,false);
  lc.setChar(0,0,' ',false);
}

void displayLEDfreqNAV(long freq){
  lc.setChar(0,7,' ',false);
  lc.setChar(0,6,(freq/100000)%10,false);
  lc.setChar(0,5,(freq/10000)%10,false);
  lc.setChar(0,4,(freq/1000)%10,true);
  lc.setChar(0,3,(freq/100)%10,false);
  lc.setChar(0,2,(freq/10)%10,false);
  lc.setChar(0,1,freq%10,false);
  lc.setChar(0,0,' ',false);
}

void drawLEDObs(){
  int obs1 = connectorRX.getNavObs1();
  int obs2 = connectorRX.getNavObs2();
  lc.setChar(0,7,' ',false);
  lc.setChar(0,6,(obs1/100)%10,false);
  lc.setChar(0,5,(obs1/10)%10,false);
  lc.setChar(0,4,obs1%10,connectorRX.getAPNav1LockOn());
  lc.setChar(0,3,' ',false);
  lc.setChar(0,2,(obs2/100)%10,false);
  lc.setChar(0,1,(obs2/10)%10,false);
  lc.setChar(0,0,obs2%10,false);  
}

void drawLEDAdfXpndr(){
  long adf = 0;
  long xpndr = 0;
  if (!itemsMain[selItem].option){ // 1
    adf = connectorRX.getAdfStandbyFreq1().toInt();
    xpndr = connectorRX.getTransponderCode1().toInt();
  } else { // 2
    adf = connectorRX.getAdfStandbyFreq2().toInt();
    xpndr = connectorRX.getTransponderCode2().toInt();
  }
  lc.setChar(0,7,(adf/1000)%10,false);
  lc.setChar(0,6,(adf/100)%10,false);
  lc.setChar(0,5,(adf/10)%10,false);
  lc.setChar(0,4,adf%10,false);
  switch (adfDigit){
    case 0:
    lc.setChar(0,4,adf%10,true); 
    break;
    case 1:
    lc.setChar(0,5,(adf/10)%10,true);
    break;
    case 2:
    lc.setChar(0,6,(adf/100)%10,true);
    break;
    default:
    break;
  }
  lc.setChar(0,3,(xpndr/1000)%10,false);
  lc.setChar(0,2,(xpndr/100)%10,false);
  lc.setChar(0,1,(xpndr/10)%10,false);
  lc.setChar(0,0,xpndr%10,false); 
  switch (xpndrDigit){
    case 0:
    lc.setChar(0,0,xpndr%10,true); 
    break;
    case 1:
    lc.setChar(0,1,(xpndr/10)%10,true);
    break;
    case 2:
    lc.setChar(0,2,(xpndr/100)%10,true);
    break;
    case 3:
    lc.setChar(0,3,(xpndr/1000)%10,true);
    break;
    default:
    break;
  }
}

void drawLEDBrightInv(){
  char inv[4] = {' ',' ',' '};
  if (inverted){
    inv[0] = 0; inv[1] = 'n'; inv[2] = ' ';
  } else {
    inv[0] = 0; inv[1] = 'F'; inv[2] = 'F';
  }  
  char bright[4];
  sprintf(bright, "%03u", brightness);
  lc.setChar(0,7,'B',false);
  lc.setChar(0,6,' ',false);
  lc.setChar(0,5,bright[0],false);
  lc.setChar(0,4,bright[1],false);
  lc.setChar(0,3,' ',false);
  lc.setChar(0,2,inv[0],false);
  lc.setChar(0,1,inv[1],false);
  lc.setChar(0,0,inv[2],false);
  //delay(1);   
}

void drawLEDAltVs(){
  long int alt = connectorRX.getApAltLock();
  int vs = connectorRX.getApVerticalSpeed();
  boolean altOn = connectorRX.getAPAltitudeLockOn();
  boolean vsOn = connectorRX.getAPVerticalHoldOn();

  if (alt < 10000){
    if (alt < 0){
      alt = abs(alt);
      if (alt >= 10000){
        lc.setChar(0,7,'-',false);
        lc.setChar(0,6,'-',false);
        lc.setChar(0,5,'-',false);
        lc.setChar(0,4,'-',altOn);

      } else if (alt < 1000){
        lc.setChar(0,7,'-',false);
        lc.setChar(0,6,(alt/100)%10,false);
        lc.setChar(0,5,(alt/10)%10,false);
        lc.setChar(0,4,alt%10,altOn);
      } else {    
        lc.setChar(0,7,'-',false);
        lc.setChar(0,6,(alt/1000)%10,true);
        lc.setChar(0,5,(alt/100)%10,false);
        lc.setChar(0,4,(alt/10)%10,altOn);
      }
    } else {
      lc.setChar(0,7,(alt/1000)%10,false);
      lc.setChar(0,6,(alt/100)%10,false);
      lc.setChar(0,5,(alt/10)%10,false);
      lc.setChar(0,4,alt%10,altOn);
    }
  } else {    
    lc.setChar(0,7,'F',false);
    lc.setChar(0,6,(alt/10000)%10,false);
    lc.setChar(0,5,(alt/1000)%10,false);
    lc.setChar(0,4,(alt/100)%10,altOn);
  }
  if (!vsOn){
      lc.setChar(0,3,'-',false);
      lc.setChar(0,2,'-',false);
      lc.setChar(0,1,'-',false);
      lc.setChar(0,0,'-',vsOn);

  } else {
    if (vs < 0){
      if (abs(vs) >= 1000){
        lc.setChar(0,3,'-',false);
        lc.setChar(0,2,(abs(vs)/1000)%10,true);
        lc.setChar(0,1,(abs(vs)/100)%10,false);
        lc.setChar(0,0,(abs(vs)/10)%10,vsOn);
      } else {
        lc.setChar(0,3,'-',false);
        lc.setChar(0,2,(abs(vs)/100)%10,false);
        lc.setChar(0,1,(abs(vs)/10)%10,false);
        lc.setChar(0,0,abs(vs)%10,vsOn);    
      }
    } else {
      lc.setChar(0,3,(vs/1000)%10,false);
      lc.setChar(0,2,(vs/100)%10,false);
      lc.setChar(0,1,(vs/10)%10,false);
      lc.setChar(0,0,vs%10,vsOn);
    }
  }
}

void drawLEDTrim(){
  int e = connectorRX.getElevatorTrimPct();
  int r = 0;
  if (!itemsMain[selItem].option){
    r = connectorRX.getRudderTrimPct();
  } else {
    r = connectorRX.getAileronTrimPct();
  }
  if (e < 0){
    lc.setChar(0,7,'-', false);
  } else {
    lc.setChar(0,7,' ', false);
  }
  e = abs(e);
  lc.setChar(0,6,(e/100)%10, false);
  lc.setChar(0,5,(e/10)%10, false);
  lc.setChar(0,4,e%10, false);

  if (r < 0){
    lc.setChar(0,3,'-', false);
  } else {
    lc.setChar(0,3,' ', false);
  }

  r = abs(r);  
  lc.setChar(0,2,(r/100)%10, false);
  lc.setChar(0,1,(r/10)%10, false);
  lc.setChar(0,0,r%10, false);
}

void redrawLED() {
  if ((millis() - lastRedraw) >= REDRAW_INTERVAL) {
    if (drawLED != NULL) {
      drawLED();
      lastRedraw = millis();
    }
  }
}