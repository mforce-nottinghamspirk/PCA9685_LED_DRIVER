# PCA9698
Arduino library for the GPIO Driver PCA9685 from NXP.

## Installation 
1. Navigate to the Releases page.
2. Download the latest release.
3. Rename to PCA9685_LED_DRIVER.zip
4. In the Arduino IDE, navigate to Sketch > Include Library > Add .zip library

## Functions
begin()\
setPWM()\
setPWM()\
getPWM()\
setPWMFreq()\
getPWMFreq()\
setOutputMode()\
update()\

## I2C Buffer problem
The Wire.h library uses a 32 byte buffer for some boards like the Arduino Nano. This library send data packages larger than 32 bytes to the PCA9685. 

1. Close the Arduino IDE if open.

2. Find the file Wire.h:

- Windows (IDE 2): C:\Users\{username}\AppData\Local\Arduino15\packages\arduino\hardware\avr\{version}\libraries\Wire\src\Wire.h
- Windows (IDE 1.x): C:\Program Files (x86)\Arduino\hardware\arduino\avr\libraries\Wire\src\Wire.h
- macOS: ~/Library/Arduino15/hardware/arduino/avr/libraries/Wire/src/Wire.h
- Linux: ~/sketchbook/hardware/arduino/avr/libraries/Wire/src/Wire.h

3. Open the file Wire.h with a text editor.

4. Locate the line #define BUFFER_LENGTH 32 and change the number 32 to the desired value(128).

5. Save the changes made and close the text editor.

6. Restart the IDE.

## Guide
First include the library and create a class object. When no arguments used the class will be use 0x40 as basic I2C adress. The **begin()** function initialize the I2C bus.
```c++
#include <PCA9685_LED_DRIVER.h>

PCA9685 gpio;

void setup()
{
    //Init the I2C bus and must be called in setup
    //PWM speed is optional. If called empty, default is 200Hz.
    gpio.begin();
    //or
    //gpio.begin(1524);
    //min is 24Hz and max is 1524Hz
}

void loop()
{

}
```
Or use some custom adress.
```c++
#include <PCA9685_LED_DRIVER.h>

#define addr  0x42  //I2C Adress of the PCA9685

PCA9685 gpio = (addr);  //Config the PCA9685 with adress 0x42

void setup()
{

}

void loop()
{

}
```
<br/>

To change the PWM duty cycle use **setPWM()**. The changes will be stored in the RAM. 
For changing outputs from RAM use **update()**. The changes will transmitted to the slave.
For reading the PWM value use **getPWM()**. This returns the value from the RAM.
```c++
#include <PCA9685_LED_DRIVER.h>

PCA9685 gpio;

uint8_t pwmValues[16] = {   0xFFF, 0x000, 0xFFF, 0x000, 0xFFF, 0x000, 0xFFF, 0x000, 
                            0xFFF, 0x000, 0xFFF, 0x000, 0xFFF, 0x000, 0xFFF, 0x000}; 

void setup()
{
    gpio.begin();   //Call to init I2C Bus
    gpio.setPWM(0, 1000);   //Set PWM for output 0 to 1000_ON to 3095_OFF. 4095/10Bit.
    //gpio.setPWM(pwmValues, sizeof(pwmValues));    //Use an uint16_t array for updating multiple PWM values with one function
    gpio.update();  //Send the PWM value from the RAM to the PCA9685 over I2C.
}

void loop()
{
    
}
```
<br/>