#include "FastLEDLine.h"

#include <FastLED.h>

FastLEDLine::FastLEDLine(LEDLine* ledLine, uint8_t startBrightness, uint16_t effectDuration)
	: ledLine(ledLine), brightness(startBrightness), effectsTicker(effectDuration * MillisTimer::CLOCKS_IN_SEC)
{
}

FastLEDLine::~FastLEDLine()
{
}

void FastLEDLine::setup()
{
	FastLED.setBrightness(brightness);
	FastLED.clear(true);
}

void FastLEDLine::process()
{
	if (effectsTicker.isReady())
	{
		nextEffect();
	}

	if (ledLine->refresh())
	{
		FastLED.show();
	}
}

void FastLEDLine::adjustBrightness(int8_t delta)
{
	brightness += delta;
	FastLED.setBrightness(brightness);
}

void FastLEDLine::setBrightness(uint8_t newBrightness)
{
	brightness = newBrightness;
	FastLED.setBrightness(brightness);
}

void FastLEDLine::turnOnLeds()
{
	effectsTicker.start();

	ledLine->setEffectByIdx(0);
	ledLine->turnOn();
}

void FastLEDLine::turnOffLeds()
{
	effectsTicker.stop();

	ledLine->turnOff();

	FastLED.clear(true);
}

void FastLEDLine::holdNextEffect()
{
	effectsTicker.stop();

	ledLine->setNextEffect();
	ledLine->turnOn();
}

bool FastLEDLine::holdEffectByName(char* data)
{
	if (ledLine->setEffectByName(data))
	{
		effectsTicker.stop();

		ledLine->turnOn();

		return true;
	}

	return false;
}

void FastLEDLine::nextEffect()
{
	ledLine->setNextEffect();
}
