/* Header */
#include "PCA9685_LED_DRIVER.h"

/* Class Functions */
/* private */
void PCA9685::writeI2C(uint8_t command, uint8_t data)
{
    _wire->beginTransmission(_adress);
    _wire->write(command);
    _wire->write(data);
    _wire->endTransmission();
}

void PCA9685::writeI2C(uint8_t command, uint8_t *data, uint8_t length)
{
    _wire->beginTransmission(_adress);
    _wire->write(command);
    _wire->write(data, length);
    _wire->endTransmission();
}

uint8_t PCA9685::readI2C(uint8_t command)
{
    _wire->beginTransmission(_adress);
    _wire->write(command);
    _wire->endTransmission();
    _wire->requestFrom((uint8_t)_adress, (uint8_t)1);
    return _wire->read();
}

void PCA9685::readI2C(uint8_t command, uint8_t *data, uint8_t length)
{
    _wire->beginTransmission(_adress);
    _wire->write(command);
    _wire->endTransmission();
    _wire->requestFrom((uint8_t)_adress, (uint8_t)length);
    _wire->readBytes(data, length);
}

/* public */
/*!
    @brief  Constructor for PCA9685 IO-Expander.
    @param  adress   
            I2C Adress for PCA9685.
    @param  TwoWire
            Used I2C bus object. Optional. Wire is standard
    @return PCA9685 obejct.
*/
PCA9685::PCA9685(uint8_t adress, TwoWire &wirePort)
{
    _wire = &wirePort;
    _adress = adress;
}

/*!
    @brief  Init PCA9685
    @return None (void).
*/
void PCA9685::begin(uint16_t freq)
{
    uint8_t mode1Reg = readI2C(PCA9685_MODE1);
    if (freq < 24 || freq > 1526) freq = PCA9685_DEFAULT_FREQ; 
    uint8_t prescaleVal = OSC_CLOCK / ((uint32_t)4096 * freq) - 1;
    _wire->begin();
    writeI2C(PCA9685_ALL_LED_OFF_H, 0x80);
    writeI2C(PCA9685_MODE1, mode1Reg | PCA9685_MODE1_SLEEP);
    writeI2C(PCA9685_PRE_SCALE, prescaleVal);
    writeI2C(PCA9685_MODE1, mode1Reg & ~PCA9685_MODE1_SLEEP);
    delay(1);
}

/*!
    @brief  Set PWM for one channel.
    @param  num   
            Pin number. 0-15.
    @param  pwm
            PWM ratio. 0-4095.
    @return None (void).
*/
void PCA9685::setPWM(uint8_t num, uint16_t pwm)
{
    uint8_t baseIndex = num * 4;
    if (num > 0xF) return;
    if (pwm > 0xFFF) return;
    _ledRegisters[baseIndex] = 0x00;
    _ledRegisters[baseIndex + 1] = 0x00;
    _ledRegisters[baseIndex + 2] = pwm & 0xFF;
    _ledRegisters[baseIndex + 3] = (pwm >> 8) & 0x0F;
}

/*!
    @brief  Set PWM for multiple channel from array.
    @param  data   
            Data array.
    @param  size
            Size of the array in bytes. Just use sizeOf() makro. 
    @return None (void).
*/
void PCA9685::setPWM(uint16_t *data, uint8_t size)
{
    if (size > 32) size = 32;
    size = size / 2;
    for (uint8_t index = 0; index < size; index++) setPWM(index, data[index]);
}

/*!
    @brief  Get PWM value from RAM for one channel.
    @param  num   
            Pin number. 0-15.
    @return PWM value from RAM.
*/
uint16_t PCA9685::getPWM(uint8_t num)
{
    uint8_t baseIndex = num * 4;
    uint16_t pwm = _ledRegisters[baseIndex + 3];
    if (num > 0xF) return 0;
    pwm = pwm << 8;
    pwm += _ledRegisters[baseIndex + 2];
    return pwm;
}

/*!
    @brief  Set PWM frequency.
    @param  freq
            Frequency for PWM. From 24Hz - 1526Hz.
    @return None (void).
*/
void PCA9685::setPWMFreq(uint16_t freq)
{
    if (freq < 24 || freq > 1526) return;
    uint8_t prescaleVal = OSC_CLOCK / ((uint32_t)4096 * freq) - 1;
    uint8_t mode1Reg = readI2C(PCA9685_MODE1);
    writeI2C(PCA9685_ALL_LED_OFF_H, 0x80);
    writeI2C(PCA9685_MODE1, mode1Reg | PCA9685_MODE1_SLEEP);
    writeI2C(PCA9685_PRE_SCALE, prescaleVal);
    writeI2C(PCA9685_MODE1, mode1Reg & ~PCA9685_MODE1_SLEEP);
    delay(1);
}

/*!
    @brief  Get PWM frequency.
    @return Frequency from the chip.
*/
uint16_t PCA9685::getPWMFreq()
{
    uint8_t prescaleReg = readI2C(PCA9685_PRE_SCALE);
    uint16_t freq = OSC_CLOCK / ((uint32_t)4096 * (prescaleReg + 1));
    return freq;
}

/*!
    @brief  Set output mode. Open drain or totem pole
    @param  mode
            0 for totem pole (default) and 1 for open drain
    @return None (void).
*/
void PCA9685::setOutputMode(uint8_t mode)
{
    uint8_t mode2Reg = readI2C(PCA9685_MODE2);
    if (mode)
    {
        writeI2C(PCA9685_MODE2, mode2Reg & ~PCA9685_MODE2_OUTDRV);
        return;
    }
    writeI2C(PCA9685_MODE2, mode2Reg | PCA9685_MODE2_OUTDRV);
}

/*!
    @brief  Set output invert. normal or inverted
    @param  mode
            0 for active high (default) and 1 for active low
    @return None (void).
*/
void PCA9685::setOutputInvert(uint8_t mode)
{
    uint8_t mode2Reg = readI2C(PCA9685_MODE2);
    if (mode)
    {
		writeI2C(PCA9685_MODE2, mode2Reg | PCA9685_MODE2_INVRT);
        return;
    writeI2C(PCA9685_MODE2, mode2Reg & ~PCA9685_MODE2_INVRT);
    }
}
/*!
    @brief  Update all 16 PWM Channels.
    @return None (void).
*/
void PCA9685::update()
{
    writeI2C(PCA9685_MODE1, PCA9685_MODE1_AI);
    writeI2C(PCA9685_LED0_ON_L, _ledRegisters, sizeof(_ledRegisters));
}
