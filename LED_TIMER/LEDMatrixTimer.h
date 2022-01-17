#ifndef _LEDMATRIXTIMER_H_
#define _LEDMATRIXTIMER_H_

//#define SERIAL_DEBUG

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

void setup_LED();

void processLED();

void adjustBrightness(int8_t delta);

void changeEffect();

void turnOnLeds();

void turnOffLeds();

void holdNextEffect();

#endif
