#include <BitsAndDroidsFlightConnector.h>
#include <LedControl.h>
#include <OLED_I2C.h>
#include <Encoder.h>

String version = "1.94.090";
String BADversion = "0.9.0";

// data type for button
struct Button {
  uint8_t gpioSw;
  uint8_t lastStateSw;
};

//data type for rotary
struct Rotary {
  long oldPosition;
  uint8_t gpioClk;
  uint8_t gpioDt;
  uint8_t lastStateClk;
  uint8_t lastStateDt;
  Button button;
  Encoder enc;
};

#define ROTARIES 3
Rotary rotaries[ROTARIES];

// debouncing
unsigned long lastSample = 0;
int debounce = 5; // time in ms

//BAD - true for pro-micro / Leonardo
BitsAndDroidsFlightConnector connectorTX(false, &Serial);
BitsAndDroidsFlightConnector connectorRX(false, &SerialUSB);

// define 7-segment display
#define REDRAW_INTERVAL 50
long lastRedraw = 0;
LedControl lc=LedControl(12,11,10,1);

// define OLED
OLED myOLED(SDA, SCL);
extern uint8_t TinyFont[];
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
int brightness = 0;
boolean inverted = false;

//data tpye for menu item
struct menuItem {
  String entry;
  boolean option = false; 
  void (*onSelect)();
  void (*drawLED)(); 
};

// Init Menu Structure
#define MAX_ITEMS 20
menuItem itemsMain[MAX_ITEMS];
uint8_t mainMenuLength = 0;
int menu = 0;
int firstItem = 0;
int selItem = 0;

int idxApHdgGps = 3; // set where AP HDG-GPS page is in list
int idxApAltVs = 2; // set where AP ALT-VS page is in list
int idxApObs = 4; // set where AP OBS page is in list

// Init Cursor
int yArr[] = {14, 31, 48};
int iArr = 0;

// Init ADF & XPNDR
int adfDigit = 2;
int xpndrDigit = 3;

// Init GPS state - not supported by BAD - default is ON
boolean getAPGpsLockOn = true;

//Init Trim Step
boolean isElvTrimBIG = false;
boolean isRudTrimBIG = false;
boolean isAilTrimBIG = false;

//Init ALT page
boolean showBaro = false;
String pageALT = "MSL";

//Init SPD page
boolean showGS = false;
String pageSPD = "IAS";

// Init AP Modes
String ApHDGOn = " ";
String ApGPSOn = " ";
String ApALTOn = " ";
String ApVSOn = " ";
String ApOBSOn = " ";

// constructor for menu item
void addMenuItem(String entry, void (*onSelect)()) {
    if (mainMenuLength >= MAX_ITEMS) return;
    itemsMain[mainMenuLength].entry = entry;
    itemsMain[mainMenuLength].onSelect = onSelect;
    mainMenuLength++;
}

// constructor for menu item callback
void (*onMainSelect)();
void (*onLeftSelect)();
void (*onLeftCW)();
void (*onLeftCCW)();
void (*onRightSelect)();
void (*onRightCW)();
void (*onRightCCW)();
void (*drawLED)();

// callbacks for all menu items (make sure unused are NULL) 
void onAltSpeedSelect() {
  drawLED = &drawLEDAltSpd;
  onMainSelect = &onSelectRadarAlt;
  onLeftSelect = &onSelectGroundSpeed;
  onLeftCW = NULL;
  onLeftCCW = NULL;
  onRightSelect = &onSelectBaro;
  onRightCW = &onBaroIncrease;
  onRightCCW = &onBaroDecrease;
}

void onHdgGPSSelect() {
    drawLED = &drawLEDHdgGPS;
    onMainSelect = &onAPMasterToggle;
    onLeftSelect = &onAPHdgToggle;
    onLeftCW = &onHdgIncrease;
    onLeftCCW = &onHdgDecrease;
    onRightSelect = &onAPGPSToggle;
    onRightCW = NULL;
    onRightCCW = NULL;
}

void onAltVsSelect() {
    drawLED = &drawLEDAltVs;
    onMainSelect = &onAPMasterToggle;
    onLeftSelect = &onAPAltToggle;
    onLeftCW = &onAltIncrease;
    onLeftCCW = &onAltDecrease;
    onRightSelect = &onAPVSToggle;
    onRightCW = &onVsIncrease;
    onRightCCW = &onVsDecrease;
}

void onComSelect() {
    drawLED = &drawLEDCom;
    onMainSelect = &onComSelect12;
    onLeftSelect = &onComSwitch;
    onLeftCW = &onComMhzIncrease;
    onLeftCCW = &onComMhzDecrease;
    onRightSelect = &onComSwitch;
    onRightCW = &onComKhzIncrease;
    onRightCCW = &onComKhzDecrease;
}

void onNavSelect() {
    drawLED = &drawLEDNav;
    onMainSelect = &onNavSelect12;
    onLeftSelect = &onNavSwitch;
    onLeftCW = &onNavMhzIncrease;
    onLeftCCW = &onNavMhzDecrease;
    onRightSelect = &onNavSwitch;
    onRightCW = &onNavKhzIncrease;
    onRightCCW = &onNavKhzDecrease;
}

void onAdfXpndrSelect() {
    drawLED = &drawLEDAdfXpndr;
    onMainSelect = &onAdfXpndrSelect12;
    onLeftSelect = &onAdfNext;
    onLeftCW = &onAdfIncrease;
    onLeftCCW = &onAdfDecrease;
    onRightSelect = &onXpndrNext;
    onRightCW = &onXpndrIncrease;
    onRightCCW = &onXpndrDecrease;
}

void onOBSSelect() {
    drawLED = &drawLEDObs;
    onMainSelect = &onAPMasterToggle;
    onLeftSelect = &onAPNAVToggle;
    onLeftCW = &onOBS1Increase;
    onLeftCCW = &onOBS1Decrease;
    onRightSelect = NULL;
    onRightCW = &onOBS2Increase;
    onRightCCW = &onOBS2Decrease;
}

void onTrimSelect() {
    drawLED = &drawLEDTrim;
    onMainSelect = &onTrimSwitch;    
    onLeftSelect = &onElvTrimBIG;
    onLeftCW = &onElvTrimUp;
    onLeftCCW = &onElvTrimDn;
    onRightSelect = &onRudTrimBIG;
    onRightCW = &onRudTrimR;
    onRightCCW = &onRudTrimL;
}

void onBrightInvSelect() {
    drawLED = &drawLEDBrightInv;
    onMainSelect = NULL;   
    onLeftSelect = NULL; 
    onLeftCW = &onBrightIncrease;
    onLeftCCW = &onBrightDecrease;
    onRightSelect = onInvSwitch;    
    onRightCW = NULL;
    onRightCCW = NULL;
}

void setupRotary(Rotary *cur, uint8_t gpioClk, uint8_t gpioDt , uint8_t gpioSw) {
  pinMode(gpioSw, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(gpioSw), loopRotaries, RISING);
  cur->enc = Encoder(gpioClk, gpioDt);
  cur->oldPosition = -999;
  cur->gpioClk = gpioClk;
  cur->gpioDt = gpioDt;
  cur->button.gpioSw = gpioSw;
  cur->button.lastStateSw = LOW;
}

void setup() {
  // menu - first item is 0
  // menu create by aviate - navigate - communicate
  addMenuItem("ELV  RUD", &onTrimSelect);
  addMenuItem(pageSPD + "  " + pageALT, &onAltSpeedSelect);
  addMenuItem("ALT" + ApALTOn + ApVSOn + "V/S", &onAltVsSelect);
  addMenuItem("HDG" + ApHDGOn + ApGPSOn + "GPS", &onHdgGPSSelect);  
  addMenuItem("OBS" + ApOBSOn + " 1-2", &onOBSSelect);
  addMenuItem("NAV1STBY", &onNavSelect);
  addMenuItem("ADF1 XP1", &onAdfXpndrSelect);
  addMenuItem("COM1STBY", &onComSelect);  
  addMenuItem("BRT  INV", &onBrightInvSelect);
 
  // rotaries
  setupRotary(&rotaries[0], 2, 3, 4);
  setupRotary(&rotaries[1], 5, 6, 7);
  setupRotary(&rotaries[2], 8, 9, 13);

  //Init LCD
  lc.shutdown(0,false);
  lc.setIntensity(0,1);
  for(int i = 0; i <= 7; i++){
    lc.setChar(0,i,8,true);
  }
  
  // Init OLED
  myOLED.begin(); 
  myOLED.clrScr();  
  myOLED.setFont(SmallFont);
  myOLED.setBrightness(brightness);
  displayOLEDtitle();
  myOLED.print("Loading", 0, yArr[0]);
  myOLED.update();
  
  // Init START PAGE
  delay(1000);
  lc.clearDisplay(0);
  scrollLED();
  pressAll3Buttons(); // Fix for bug of all 3 buttons pressed on init.
  updateOLED();

  //Open USB port
  //If you're using a Leonardo / Pro-micro this line can be left out.
  Serial.begin(115200);
  SerialUSB.begin(115200);
   //If you notice some instability gradually increase this value with increments of 5.
  SerialUSB.setTimeout(15); 
  Serial.setTimeout(15);
}

void loop() {
  connectorRX.dataHandling();
  redrawLED();
  // if (millis() - lastSample > debounce){ // debouncing before checking rotaries again
  //   lastSample = millis();
  //   loopRotaries();
  // }
}

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
  Serial.println(connectorTX.sendApMasterOn());
}

void onAPHdgToggle(){
  Serial.println(connectorTX.sendAPHeadingHold());
  if(!connectorRX.getAPHeadingLockOn()){
    ApHDGOn = " "; 
  } else {
    ApHDGOn = "|";
  }
  itemsMain[selItem].entry = "HDG" + ApHDGOn + ApGPSOn + "GPS";
  updateOLED();
}

void onHdgIncrease(){
  Serial.println(connectorTX.sendHeadingBugInc());
}

void onHdgDecrease(){
  Serial.println(connectorTX.sendHeadingBugDec());
}

void onBaroIncrease(){
  Serial.println(connectorTX.sendKohlsmanInc());
}

void onBaroDecrease(){
  Serial.println(connectorTX.sendKohlsmanDec());
}

void onAPAltToggle(){
  Serial.println(connectorTX.sendAPAltitudeHold());
  if(!connectorRX.getAPAltitudeLockOn()){
    ApALTOn = " "; 
  } else {
    ApALTOn = "|";
  }
  itemsMain[selItem].entry = "ALT" + ApALTOn + ApVSOn + "V/S";
  updateOLED();
}

void onAltIncrease(){
  Serial.println(connectorTX.sendAPAltitudeInc());
}

void onAltDecrease(){
  Serial.println(connectorTX.sendAPAltitudeDec());
}

void onAPVSToggle(){
  Serial.println(connectorTX.sendAPVSHold());
  if(!connectorRX.getAPVerticalHoldOn()){
    ApVSOn = " "; 
  } else {
    ApVSOn = "|";
  }
  itemsMain[selItem].entry = "ALT" + ApALTOn + ApVSOn + "V/S";
  updateOLED();
}

void onVsIncrease(){
  Serial.println(connectorTX.sendAPVSInc());
}

void onVsDecrease(){
  Serial.println(connectorTX.sendAPVSDec());
}

void onComSwitch(){
    if (!itemsMain[selItem].option){ // 1
      Serial.println(connectorTX.sendSwapCom1());
    } else { // 2
      Serial.println(connectorTX.sendSwapCom2());
    }  
}

void onComMhzIncrease(){
  if (!itemsMain[selItem].option){ // COM 1
    Serial.println(connectorTX.sendCom1WholeInc());
  } else { // COM 2
    Serial.println(connectorTX.sendCom2WholeInc());
  }  
}

void onComMhzDecrease(){
  if (!itemsMain[selItem].option){ // 1
    Serial.println(connectorTX.sendCom1WholeDec());
  } else { // 2
    Serial.println(connectorTX.sendCom2WholeDec());
  }
}

void onComKhzIncrease(){
  if (!itemsMain[selItem].option){ // COM 1
    Serial.println(connectorTX.sendCom1FractInc());
  } else { // COM 2
    Serial.println(connectorTX.sendCom2FractInc());
  }  
}

void onComKhzDecrease(){
  if (!itemsMain[selItem].option){ // 1
    Serial.println(connectorTX.sendCom1FractDecr());
  } else { // 2
    Serial.println(connectorTX.sendCom2FractDecr());
  }  
}

void onNavSwitch(){
  if (!itemsMain[selItem].option){ // 1
    Serial.println(connectorTX.sendSwapNav1());
  } else { // 2
    Serial.println(connectorTX.sendSwapNav2());
  }  
}

void onNavMhzIncrease(){
  if (!itemsMain[selItem].option){ // 1
    Serial.println(connectorTX.sendIncWholeNav1());
  } else { // 2
    Serial.println(connectorTX.sendIncWholeNav2());
  }  
}

void onNavMhzDecrease(){
  if (!itemsMain[selItem].option){ // 1
    Serial.println(connectorTX.sendDecWholeNav1());
  } else { // 2
    Serial.println(connectorTX.sendDecWholeNav2());
  }  
}

void onNavKhzIncrease(){
  if (!itemsMain[selItem].option){ // 1
    Serial.println(connectorTX.sendIncFractNav1());
  } else { // 2
    Serial.println(connectorTX.sendIncFractNav2());
  }  
}

void onNavKhzDecrease(){
  if (!itemsMain[selItem].option){ // 1
    Serial.println(connectorTX.sendDecFractNav1());
  } else { // 2
    Serial.println(connectorTX.sendDecFractNav2());
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
      Serial.println(connectorTX.sendAdf1Inc());
      break;
      case 1:
      Serial.println(connectorTX.sendAdf10Inc());
      break;
      case 2:
      Serial.println(connectorTX.sendAdf100Inc());
      break;
      default:
      break;
    }
  } else { // 2
    switch (adfDigit){
      case 0:
      Serial.println(connectorTX.sendAdf21Inc());
      break;
      case 1:
      Serial.println(connectorTX.sendAdf210Inc());
      break;
      case 2:
      Serial.println(connectorTX.sendAdf2100Inc());
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
      Serial.println(connectorTX.sendAdf1Dec());
      break;
      case 1:
      Serial.println(connectorTX.sendAdf10Dec());
      break;
      case 2:
      Serial.println(connectorTX.sendAdf100Dec());
      break;
      default:
      break;
    }
  } else { // 2
    switch (adfDigit){
      case 0:
      Serial.println(connectorTX.sendAdf21Dec());
      break;
      case 1:
      Serial.println(connectorTX.sendAdf210Dec());
      break;
      case 2:
      Serial.println(connectorTX.sendAdf2100Dec());
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
      Serial.println(connectorTX.sendXpndr1Inc());
      break;
      case 1:
      Serial.println(connectorTX.sendXpndr10Inc());
      break;
      case 2:
      Serial.println(connectorTX.sendXpndr100Inc());
      break;
      case 3:
      Serial.println(connectorTX.sendXpndr1000Inc());
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
      Serial.println(connectorTX.sendXpndr1Dec());
      break;
      case 1:
      Serial.println(connectorTX.sendXpndr10Dec());
      break;
      case 2:
      Serial.println(connectorTX.sendXpndr100Dec());
      break;
      case 3:
      Serial.println(connectorTX.sendXpndr1000Dec());
      break;
      default:
      break;
    }
  }  
}

void onOBS1Increase(){
  Serial.println(connectorTX.sendVor1ObiInc());
}

void onOBS1Decrease(){
  Serial.println(connectorTX.sendVor1ObiDec());
}

void onOBS2Increase(){
  Serial.println(connectorTX.sendVor2ObiInc());
}

void onOBS2Decrease(){
  Serial.println(connectorTX.sendVor2ObiDec());
}

void onAPGPSToggle(){
  Serial.println(connectorTX.sendToggleGPSDrivesNav1());
  getAPGpsLockOn = !getAPGpsLockOn;
  if(!getAPGpsLockOn){
    ApGPSOn = " "; 
  } else {
    ApGPSOn = "|";
  }
  itemsMain[selItem].entry = "HDG" + ApHDGOn + ApGPSOn + "GPS";
  updateOLED();
}

void onAPNAVToggle(){
  Serial.println(connectorTX.sendAPNav1Hold());
  if(!connectorRX.getAPNav1LockOn()){
    ApOBSOn = " "; 
  } else {
    ApOBSOn = "|";
  }
  itemsMain[selItem].entry = "OBS" + ApOBSOn + " 1-2";
  updateOLED();
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
  if(!isElvTrimBIG){
    Serial.println(connectorTX.sendElevTrimUp());
  } else {
    for (int i = 0; i < 20; i++){
      Serial.println(connectorTX.sendElevTrimUp());
      delay(10);
    }
  }
}

void onElvTrimDn(){
  if(!isElvTrimBIG){
    Serial.println(connectorTX.sendElevTrimDn());
  } else {
    for (int i = 0; i < 20; i++){
      Serial.println(connectorTX.sendElevTrimDn());
      delay(10);
    }
  }
}

void onRudTrimL(){
  if (!itemsMain[selItem].option){
    if(!isRudTrimBIG){
      Serial.println(connectorTX.sendRudderTrimLeft());
    } else {
      for (int i = 0; i < 20; i++){
        Serial.println(connectorTX.sendRudderTrimLeft());
        delay(10);
      }
    }
  } else {
    if(!isAilTrimBIG){
      Serial.println(connectorTX.sendAileronTrimLeft());
    } else {
      for (int i = 0; i < 20; i++){
        Serial.println(connectorTX.sendAileronTrimLeft());
        delay(10);
      }
    }
  }
}

void onRudTrimR(){
  if (!itemsMain[selItem].option){
    if(!isRudTrimBIG){
      Serial.println(connectorTX.sendRudderTrimRight());
    } else {
      for (int i = 0; i < 20; i++){
        Serial.println(connectorTX.sendRudderTrimRight());
        delay(10);
      }
    }
  } else {
    if(!isAilTrimBIG){
      Serial.println(connectorTX.sendAileronTrimRight());
    } else {
      for (int i = 0; i < 20; i++){
        Serial.println(connectorTX.sendAileronTrimRight());
        delay(10);
      }
    }
  }
}

void onElvTrimBIG(){
  isElvTrimBIG = !isElvTrimBIG;
}

void onRudTrimBIG(){
  if (!itemsMain[selItem].option){
    isRudTrimBIG = !isRudTrimBIG;
  } else {
    isAilTrimBIG = !isAilTrimBIG;
  }
}

void updateOLED(){
  displayOLEDMain();
}

void displayOLEDMain(){
  int x = firstItem+1 % mainMenuLength;
  int y = firstItem+2 % mainMenuLength;
  int z = firstItem+3 % mainMenuLength;
  myOLED.clrScr();
  displayOLEDtitle();
  invSelected(x, 0, iArr);
  invSelected(y, 1, iArr);
  invSelected(z, 2, iArr);        
  myOLED.update();
}

void displayOLEDtitle(){
  //myOLED.drawRoundRect(0,0,127,10);
  myOLED.drawLine(0,10,127,10);
  myOLED.setFont(SmallFont);
  myOLED.print("miniAVX", LEFT, 1);
  //myOLED.setFont(TinyFont);
  myOLED.print(version, RIGHT, 1);
  myOLED.setFont(BigFont);
}

void invSelected(int iItem, int itemArr, int cursorArr){
  // iItem: menu item
  // itemArr: index of item
  // cursorArr: index of cursor
  if (itemArr == cursorArr){
    myOLED.invertText(true);
    myOLED.print(itemsMain[iItem-1].entry, 0, yArr[itemArr]);
    myOLED.invertText(false);
  } else {
    myOLED.print(itemsMain[iItem-1].entry, 0, yArr[itemArr]);
  }
}

void loopRotaries() {
  Rotary *cur;

  for (int i = 0; i < ROTARIES; i++) {
    cur = &rotaries[i];    
    // first, check state
    long newPosition = cur->enc.read();
    if (newPosition != oldPosition && newPosition %  2 == 0) {
      oldPosition = newPosition;

      int currentStateClk = !digitalRead(cur->gpioClk);
      int currentStateDt = !digitalRead(cur->gpioDt);
      int currentStateSw = digitalRead(cur->button.gpioSw);

      if (currentStateDt != cur->lastStateDt) {
      // check for change
      cur->lastStateDt = currentStateDt;
      }
    
      if (currentStateClk != cur->lastStateClk) {
        // check for change
        if (currentStateClk == 1) { 
          if(cur->lastStateDt == 0) {
            // turned clockwise
            switch(i){
              case 0: //MAIN
              mainDown();
              break;
              case 1: //LEFT
              if(onLeftCW != NULL) onLeftCW();
              break;
              case 2: //RIGHT
              if(onRightCW != NULL) onRightCW();
              break;
              default:
              break;
            }
          } else {
            // turned counter-clockwise
            switch(i){
              case 0: //MAIN
              mainUp();
              break;
              case 1: //LEFT
              if(onLeftCCW != NULL) onLeftCCW();
              break;
              case 2: //RIGHT
              if(onRightCCW != NULL) onRightCCW();
              break;
              default:
              break;
            }            
          }
        }
      }
      cur->lastStateClk = currentStateClk;
    }

    if (currentStateSw != cur->button.lastStateSw && currentStateSw == HIGH) {
      switch(i){
        case 0: //MAIN
        if(onMainSelect != NULL) onMainSelect();
        break;
        case 1: //LEFT
        if(onLeftSelect != NULL) onLeftSelect();
        break;
        case 2: //RIGHT
        if(onRightSelect != NULL) onRightSelect();
        break;
        default:
        break;
      } 
    }
    cur->button.lastStateSw = currentStateSw;
  }
}

void drawLEDAltSpd(){
  int alt = 0;
  int spd = 0;
  if (!showBaro){
    if (!itemsMain[selItem].option){ // MSL    
      alt = connectorRX.getIndicatedAltitude();
    } else { // AGL
      alt = connectorRX.getFeetAboveGround();
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
  lc.setChar(0,0,gps%10,getAPGpsLockOn);    
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
  int alt = connectorRX.getApAltLock();
  int vs = connectorRX.getApVerticalSpeed();
  boolean altOn = connectorRX.getAPAltitudeLockOn();
  boolean vsOn = connectorRX.getAPVerticalHoldOn();
  if (alt < 10000){
      lc.setChar(0,7,(alt/1000)%10,false);
      lc.setChar(0,6,(alt/100)%10,false);
      lc.setChar(0,5,(alt/10)%10,false);
      lc.setChar(0,4,alt%10,altOn);
  } else {    
    lc.setChar(0,7,'F',false);
    lc.setChar(0,6,(alt/10000)%10,false);
    lc.setChar(0,5,(alt/1000)%10,false);
    lc.setChar(0,4,(alt/100)%10,altOn);
  }
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

void drawLEDTrim(){
  int e = connectorRX.getElevatorTrimPct();
  int r = 0;
  if (!itemsMain[selItem].option){
    r = connectorRX.getRudderTrimPct();
  } else {
    r = connectorRX.getAileronTrimPct();
  }
  if (e < 0){
    lc.setChar(0,7,'-', isElvTrimBIG);
  } else {
    lc.setChar(0,7,' ', isElvTrimBIG);
  }
  e = abs(e);
  lc.setChar(0,6,(e/100)%10, false);
  lc.setChar(0,5,(e/10)%10, false);
  lc.setChar(0,4,e%10, false);

  if (r < 0){
    if (!itemsMain[selItem].option){
      lc.setChar(0,3,'-', isRudTrimBIG);
    } else {
      lc.setChar(0,3,'-', isAilTrimBIG);
    }
  } else {
    if (!itemsMain[selItem].option){
      lc.setChar(0,3,' ', isRudTrimBIG);
    } else {
      lc.setChar(0,3,' ', isAilTrimBIG);
    }
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

void scrollLED(){
  if (selItem >= 0 && selItem < mainMenuLength && itemsMain[selItem].onSelect != NULL) itemsMain[selItem].onSelect();
}

void pressAll3Buttons(){
  if (selItem >= 0 && selItem < mainMenuLength && itemsMain[selItem].onSelect != NULL){
    if(onMainSelect != NULL) onMainSelect();
    if(onLeftSelect != NULL) onLeftSelect();
    if(onRightSelect != NULL) onRightSelect();
  }
}
