// callbacks for all menu items (make sure unused are NULL) 
void onAltSpeedSelect() {
  drawLED = &drawLEDAltSpd;
  onMainSelect = &onSelectRadarAlt;
  onLeftSelect = &onSelectGroundSpeed;
  onLeftCW = &onSwitchKnotsMach;
  onLeftCCW = &onSwitchKnotsMach;
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

void onAdfSelect() {
    drawLED = &drawLEDAdf;
    onMainSelect = &onAdfSelect12;
    onLeftSelect = &onAdfNext;
    onLeftCW = &onAdfIncrease;
    onLeftCCW = &onAdfDecrease;
    onRightSelect = onAdfSwitch;
    onRightCW = &onAdfHzIncrease;
    onRightCCW = &onAdfHzDecrease;
}

void onXpndrSelect() {
    drawLED = &drawLEDXpndr;
    //onMainSelect = &onXpndrSelect12; // removed as transponder 2 not used by BAD or MSFS
    onMainSelect = NULL;
    onLeftSelect = &onXpndrNext;
    onLeftCW = &onXpndrIncrease;
    onLeftCCW = &onXpndrDecrease;
    onRightSelect = &onXpndrIdent;
    onRightCW = &onXpndrModeIncrease;
    onRightCCW = &onXpndrModeDecrease;
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
    onMainSelect = &onShowBADVersion;   
    onLeftSelect = NULL; 
    onLeftCW = &onBrightIncrease;
    onLeftCCW = &onBrightDecrease;
    onRightSelect = &onInvSwitch;    
    onRightCW = NULL;
    onRightCCW = NULL;
}

void onClkStwSelect() {
    drawLED = &drawLEDClk;
    onMainSelect = &onTmrSwitch;    
    onLeftSelect = &onClkToggle;
    onLeftCW = NULL;
    onLeftCCW = NULL;
    onRightSelect = &onTmrToggle;
    onRightCW = &onTmrInc;
    onRightCCW = &onTmrDec;
}

void onWxSelect() {
  drawLED = &drawLEDWx;
  onMainSelect = &onOATSwitch;    
  onLeftSelect = &onWNDPerpToggle;
  onLeftCW = NULL;
  onLeftCCW = NULL;
  onRightSelect = &onOATToggle;
  onRightCW = NULL;
  onRightCCW = NULL;
}