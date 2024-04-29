#include <LedControl.h>
#include <BitsAndDroidsFlightConnector.h>
#include <LedControl.h>
#include <OLED_I2C.h>

#define ARDUINO_SAM_DUE

String MAJOR = "2";
String MINOR = "26";
String PATCH = "0";
String version = MAJOR + "." + MINOR + "." + PATCH;
String BADConnector = "0.3.3"; // this is the new RUST app (04/2024)
String BADLibrary = "1.6.7";
String BADVersion = BADConnector + "|" + BADLibrary;

//data type for button
struct Button {
  uint8_t gpioSw;
  uint8_t lastStateSw;
};

//data type for rotary
struct Rotary {
  int rotationCounter;
  uint8_t gpioClk;
  uint8_t gpioDt;
  volatile bool rotMoved;
  Button button;
};

#define ROTARIES 3
Rotary rotaries[ROTARIES];

// debouncing - used on buttons only (not rotaries)
unsigned long lastSample = 0;
int debounce = 5; // time in ms

//BAD - true for pro-micro / Leonardo
BitsAndDroidsFlightConnector connectorTX(&Serial);
BitsAndDroidsFlightConnector connectorRX(&SerialUSB);

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

// set where page are in list
int idxApHdgGps = 3;
int idxApAltVs = 2;
int idxApObs = 4;

// Init Cursor
int yArr[] = {16, 32, 48}; // 3 mneu items
int iArr = 0;
int page = 0;
String pageArr[] = {"ESS", "AP", "NAV", "MISC"};

// Init ADF
int adfDigit = 2;

// Init XPNDR
int xpndrDigit = 3;

//Init ALT page
boolean showBaro = false;
String pageALT = "MSL";

//Init SPD page
boolean showGS = false;
boolean showIAS = true;
String pageSPD = "IAS";

//Init CLK page
boolean showZ = false;
// Stopwatch (counting up)
long millisStartStw = 0;
long millisStw = 0;
int currStateStw = 0; // 0 = Reset, 1 = Start (running), 2 = Stop

// Timer (counting down)
long milTmrStart = 0; // start time of timer in millis
long milTmrSpan = 0;  // preset run time of timer in millis
long milTmrDisp = 0;  // time to show
int minsTmr = 0;
int secsTmr = 0;
int currStateTmr = 0; // 0 = Reset, 1 = Start (running), 2 = Stop

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

// Interrupt routines
void rotaryMain()
{
  rotaries[0].rotationCounter += checkRotaryEncoder(&rotaries[0]);
}
void rotaryLeft()
{
  rotaries[1].rotationCounter += checkRotaryEncoder(&rotaries[1]);
}
void rotaryRight()
{
  rotaries[2].rotationCounter += checkRotaryEncoder(&rotaries[2]);
}

// Rotary encoder has moved (interrupt tells us) but what happened?
// See https://www.pinteric.com/rotary.html
int8_t checkRotaryEncoder(Rotary *cur)
{
    // Reset the flag that brought us here (from ISR)
    cur->rotMoved = false;

    static uint8_t lrmem = 3;
    static int lrsum = 0;
    static int8_t TRANS[] = {0, -1, 1, 14, 1, 0, 14, -1, -1, 14, 0, 1, 14, 1, -1, 0};

    // Read BOTH pin states to deterimine validity of rotation (ie not just switch bounce)
    int8_t l = digitalRead(cur->gpioClk);
    int8_t r = digitalRead(cur->gpioDt);

    // Move previous value 2 bits to the left and add in our new values
    lrmem = ((lrmem & 0x03) << 2) + 2 * l + r;

    // Convert the bit pattern to a movement indicator (14 = impossible, ie switch bounce)
    lrsum += TRANS[lrmem];

    // encoder not in the neutral (detent) state
    if (lrsum % 4 != 0)
    {
        return 0;
    }
    // encoder in the neutral state - clockwise rotation
    if (lrsum == 4)
    {
        lrsum = 0;
        return 1;
    }
    // encoder in the neutral state - anti-clockwise rotation
    if (lrsum == -4)
    {
        lrsum = 0;
        return -1;
    }
    // An impossible rotation has been detected - ignore the movement
    lrsum = 0;
    return 0;
}

// define OLED
OLED myOLED(SDA, SCL);
extern uint8_t TinyFont[];
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
int OLEDbright = 17;
boolean inverted = false;
boolean showBADversion = false;

// define 7-segment display
#define REDRAW_INTERVAL 50
long lastRedraw = 0;
LedControl lc=LedControl(12,11,10,1);
int LCDbright = 1;

void setupRotary(Rotary *cur, uint8_t gpioClk, uint8_t gpioDt , uint8_t gpioSw) {
  cur->rotationCounter = 0;
  cur->gpioClk = gpioClk;
  cur->gpioDt = gpioDt;
  cur->rotMoved = false;
  cur->button.gpioSw = gpioSw;
  cur->button.lastStateSw = LOW;
  
  // The module already has pullup resistors on board
  pinMode(cur->gpioClk, INPUT);
  pinMode(cur->gpioDt, INPUT);

  // But not for the push switch
  pinMode(cur->button.gpioSw, INPUT_PULLUP);
}

void setup() {
  // menu - first item is 0
  // menu create by aviate - navigate - communicate
  addMenuItem("ELV  RUD", &onTrimSelect);
  addMenuItem(pageSPD + "  " + pageALT, &onAltSpeedSelect);
  addMenuItem("CLK  STW", &onClkStwSelect);
  addMenuItem("ALT  V/S", &onAltVsSelect);
  addMenuItem("HDG  -D>", &onHdgGPSSelect);
  addMenuItem("XPNDR   ", &onXpndrSelect);
  addMenuItem("NAV1 Sby", &onNavSelect);
  addMenuItem("OBS  1-2", &onOBSSelect);
  addMenuItem("ADF1 Sby", &onAdfSelect);
  addMenuItem("COM1 Sby", &onComSelect);
  addMenuItem("WND  OAT", &onWxSelect); 
  addMenuItem("BRT  INV", &onBrightInvSelect);

  // Init OLED
  myOLED.begin(); 
  myOLED.clrScr();  
  myOLED.setFont(SmallFont);
  myOLED.setBrightness(OLEDbright);
  displayOLEDtitle();
  myOLED.print("Loading", 0, yArr[0]);
  myOLED.update();

  //Init LCD
  delay(1000);
  lc.shutdown(0,false);
  lc.setIntensity(0,LCDbright);
  for(int i = 0; i <= 7; i++){
    lc.setChar(0,i,8,true);
  }

  // rotaries
  setupRotary(&rotaries[0], 2, 3, 4);
  attachInterrupt(digitalPinToInterrupt(2), rotaryMain, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), rotaryMain, CHANGE); 
  setupRotary(&rotaries[1], 5, 6, 7);
  attachInterrupt(digitalPinToInterrupt(5), rotaryLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(6), rotaryLeft, CHANGE); 
  setupRotary(&rotaries[2], 8, 9, 13);
  attachInterrupt(digitalPinToInterrupt(8), rotaryRight, CHANGE);
  attachInterrupt(digitalPinToInterrupt(9), rotaryRight, CHANGE); 
  
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
  loopRotaries();
}

void updateOLED(){
  displayOLEDMain();
}

void displayOLEDMain(){
  int x = firstItem+1 % mainMenuLength;
  int y = firstItem+2 % mainMenuLength;
  int z = firstItem+3 % mainMenuLength;
  int pageX = 0;
  int pageY = pageX + 31;
  myOLED.clrScr();
  displayOLEDtitle();
  switch (page){
    case 0:
    pageX = 0;
    break;
    case 1:
    pageX = 32;
    break;
    case 2:
    pageX = 64;
    break;
    case 3:
    pageX = 96;
    break;
  }
  pageY = pageX + 31;
  //myOLED.drawLine(pageX,10,pageY,10);
  myOLED.drawRect(pageX, 10, pageY, 13);
  myOLED.drawRect(pageX, 11, pageY, 12);
  invSelected(x, 0, iArr);
  invSelected(y, 1, iArr);
  invSelected(z, 2, iArr);      
  myOLED.update();
}

void displayOLEDtitle(){
  // myOLED.drawRoundRect(0,0,127,10);
  // myOLED.drawLine(0,14,127,14);
  myOLED.setFont(SmallFont);
  myOLED.print("miniAVX " + version, LEFT, 0);
  //myOLED.setFont(TinyFont);
  if (!showBADversion){
    myOLED.print(removePeriodAndZero(pageArr[page]), RIGHT, 0);
  } else {
    myOLED.print(removePeriodAndZero(BADVersion), RIGHT, 0);
  }
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
    int currentStateSw = digitalRead(cur->button.gpioSw);      
    // If valid movement, do something
    while(cur->rotationCounter != 0)
    {
      if(cur->rotationCounter > 0) {
        // turned clockwise
        cur->rotationCounter--;
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
        cur->rotationCounter++;
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
    delay(debounce); 
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

String removePeriodAndZero(String inputString) {
    // Remove all periods (.)
    inputString.replace(".", "");
    // Remove all zeros (0)
    inputString.replace("0", "");
    
    // Return the modified string
    return inputString;
}
