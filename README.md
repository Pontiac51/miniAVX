# miniAVX 2.26.0 #

Mini Avionics Panel with Arduino for Microsoft Flight Simulator 2020 (MSFS)

***Functions:***
* Trim Elevator, Rudder & Aileron with display in %
* Display ALT (MSL, AGL), SPD (IAS (knts or Mach), GS) and QNH with adjustment knob
* Set and display HDG, OBS1 & 2
* Set and display autopilot values: ALT and V/S
* Set and display autopilot modes: Master, HDG, ALT, V/S, NAV, NAV/GPS switch
* Display direct course to next waypoint: -D>
* Complete Radio Stack: COM1 & 2, NAV1 & 2, ADF1 & 2, XPNDR1 (with mode & ident)
* Chronometer (Clock, Stopwatch and customizable Timer)
* Weather: Wind direction and speed, outside air temperature in C and F (in development)
* Adjust brightness and invert display

***Libraries used:***
* BitsAndDroids Connector - https://www.bitsanddroids.com/
* LedControl - https://www.arduinolibraries.info/libraries/led-control
* OLED_I2C - http://www.rinkydinkelectronics.com/library.php?id=79
* BigFont.c - Converted from RinkyDinkyElectronics, needs to go into OLED_I2C library folder

***Remarks:***
* The programer port is used for TX and the native port for RX. Reason is that a RX on programer port resets the Arduino.

***UPDATE 2.26.0 (BitsAndDroids NEW Rust Connector v0.3.3 & Library v1.6.7)***
* NEW: Transponder IDENT (setting the mode in development)
* NEW: Weather menu entry (wind and temperature in development)
* NEW: Pages with progressbar! The menu now has 12 items which made scrolling to it a bit confusing. Reaching the last item on a page brings up the next page. Page title is also displayed in top right corner. Current pages: Essentials (Trim, Speed, Altitude, Chrono), Autopilot (ALT, VS, HDG, GPS, Transponder), Navigation (NAV, OBS, ADF) and Misc. (COM, WX, SET).
* ADDED: Switch "Page title <-> BAD Version" on MAIN button on BRT INV page.
* REMOVED: Transponder 2 (has never been used in BAD or MSFS)

***UPDATE 2.25.0 (BitsAndDroids NEW Rust Connector 0.3.2 & Library v1.6.6)***
* FIXED: Brightness now changes OLED and LED in 15 steps.
* ADDED: STL files for 3D Printing
* ADDED: Github versioning

***UPDATE 2.21 (BitsAndDroids NEW Rust Connector 0.2.18 & Library v1.6.6)***
* FIXED: AP VS values are now displayed properly.

***UPDATE 2.20 (BitsAndDroids NEW Rust Connector 0.2.16 & Library v1.6.5)***
* NEW: Display of Transponder Mode (Off, Stby, On, Alt, Grnd, Test if applicable)
* NEW: Display of IAS in Mach (Turn left knob in IAS mode to switch)
* CHANGED: Shifted SPD page display on LED one to the left. Thx to fjbermejillo for the hint!
* CHANGED: ADFs now displayed with Hz digit
* FIXED: ADFs not displayed correctly if 4 digits.
* BUG: AP VS values are not displayed properly, setting works!

***UPDATE 2.17.163 (BitsAndDroids NEW Rust Connector 0.2.14 & Library v1.6.3)***
* NEW: Clock with Local and Zulu time, stopwatch and customizable timer
* BUG: ADFs not displayed correctly if 4 digits.

***UPDATE 2.02.178 (BitsAndDroids Library v1.6.0 & Connector 1.7.8)***
* CHANGED "GPS" to "-D>" as it now displays direct heading to next waypoint of flightplan (button press is still GPS-NAV switch)
* CHANGED If the aircraft is on the ground altitude AGL is displayed as 0
* FIXED Kohlsman HG reading incorrectly (29 instead of 29.92)

***UPDATE 2.01.160 (BitsAndDroids v1.6.0)***
* CHANGED If Autopilot V/S is OFF "----" will be displayed
* FIXED Display NAV Standby Frequencies not working
* FIXED Display Autopilot V/S Mode Display not working

***UPDATE 2.00.160 (BitsAndDroids v1.6.0)***
* NEW: Debounced buttons & handled rotary decoder via interrupts for better performance (especially on fast movements
* CHANGED Removed AP Modes from OLED (had snyc issues), still available in digit display
* BUG Display NAV Standby Frequencies not working (have to check)
* BUG Autopilot V/S Mode Display not working (have to check)
* BUG Auotpilot GPS Mode Display not working (not implemented)

***UPDATE 1.92.090 (BitsAndDroids v0.9.0)***
* NEW AP Master ON/OFF on main switch in pages "HDG GPS", "ALT V/S" and "OBS"
* NEW Active AP modes are now shown on "HDG GPS", "ALT V/S" and "OBS" pages with a vertical stripe ("|")
* NEW Rudder and aileron trim now feature coarse mode
* CHANGED Main button clockwise turn is now down in main menu, counter-clockwise is up
* CHANGED "NAV" and "COM" pages now display "STBY"
* CHANGED "ELE" to "ELV" on trim page
* CHANGED "OBS1-2" to "OBS  1-2" to allow to disply active AP NAV mode
* CHANGED order of menu items to fit with "Aviate-Navigate-Communicate"

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
