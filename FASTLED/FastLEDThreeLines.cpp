#include "FastLEDThreeLines.h"

#include <FastLED.h>

FastLEDThreeLines::FastLEDThreeLines(LEDLine* ledLineLeft, LEDLine* ledLineCenter, LEDLine* ledLineRight, uint8_t startBrightness, uint16_t effectDuration)
	: ledLineL(ledLineLeft), ledLineC(ledLineCenter), ledLineR(ledLineRight), brightness(startBrightness), effectsTicker(effectDuration * MillisTimer::CLOCKS_IN_SEC)
{
}

FastLEDThreeLines::~FastLEDThreeLines()
{
}

void FastLEDThreeLines::setup()
{
	FastLED.setBrightness(brightness);
	FastLED.clear(true);
}

void FastLEDThreeLines::process()
{
	if (effectsTicker.isReady())
	{
		nextEffect();
	}

	bool refresh = false;

	refresh |= ledLineL->refresh();
	refresh |= ledLineC->refresh();
	refresh |= ledLineR->refresh();

	if (refresh)
	{
		FastLED.show();
	}
}

void FastLEDThreeLines::adjustBrightness(int8_t delta)
{
	brightness += delta;
	FastLED.setBrightness(brightness);
}

void FastLEDThreeLines::setBrightness(uint8_t newBrightness)
{
	brightness = newBrightness;
	FastLED.setBrightness(brightness);
}

void FastLEDThreeLines::turnOnLeds()
{
	effectsTicker.start();

	ledLineL->setEffectByIdx(0);
	ledLineL->turnOn();

	ledLineC->setEffectByIdx(0);
	ledLineC->turnOn();

	ledLineR->setEffectByIdx(0);
	ledLineR->turnOn();
}

void FastLEDThreeLines::turnOffLeds()
{
	effectsTicker.stop();

	ledLineL->turnOff();
	ledLineC->turnOff();
	ledLineR->turnOff();

	FastLED.clear(true);
}

void FastLEDThreeLines::holdNextEffect()
{
	effectsTicker.stop();

	ledLineL->setNextEffect();
	ledLineL->turnOn();

	ledLineC->setNextEffect();
	ledLineC->turnOn();

	ledLineR->setNextEffect();
	ledLineR->turnOn();
}

bool FastLEDThreeLines::holdEffectByName(char* data)
{
	bool success = false;

	if (ledLineL->setEffectByName(data))
	{
		ledLineL->turnOn();

		success = true;
	}

	if (ledLineC->setEffectByName(data))
	{
		ledLineC->turnOn();

		success = true;
	}

	if (ledLineR->setEffectByName(data))
	{
		ledLineR->turnOn();

		success = true;
	}

	if(success)	effectsTicker.stop();

	return success;
}

void FastLEDThreeLines::nextEffect()
{
	ledLineL->setNextEffect();
	ledLineC->setNextEffect();
	ledLineR->setNextEffect();
}
