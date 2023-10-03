/*  
    Arduino Library for the PCA9685 16-Channel 12-Bit PWM GPIO Module.
    
    MIT License
    Copyright (c) 2022 Justin-Pl
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifndef PCA9685_LED_DRIVER_H
#define PCA9685_LED_DRIVER_H

/* Libs */
#include <Arduino.h>
#include <Wire.h>

/* Registers */
#define PCA9685_MODE1           0x00
#define PCA9685_MODE2           0x01
#define PCA9685_SUBADR1         0x02
#define PCA9685_SUBADR2         0x03
#define PCA9685_SUBADR3         0x04
#define PCA9685_ALLCALLADR      0x05
#define PCA9685_LED0_ON_L       0x06
#define PCA9685_LED0_ON_H       0x07
#define PCA9685_LED0_OFF_L      0x08
#define PCA9685_LED0_OFF_H      0x09
#define PCA9685_LED1_ON_L       0x0A
#define PCA9685_LED1_ON_H       0x0B
#define PCA9685_LED1_OFF_L      0x0C
#define PCA9685_LED1_OFF_H      0x0D
#define PCA9685_LED2_ON_L       0x0E
#define PCA9685_LED2_ON_H       0x0F
#define PCA9685_LED2_OFF_L      0x10
#define PCA9685_LED2_OFF_H      0x11
#define PCA9685_LED3_ON_L       0x12
#define PCA9685_LED3_ON_H       0x13
#define PCA9685_LED3_OFF_L      0x14
#define PCA9685_LED3_OFF_H      0x15
#define PCA9685_LED4_ON_L       0x16
#define PCA9685_LED4_ON_H       0x17
#define PCA9685_LED4_OFF_L      0x18
#define PCA9685_LED4_OFF_H      0x19
#define PCA9685_LED5_ON_L       0x1A
#define PCA9685_LED5_ON_H       0x1B
#define PCA9685_LED5_OFF_L      0x1C
#define PCA9685_LED5_OFF_H      0x1D
#define PCA9685_LED6_ON_L       0x1E
#define PCA9685_LED6_ON_H       0x1F
#define PCA9685_LED6_OFF_L      0x20
#define PCA9685_LED6_OFF_H      0x21
#define PCA9685_LED7_ON_L       0x22
#define PCA9685_LED7_ON_H       0x23
#define PCA9685_LED7_OFF_L      0x24
#define PCA9685_LED7_OFF_H      0x25
#define PCA9685_LED8_ON_L       0x26
#define PCA9685_LED8_ON_H       0x27
#define PCA9685_LED8_OFF_L      0x28
#define PCA9685_LED8_OFF_H      0x29
#define PCA9685_LED9_ON_L       0x2A
#define PCA9685_LED9_ON_H       0x2B
#define PCA9685_LED9_OFF_L      0x2C
#define PCA9685_LED9_OFF_H      0x2D
#define PCA9685_LED10_ON_L      0x2E
#define PCA9685_LED10_ON_H      0x2F
#define PCA9685_LED10_OFF_L     0x30
#define PCA9685_LED10_OFF_H     0x31
#define PCA9685_LED11_ON_L      0x32
#define PCA9685_LED11_ON_H      0x33
#define PCA9685_LED11_OFF_L     0x34
#define PCA9685_LED11_OFF_H     0x35
#define PCA9685_LED12_ON_L      0x36
#define PCA9685_LED12_ON_H      0x37
#define PCA9685_LED12_OFF_L     0x38
#define PCA9685_LED12_OFF_H     0x39
#define PCA9685_LED13_ON_L      0x3A
#define PCA9685_LED13_ON_H      0x3B
#define PCA9685_LED13_OFF_L     0x3C
#define PCA9685_LED13_OFF_H     0x3D
#define PCA9685_LED14_ON_L      0x3E
#define PCA9685_LED14_ON_H      0x3F
#define PCA9685_LED14_OFF_L     0x40
#define PCA9685_LED14_OFF_H     0x41
#define PCA9685_LED15_ON_L      0x42
#define PCA9685_LED15_ON_H      0x43
#define PCA9685_LED15_OFF_L     0x44
#define PCA9685_LED15_OFF_H     0x45
#define PCA9685_ALL_LED_ON_L    0xFA
#define PCA9685_ALL_LED_ON_H    0xFB
#define PCA9685_ALL_LED_OFF_L   0xFC
#define PCA9685_ALL_LED_OFF_H   0xFD
#define PCA9685_PRE_SCALE       0xFE
#define PCA9685_TESTMODE        0xFF
#define PCA9685_MODE1_SLEEP     0x10
#define PCA9685_MODE1_AI        0x20
#define PCA9685_MODE1_ALLCALL   0x01
#define PCA9685_MODE1_RESTART   0x80
#define PCA9685_MODE2_INVRT     0x10
#define PCA9685_MODE2_OUTDRV    0x04
#define PCA9685_DEFAULT_FREQ    200
#define PCA9685_MAX_FREQ        1526
#define PCA9685_BASE_ADDRESS    0x40

/* Defines */
#if !defined I2C_STANDARD_MODE
    #define I2C_STANDARD_MODE       100000UL
#endif
#if !defined I2C_FAST_MODE 
    #define I2C_FAST_MODE           400000UL
#endif
#if !defined I2C_FAST_MODE_PLUS 
    #define I2C_FAST_MODE_PLUS      1000000UL
#endif
#if !defined OSC_CLOCK 
    #define OSC_CLOCK               25000000UL
#endif

/* Class */
class PCA9685
{
private:
    TwoWire *_wire;
    uint8_t _adress;
    uint8_t _ledRegisters[64];
    void writeI2C(uint8_t command, uint8_t data);
    void writeI2C(uint8_t command, uint8_t *data, uint8_t length);
    uint8_t readI2C(uint8_t command);
    void readI2C(uint8_t command, uint8_t *data, uint8_t length);
public:
    PCA9685(uint8_t adress = PCA9685_BASE_ADDRESS, TwoWire &wire = Wire);
    void begin(uint16_t freq = PCA9685_DEFAULT_FREQ);
    void setPWM(uint8_t num, uint16_t pwm);
    void setPWM(uint16_t *data, uint8_t size);
    uint16_t getPWM(uint8_t num);
    void setPWMFreq(uint16_t freq);
    uint16_t getPWMFreq();
    void setOutputMode(uint8_t mode);
    void setOutputInvert(uint8_t mode);
    void update();
};

#endif PCA9685_LED_DRIVER_H
