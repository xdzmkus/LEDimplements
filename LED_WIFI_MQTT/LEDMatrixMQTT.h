#ifndef _LEDMATRIXMQTT_H_
#define _LEDMATRIXMQTT_H_

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

void performAction_callback(uint32_t x);

void setEffect_callback(char* data, uint16_t len);

void setRunningString_callback(char* data, uint16_t len);

#endif
