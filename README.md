# miniAVX 1.87

MSFS Radio and AP panel with Arduino

Arduino code to be used with BitsAndDroids Connector and Library.

***UPDATE 1.87 (BitsAndDroids v0.8.7)***
* NEW: TRIM percentages for Elevator, Aileron and Rudder
* NEW: Elevator trim can be set to coarse or fine setting, default is fine (turn slowly on coarse), press left button to switch
* Resorted menu: TRIM - ALT/SPD - Autopilot (HDG, GPS, ALT, VS, OBS) - Radio (COM, NAV, ADF, XPNDR)
* Moved baro setting to ALT - SPD page, press left button to show QNH
* Moved GPS course to HDG page
* Renamed ALTs and SPDs to proper abbreviations

***UPDATE 1.06 (BitsAndDroids v0.8.4)***
* OBS 1 & 2 display now working
* GPS course display
* NAV1 and GPS Autopilot ON/OFF (NAV1 mode press OBS1, GPS mode press GPS)
* ALT and SPD can be switched between radar/indicated and indicated/ground

***Material List:***
* 1x Arduino Due
* 3x Rotary encoder with pushbutton (KY-040 or similar)
* 1x OLED I2C Display 128x64 (monochrome)
* 1x 8-Digit seven segment display with MAX7219 IC
* Cables
* Housing

***Remarks:***
* The programer port is used for TX and the native port for RX. Reason is that a RX on programer port resets the Arduino.
* 2 sessions of the connector need to be started, one for TX, one for RX. The TX session needs to be started first as it resets Arduino.

***Libraries used:***
* BitsAndDroids Connector - https://www.bitsanddroids.com/
* OLED_I2C - http://www.rinkydinkelectronics.com/library.php?id=79
* LedControl - https://www.arduinolibraries.info/libraries/led-control
