# miniAVX 1.05
MSFS Radio and AP panel with Arduino

Arduino code to be used with BitsAndDroids Connector and Library (v0.8.3.1).

Material List:

1x Arduino Due
3x Rotary encoder with pushbutton (KY-040 or similar), 1x OLED I2C Display 128x64 (monochrome), 1x 8-Digit seven segment display with MAX7219 IC, Cables, Housing

Remarks:

The programer port is used for TX and the native port for RX. Reason is that a RX on programer port resets the Arduino.

2 sessions of the connector need to be started, one for TX, one for RX. The TX session needs to be started first as it resets Arduino.

Libraries used:

BitsAndDroids Connector - https://www.bitsanddroids.com/

OLED_I2C - http://www.rinkydinkelectronics.com/library.php?id=79

LedControl - https://www.arduinolibraries.info/libraries/led-control
