// LEDLine256.h

#ifndef _LEDLINE_256_H_
#define _LEDLINE_256_H_

#include <DynamicLEDLine.hpp>

template <CRGB* const leds, const uint16_t numLeds>
class LEDLine256 : public DynamicLEDLine<leds, numLeds>
{
public:

	virtual bool setEffectByName(const char* effectName) override
	{
		if (strcmp(BouncingBallsLedEffect<leds, numLeds>::name, effectName) == 0)
		{
			delete DynamicLEDLine<leds, numLeds>::effect; DynamicLEDLine<leds, numLeds>::effect = new BouncingBallsLedEffect<leds, numLeds>(50, random(1, 10));
		}
		else if (strcmp(BugsLedEffect<leds, numLeds>::name, effectName) == 0)
		{
			delete DynamicLEDLine<leds, numLeds>::effect; DynamicLEDLine<leds, numLeds>::effect = new BugsLedEffect<leds, numLeds>(20, random(3, 40));
		}
		else
		{
			return DynamicLEDLine<leds, numLeds>::setEffectByName(effectName);
		}

		if (DynamicLEDLine<leds, numLeds>::effect != nullptr) DynamicLEDLine<leds, numLeds>::effect->start();

		return true;
	};
};


#endif
