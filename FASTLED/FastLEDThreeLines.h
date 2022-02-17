#ifndef _FASTLEDTHREELINES_H_
#define _FASTLEDTHREELINES_H_

#include <Arduino.h>

#include <LEDLine.h>

class FastLEDThreeLines
{
protected:

	LEDLine* ledLineL;
	LEDLine* ledLineC;
	LEDLine* ledLineR;

	uint8_t brightness;
	MillisTimer effectsTicker;
	
public:

	FastLEDThreeLines(LEDLine* ledLineLeft, LEDLine* ledLineCenter, LEDLine* ledLineRight, uint8_t startBrightness = 255, uint16_t effectDuration = 60);

	virtual ~FastLEDThreeLines();

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
