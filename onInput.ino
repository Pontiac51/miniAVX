//These are all the on Input functions

void mainUp()  { //up function. selection sign ">" will move upwards.
    iArr = iArr - 1;
    if (iArr < 0)
    {
      iArr = 0;
      if (firstItem > 0){
        firstItem--;
      } else {
        iArr = 2;
        firstItem = mainMenuLength - 3;
      }      
    }
    selItem = firstItem + iArr;
    updateOLED();
    scrollLED();
}

void mainDown() { // down function. selection sign ">" will move downwards.
    iArr = iArr + 1;
    if (iArr > 2)
    {
      iArr = 2;
      if (firstItem < (mainMenuLength - 3)){
        firstItem++;
      } else {
        iArr = 0;
        firstItem = 0;
      } 
    }
    selItem = firstItem + iArr;
    updateOLED();
    scrollLED();
}

void onSelectRadarAlt(){
  if (!showBaro){
    if (!itemsMain[selItem].option){ // 1
      pageALT = "AGL";
      itemsMain[selItem].option = true;
    } else { // 2
      pageALT = "MSL";
      itemsMain[selItem].option = false;
    }
    itemsMain[selItem].entry = pageSPD + "  " + pageALT;
    updateOLED();
  }
}

void onSelectGroundSpeed(){
  showGS = !showGS;
  if (showGS){
    pageSPD = "GS ";
  } else {
    pageSPD = "IAS";
  }
  itemsMain[selItem].entry = pageSPD + "  " + pageALT;
  updateOLED();
}

void onSelectBaro(){
  showBaro = !showBaro;
  if (showBaro){
    pageALT = "QNH";
  } else {
    if (!itemsMain[selItem].option){ // 1
      pageALT = "MSL";
    } else { // 2
      pageALT = "AGL";
    }
  }
  itemsMain[selItem].entry = pageSPD + "  " + pageALT;
  updateOLED();
}

void onComSelect12(){
  if (!itemsMain[selItem].option){ // 1
    itemsMain[selItem].entry = "COM2STBY";
    itemsMain[selItem].option = true;
  } else { // 2
    itemsMain[selItem].entry = "COM1STBY";
    itemsMain[selItem].option = false;
  }
  updateOLED();
}

void onNavSelect12(){
  if (!itemsMain[selItem].option){ // 1
    itemsMain[selItem].entry = "NAV2STBY";
    itemsMain[selItem].option = true;
  } else { // 2
    itemsMain[selItem].entry = "NAV1STBY";
    itemsMain[selItem].option = false;
  }
  updateOLED();
}

void onAdfXpndrSelect12(){
  if (!itemsMain[selItem].option){ // 1
    itemsMain[selItem].entry = "ADF2 XP2";
    itemsMain[selItem].option = true;
  } else { // 2
    itemsMain[selItem].entry = "ADF1 XP1";
    itemsMain[selItem].option = false;
  }
  adfDigit = 2;
  xpndrDigit = 3;
  updateOLED();
}

void onTrimSwitch(){
  if (!itemsMain[selItem].option){ // 1
    itemsMain[selItem].entry = "ELV  AIL";
    itemsMain[selItem].option = true;
  } else { // 2
    itemsMain[selItem].entry = "ELV  RUD";
    itemsMain[selItem].option = false;
  }
  updateOLED();  
}

void onAPMasterToggle(){
  connectorTX.send(sendApMasterOn);
}

void onAPHdgToggle(){
  if(!connectorRX.getAPHeadingLockOn()){
    connectorTX.send(sendAPHeadingHoldOn); 
  } else {
    connectorTX.send(sendAPHeadingHoldOff);
  }
}

void onHdgIncrease(){
  connectorTX.send(sendHeadingBugInc);
}

void onHdgDecrease(){
  connectorTX.send(sendHeadingBugDec);
}

void onBaroIncrease(){
  connectorTX.send(sendKohlsmanInc);
}

void onBaroDecrease(){
  connectorTX.send(sendKohlsmanDec);
}

void onAPAltToggle(){
  connectorTX.send(sendAPAltitudeHold);
}

void onAltIncrease(){
  connectorTX.send(sendAPAltitudeInc);
}

void onAltDecrease(){
  connectorTX.send(sendAPAltitudeDec);
}

void onAPVSToggle(){
  connectorTX.send(sendAPVSHold);
}

void onVsIncrease(){
  connectorTX.send(sendAPVSInc);
}

void onVsDecrease(){
  connectorTX.send(sendAPVSDec);
}

void onComSwitch(){
    if (!itemsMain[selItem].option){ // 1
      connectorTX.send(sendSwapCom1);
    } else { // 2
      connectorTX.send(sendSwapCom2);
    }  
}

void onComMhzIncrease(){
  if (!itemsMain[selItem].option){ // COM 1
    connectorTX.send(sendCom1WholeInc);
  } else { // COM 2
    connectorTX.send(sendCom2WholeInc);
  }  
}

void onComMhzDecrease(){
  if (!itemsMain[selItem].option){ // 1
    connectorTX.send(sendCom1WholeDec);
  } else { // 2
    connectorTX.send(sendCom2WholeDec);
  }
}

void onComKhzIncrease(){
  if (!itemsMain[selItem].option){ // COM 1
    connectorTX.send(sendCom1FractInc);
  } else { // COM 2
    connectorTX.send(sendCom2FractInc);
  }  
}

void onComKhzDecrease(){
  if (!itemsMain[selItem].option){ // 1
    connectorTX.send(sendCom1FractDecr);
  } else { // 2
    connectorTX.send(sendCom2FractDecr);
  }  
}

void onNavSwitch(){
  if (!itemsMain[selItem].option){ // 1
    connectorTX.send(sendSwapNav1);
  } else { // 2
    connectorTX.send(sendSwapNav2);
  }  
}

void onNavMhzIncrease(){
  if (!itemsMain[selItem].option){ // 1
    connectorTX.send(sendIncWholeNav1);
  } else { // 2
    connectorTX.send(sendIncWholeNav2);
  }  
}

void onNavMhzDecrease(){
  if (!itemsMain[selItem].option){ // 1
    connectorTX.send(sendDecWholeNav1);
  } else { // 2
    connectorTX.send(sendDecWholeNav2);
  }  
}

void onNavKhzIncrease(){
  if (!itemsMain[selItem].option){ // 1
    connectorTX.send(sendIncFractNav1);
  } else { // 2
    connectorTX.send(sendIncFractNav2);
  }  
}

void onNavKhzDecrease(){
  if (!itemsMain[selItem].option){ // 1
    connectorTX.send(sendDecFractNav1);
  } else { // 2
    connectorTX.send(sendDecFractNav2);
  }  
}

void onAdfNext(){
  adfDigit--;
  if (adfDigit < 0){
    adfDigit = 2;
  }  
}

void onAdfIncrease(){
  if (!itemsMain[selItem].option){ // 1
    switch (adfDigit){
      case 0:
      connectorTX.send(sendAdf1Inc);
      break;
      case 1:
      connectorTX.send(sendAdf10Inc);
      break;
      case 2:
      connectorTX.send(sendAdf100Inc);
      break;
      default:
      break;
    }
  } else { // 2
    switch (adfDigit){
      case 0:
      connectorTX.send(sendAdf21Inc);
      break;
      case 1:
      connectorTX.send(sendAdf210Inc);
      break;
      case 2:
      connectorTX.send(sendAdf2100Inc);
      break;
      default:
      break;
    }
  }  
}

void onAdfDecrease(){
  if (!itemsMain[selItem].option){ // 1
    switch (adfDigit){
      case 0:
      connectorTX.send(sendAdf1Dec);
      break;
      case 1:
      connectorTX.send(sendAdf10Dec);
      break;
      case 2:
      connectorTX.send(sendAdf100Dec);
      break;
      default:
      break;
    }
  } else { // 2
    switch (adfDigit){
      case 0:
      connectorTX.send(sendAdf21Dec);
      break;
      case 1:
      connectorTX.send(sendAdf210Dec);
      break;
      case 2:
      connectorTX.send(sendAdf2100Dec);
      break;
      default:
      break;
    }
  }  
}

void onXpndrNext(){
  xpndrDigit--;
  if (xpndrDigit < 0){
    xpndrDigit = 3;
  }  
}

void onXpndrIncrease(){
  if (!itemsMain[selItem].option){ // 1
    switch (xpndrDigit){
      case 0:
      connectorTX.send(sendXpndr1Inc);
      break;
      case 1:
      connectorTX.send(sendXpndr10Inc);
      break;
      case 2:
      connectorTX.send(sendXpndr100Inc);
      break;
      case 3:
      connectorTX.send(sendXpndr1000Inc);
      break;
      default:
      break;
    }
  }  
}

void onXpndrDecrease(){
  if (!itemsMain[selItem].option){ // 1
    switch (xpndrDigit){
      case 0:
      connectorTX.send(sendXpndr1Dec);
      break;
      case 1:
      connectorTX.send(sendXpndr10Dec);
      break;
      case 2:
      connectorTX.send(sendXpndr100Dec);
      break;
      case 3:
      connectorTX.send(sendXpndr1000Dec);
      break;
      default:
      break;
    }
  }  
}

void onOBS1Increase(){
  connectorTX.send(sendVor1ObiInc);
}

void onOBS1Decrease(){
  connectorTX.send(sendVor1ObiDec);
}

void onOBS2Increase(){
  connectorTX.send(sendVor2ObiInc);
}

void onOBS2Decrease(){
  connectorTX.send(sendVor2ObiDec);
}

void onAPGPSToggle(){
  connectorTX.send(sendToggleGPSDrivesNav1);
}

void onAPNAVToggle(){
  if(!connectorRX.getAPNav1LockOn()){
    connectorTX.send(sendAPNav1HoldOn);
  } else {
    connectorTX.send(sendAPNav1HoldOff);
  }
}

void onBrightIncrease(){
  brightness = brightness + 10;
  if (brightness > 255){
    brightness = 0;
  }
  myOLED.setBrightness(brightness);
}

void onBrightDecrease(){
  brightness = brightness - 10;
  if (brightness < 0){
    brightness = 255;
  }
  myOLED.setBrightness(brightness);  
}

void onInvSwitch(){
  if (!inverted){
    inverted = true;    
  } else {
    inverted = false;      
  }
  myOLED.invert(inverted);  
}

void onElvTrimUp(){
  connectorTX.send(sendElevTrimUp);
}

void onElvTrimDn(){
  connectorTX.send(sendElevTrimDn);
}

void onRudTrimL(){
  if (!itemsMain[selItem].option){
    connectorTX.send(sendRudderTrimLeft);
  } else {
    connectorTX.send(sendAileronTrimLeft);
  }
}

void onRudTrimR(){
  if (!itemsMain[selItem].option){
    connectorTX.send(sendRudderTrimRight);
  } else {
    connectorTX.send(sendAileronTrimRight);
  }
}
