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
    onLeftSelect = NULL;
    onLeftCW = &onElvTrimUp;
    onLeftCCW = &onElvTrimDn;
    onRightSelect = NULL;
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