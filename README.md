# miniAVX 1.92.090 #

Mini Avionics Panel with Arduino for Microsoft Flight Simulator 2020 (MSFS)

Arduino code to be used with BitsAndDroids Connector and Library.

***UPDATE 1.92.090 (BitsAndDroids v0.9.0)***
* NEW On "HDG GPS" page main button turns ON/OFF AP Master
* NEW Active AP modes are now shown on "HDG GPS" and "ALT V/S" pages with a vertical stripe ("|")
* NEW Rudder and aileron trim now feature coarse mode
* CHANGED Main button clockwise turn is now down in main menu, counter-clockwise is up
* CHANGED "NAV" and "COM" pages now display "STBY"
* CHANGED "ELE" to "ELV" on trim page

***UPDATE 1.91.088 (BitsAndDroids v0.8.8)***
* CHANGED COM, NAV split to seperate draw functions on the digit display (different frequency formats)
* CHANGED Redraw interval from 100ms to 50ms
* FIXED Renamed "VS" to "V/S"
* FIXED ADF, XPDNR 

***UPDATE 1.90.087 (BitsAndDroids v0.8.7)***
* NEW BIG font for menu items (BigFont.c; ADD to OLED_I2C library)
* NEW Selected item now shown inverted, Cursor ">" removed
* NEW Version number now shows BAD version as well
* CHANGED Altitude can now be switched between MSL and AGL with main button independently
* CHANGED Speed can now be switched between IAS and GS with left button independently
* CHANGED QNH can now be switched ON/OFF with right button on SPD-ALT page
* CHANGED Redesign of UI for better readability

***UPDATE 1.87 (BitsAndDroids v0.8.7)***
* NEW TRIM percentages for elevator, aileron and rudder, main button switches elevator and aileron
* NEW Elevator trim has coarse/fine setting (turn slowly on coarse); left button to switch (. means coarse)
* NEW Autopilot Master ON is displayed with an "A" on HDG page (no function to switch OFF/ON, display only)
* NEW VS -1000 and above display a thousands seperator, e.g. VS at -1200 feet would read as "-1.20"
* NEW All autopilot modes display a dot after the last digit, e.g. HDG "350." means HDG autopilot is active
* CHANGED Menu order: TRIM - ALT/SPD - Autopilot (HDG, GPS, ALT, VS, OBS) - Radio (COM, NAV, ADF, XPNDR)
* CHANGED Moved baro setting to ALT - SPD page, press left button to show QNH
* CHANGED Moved GPS course to HDG page
* FIXED Renamed ALTs and SPDs to proper abbreviations

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
* LedControl - https://www.arduinolibraries.info/libraries/led-control
* OLED_I2C - http://www.rinkydinkelectronics.com/library.php?id=79
* BigFont.c - Converted from RinkyDinkyElectronics, needs to go into OLED_I2C library folder
