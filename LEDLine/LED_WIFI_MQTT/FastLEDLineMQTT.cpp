#include "FastLEDLineMQTT.h"

FastLEDLineMQTT::FastLEDLineMQTT(WifiMQTT* wifiMqtt, LEDLine* ledLine, uint8_t startBrightness, uint16_t effectDuration)
	: FastLEDLine(ledLine, startBrightness, effectDuration), wifiMqtt(wifiMqtt)
{
}

void FastLEDLineMQTT::adjustBrightness(int8_t delta)
{
	FastLEDLine::adjustBrightness(delta);

	wifiMqtt->log(LOG_LEVEL::INFO, String(F("BRIGHTNESS = ")) + String(brightness));
}

void FastLEDLineMQTT::setBrightness(uint8_t newBrightness)
{
	FastLEDLine::setBrightness(newBrightness);

	wifiMqtt->log(LOG_LEVEL::INFO, String(F("BRIGHTNESS = ")) + String(brightness));
}

void FastLEDLineMQTT::turnOnLeds()
{
	FastLEDLine::turnOnLeds();

	wifiMqtt->log(LOG_LEVEL::INFO, String(F("TURN ON = ")) + String(ledLine->getState()));
}

void FastLEDLineMQTT::turnOffLeds()
{
	FastLEDLine::turnOffLeds();

	wifiMqtt->log(LOG_LEVEL::INFO, String(F("TURN OFF")));
}

void FastLEDLineMQTT::holdNextEffect()
{
	FastLEDLine::holdNextEffect();

	wifiMqtt->log(LOG_LEVEL::INFO, String(F("HOLD EFFECT = ")) + String(ledLine->getState()));
}

bool FastLEDLineMQTT::holdEffectByName(char* data)
{
	if (FastLEDLine::holdEffectByName(data))
	{
		wifiMqtt->log(LOG_LEVEL::INFO, String(F("HOLD EFFECT = ")) + String(ledLine->getState()));

		return true;
	}

	wifiMqtt->log(LOG_LEVEL::INFO, String(F("WRONG EFFECT NAME = ")) + String(data));

	return false;

}

void FastLEDLineMQTT::nextEffect()
{
	FastLEDLine::nextEffect();

	wifiMqtt->log(LOG_LEVEL::INFO, String(F("EFFECT = ")) + String(ledLine->getState()));
}
