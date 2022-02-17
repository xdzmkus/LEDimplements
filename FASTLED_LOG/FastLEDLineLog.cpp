#include "FastLEDLineLog.h"

#define SERIAL_DEBUG
#include "SerialDebug.h"

FastLEDLineLog::FastLEDLineLog(LEDLine* ledLine, uint8_t startBrightness, uint16_t effectDuration)
	: FastLEDLine(ledLine, startBrightness, effectDuration)
{
}

void FastLEDLineLog::adjustBrightness(int8_t delta)
{
	FastLEDLine::adjustBrightness(delta);

	log_println(String(F("BRIGHTNESS = ")) + String(brightness));
}

void FastLEDLineLog::setBrightness(uint8_t newBrightness)
{
	FastLEDLine::setBrightness(newBrightness);

	log_println(String(F("BRIGHTNESS = ")) + String(brightness));
}

void FastLEDLineLog::turnOnLeds()
{
	FastLEDLine::turnOnLeds();

	log_println(String(F("TURN ON = ")) + String(ledLine->getState()));
}

void FastLEDLineLog::turnOffLeds()
{
	FastLEDLine::turnOffLeds();

	log_println(String(F("TURN OFF")));
}

void FastLEDLineLog::holdNextEffect()
{
	FastLEDLine::holdNextEffect();

	log_println(String(F("HOLD EFFECT = ")) + String(ledLine->getState()));
}

bool FastLEDLineLog::holdEffectByName(char* data)
{
	if (FastLEDLine::holdEffectByName(data))
	{
		log_println(String(F("HOLD EFFECT BY NAME = ")) + String(ledLine->getState()));

		return true;
	}

	log_println(String(F("WRONG EFFECT NAME = ")) + String(data));

	return false;
}

void FastLEDLineLog::nextEffect()
{
	FastLEDLine::nextEffect();

	log_println(String(F("NEXT EFFECT = ")) + String(ledLine->getState()));
}
