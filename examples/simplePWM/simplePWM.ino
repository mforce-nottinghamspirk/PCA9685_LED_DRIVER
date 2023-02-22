/* Libs */
#include <PCA9685_LED_DRIVER.h>

/* Defines */
#define ADDRESS     0x40
#define FREQUENCY   400     //min 24Hz, max 1524Hz

/* Class Constructor */
PCA9685 gpio = (ADDRESS);

/* Variables */ 
uint16_t pwmVal[16];

void setup()
{
    gpio.begin(FREQUENCY);
}

void loop()
{
    gpio.setPWM(pwmVal, sizeof(pwmVal));
    gpio.update();
    for (uint8_t channel = 0; channel < 16; channel++)
    {
        pwmVal[channel] = pwmVal[channel] + 1;
        if (pwmVal[channel] > 0xFFF) pwmVal[channel] = 0x000;
    }
    delay(1);
}
