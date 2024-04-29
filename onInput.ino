//These are all the on Input functions

void mainUp()  { //up function. selection sign ">" will move upwards.
    iArr = iArr - 1;
    if (iArr < 0)
    {
      iArr = 2;
      if (firstItem > 0){
        firstItem = firstItem - 3;
      } else {
        iArr = 2;
        firstItem = mainMenuLength - 3;
      }      
    }
    selItem = firstItem + iArr;
    page = selItem / 3;
    updateOLED();
    scrollLED();
}

void mainDown() { // down function. selection sign ">" will move downwards.
    iArr = iArr + 1;
    if (iArr > 2)
    {
      iArr = 0;
      if (firstItem < (mainMenuLength - 3)){
        firstItem = firstItem + 3;
      } else {
        iArr = 0;
        firstItem = 0;
      } 
    }
    selItem = firstItem + iArr;
    page = selItem / 3;
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

void onSwitchKnotsMach(){
  if (!showGS){
    if (!showIAS){
      pageSPD = "IAS";
      showIAS = true;
    } else {
      pageSPD = "M  ";
      showIAS = false;
    }
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
    itemsMain[selItem].entry = "COM2 Sby";
    itemsMain[selItem].option = true;
  } else { // 2
    itemsMain[selItem].entry = "COM1 Sby";
    itemsMain[selItem].option = false;
  }
  updateOLED();
}

void onNavSelect12(){
  if (!itemsMain[selItem].option){ // 1
    itemsMain[selItem].entry = "NAV2 Sby";
    itemsMain[selItem].option = true;
  } else { // 2
    itemsMain[selItem].entry = "NAV1 Sby";
    itemsMain[selItem].option = false;
  }
  updateOLED();
}

void onAdfSelect12(){
  if (!itemsMain[selItem].option){ // 1
    itemsMain[selItem].entry = "ADF2 Sby";
    itemsMain[selItem].option = true;
  } else { // 2
    itemsMain[selItem].entry = "ADF1 Sby";
    itemsMain[selItem].option = false;
  }
  adfDigit = 2;
  updateOLED();
}

// removed as transponder 2 is not used by BAD or MSFS
/*
void onXpndrSelect12(){
  if (!itemsMain[selItem].option){ // 1
    itemsMain[selItem].entry = "XPNDR2  ";
    itemsMain[selItem].option = true;
  } else { // 2
    itemsMain[selItem].entry = "XPNDR1  ";
    itemsMain[selItem].option = false;
  }
  xpndrDigit = 3;
  updateOLED();
}
*/

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

void onAdfSwitch(){
  // not implemented in BAD
  /*
  if (!itemsMain[selItem].option){ // 1
    connectorTX.send(sendSwapAdf1);
  } else { // 2
    connectorTX.send(sendSwapAdf2);
  }
  */  
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

void onAdfHzIncrease(){
  if (!itemsMain[selItem].option){ // 1
    connectorTX.send(sendAdf1TenthsInc);
  } else { // 2
    connectorTX.send(sendAdf2RadioThenthsInc);
  }  
}

void onAdfHzDecrease(){
  if (!itemsMain[selItem].option){ // 1
    connectorTX.send(sendAdf1TenthsInc);
  } else { // 2
    connectorTX.send(sendAdf2RadioThenthsDec);
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

void onXpndrIdent(){

}

void onXpndrModeIncrease(){

}

void onXpndrModeDecrease(){
  
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
  OLEDbright = OLEDbright + 17;
  LCDbright = LCDbright + 1;
  if (OLEDbright > 255){
    OLEDbright = 0;
  }
  if (LCDbright > 15){
    LCDbright = 0;
  }
  myOLED.setBrightness(OLEDbright);
  lc.setIntensity(0,LCDbright);
}

void onBrightDecrease(){
  OLEDbright = OLEDbright - 17;
  LCDbright = LCDbright - 1;
  if (OLEDbright < 0){
    OLEDbright = 255;
  }
  if (LCDbright < 0){
    LCDbright = 15;
  }
  myOLED.setBrightness(OLEDbright);
  lc.setIntensity(0,LCDbright);  
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
   
void onClkToggle(){
  showZ = !showZ;
}

void onTmrToggle(){
  if (!itemsMain[selItem].option){
    // show stopwatch
    currStateStw++;
    if (currStateStw > 2){
      currStateStw = 0;
    }
    // Has timer switched?
    switch (currStateStw){
      case 0:
      millisStartStw = 0;
      break;
      case 1:
      millisStartStw = millis();
      break;
      case 2:
      break;
    }
  } else {
    // show timer
    currStateTmr++;
    if (currStateTmr > 2){
      currStateTmr = 0;
    }
    // Has timer switched?
    switch (currStateTmr){
      case 0:
      milTmrStart = 0;
      break;
      case 1:
      milTmrStart = millis();
      break;
      case 2:
      break;
    }
  }
}

void onTmrSwitch(){
  if (!itemsMain[selItem].option){ // 1
    itemsMain[selItem].entry = "CLK  TMR";
    itemsMain[selItem].option = true;
  } else { // 2
    itemsMain[selItem].entry = "CLK  STW";
    itemsMain[selItem].option = false;
  }
  updateOLED();  
}

// Right Turn Increases Minutes of Timer in Increments of 1
void onTmrInc(){
  if(itemsMain[selItem].option && currStateTmr == 0){
    // Timer can only be set if reset
    minsTmr++;
    if (minsTmr > 99){
      minsTmr = 99;
    }
    milTmrSpan = minsTmr * 60 * 1000 + secsTmr * 1000;
  }
}

// Left Turn Decreases Seconds of Timer in Increments of 10
void onTmrDec(){
  if(itemsMain[selItem].option && currStateTmr == 0){
    // Timer can only be set if reset
    secsTmr = secsTmr - 10;
    if (secsTmr < 0){
      if (minsTmr > 0){
        secsTmr = 50;
        minsTmr--;
      } else {
        minsTmr = 0;
        secsTmr = 0;
      }
    }
    milTmrSpan = minsTmr * 60 * 1000 + secsTmr * 1000;
  }
}

void onShowBADVersion(){
  if(!showBADversion){
    showBADversion = true;
    updateOLED();
  } else {
    showBADversion = false;
    updateOLED();
  }
}

void onOATSwitch(){
  if (!itemsMain[selItem].option){ // 1
    itemsMain[selItem].entry = "WND  OAT";
    itemsMain[selItem].option = true;
  } else { // 2
    itemsMain[selItem].entry = "WND  SPD";
    itemsMain[selItem].option = false;
  }
  updateOLED(); 
} 

void onWNDPerpToggle(){
  if(!showWindPerp){
    showWindPerp = true;
  }else{
    showWindPerp = false;
  }
}

void onOATToggle(){
  if (itemsMain[selItem].option){
    if(!showOATFahrenheid){
      showOATFahrenheid = true;
    }else{
      showOATFahrenheid = false;
    }
  }
}