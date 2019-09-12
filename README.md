# Arduino-I2C-KM1
KeiganMotor KM-1 control library using I2C communication

# Documents web site
 (https://document.keigan-motor.com)

## Description
This library allows an Arduino/Genuino board to control KeiganMotor KM-1 using I2C communication.
ESP32 or ESP8266 (by Espressif) is also available.

## Physical Connection
Please refer to the following page.
- Japanese: https://document.keigan-motor.com/software_dev/ports_on_wire

***NOTE***
Please refer to the following about pull-up resister.

### Arduino/Genuino
You don't add pullup resisters between SDA and Vdd and between SDL and Vdd because Arduino Library "Wire" enables internal pullup automatically. If control is unstable, please add external pullup resisters from 1kOhm to 10kOhm and add the following lines in setup() function to disable internal pullup resisters..
```arduino
pinMode(SDA, INPUT); // For Arduino Uno, SDA = 4
pinMode(SCL, INPUT);  // For Arduino Uno, SCL = 5
```

### ESP32 or ESP8266
Please add external pullup resisters from 1kOhm to 10kOhm as follows. (INPUT_PULLUP is not recommended because of stability.)
- between SDA and 5V(or 3.3V) // Default SDA = 21
- between SCL and 5V(or 3.3V) // Default SCL = 22

### M5 Stack
M5 Stack do not need external pullup resisters because it has pullup resisters.

## Basic
Including the library and initialization are required to control KeiganMotor.
### Include library
```arduino
#include "KM1_I2C.h"

```
### Initialize with I2C slave address
#### The default I2C address of KM-1 is "0xA0"
```arduino
KeiganMotor motor(0xA0);
```
### Start I2C communication
```arduino
motor.begin();
```
***NOTE***
This line is not needed from version 1.1.0 because I2C communication is available when initializing KeiganMotor.

## Examples
### See "examples" folder
### Run
```arduino
motor.enable();
motor.speedRpm(10);
motor.runForward();
delay(5000);
motor.runReverse();
delay(5000);
motor.stop();
```

### Change I2C Address
```arduino
motor.i2cSlaveAddress(0xB0);
motor.saveAllRegisters();
delay(2000);
motor.reboot();
```

## Requirement

- Arduino UNO / Mega / Mega 2560
- KeiganMotor KM-1 series

## Installation

    $ git clone https://github.com/keigan-motor/Arduino-I2C-KM1

## Zip

    https://github.com/keigan-motor/Arduino-I2C-KM1/archive/master.zip

## Author

[@tkeigan](https://twitter.com/tkeigan)
Keigan Inc.

## License

[MIT](http://b4b4r07.mit-license.org)
