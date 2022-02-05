#ifndef _FASTLEDLINE_H_
#define _FASTLEDLINE_H_

#include <Arduino.h>

#include <LEDLine.h>

class FastLEDLine
{
protected:

	LEDLine* ledLine;

	uint8_t brightness;

	MillisTimer effectsTicker;
	
public:

	FastLEDLine(LEDLine* ledLine, uint8_t startBrightness = 255, uint16_t effectDuration = 60);

	virtual ~FastLEDLine();

	virtual void setup();

	virtual void process();

	virtual void adjustBrightness(int8_t delta);

	virtual void setBrightness(uint8_t newBrightness);

	virtual void turnOnLeds();

	virtual void turnOffLeds();

	virtual void holdNextEffect();

	virtual bool holdEffectByName(char* data);

protected:

	virtual void nextEffect();
};

#endif
