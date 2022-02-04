#include "FastLEDThreeLinesMQTT.h"

FastLEDThreeLinesMQTT::FastLEDThreeLinesMQTT(WifiMQTT* wifiMqtt, LEDLine* ledLineLeft, LEDLine* ledLineCenter, LEDLine* ledLineRight, uint8_t startBrightness, uint16_t effectDuration)
	: FastLEDThreeLines(ledLineLeft, ledLineCenter, ledLineRight, startBrightness, effectDuration), wifiMqtt(wifiMqtt)
{
}

void FastLEDThreeLinesMQTT::adjustBrightness(int8_t delta)
{
	FastLEDThreeLines::adjustBrightness(delta);

	wifiMqtt->log(LOG_LEVEL::INFO, String(F("BRIGHTNESS = ")) + String(brightness));
}

void FastLEDThreeLinesMQTT::setBrightness(uint8_t newBrightness)
{
	FastLEDThreeLines::setBrightness(newBrightness);

	wifiMqtt->log(LOG_LEVEL::INFO, String(F("BRIGHTNESS = ")) + String(brightness));
}

void FastLEDThreeLinesMQTT::turnOnLeds()
{
	FastLEDThreeLines::turnOnLeds();

	wifiMqtt->log(LOG_LEVEL::INFO, String(F("TURN ON LEFT = ")) + String(ledLineL->getState()));
	wifiMqtt->log(LOG_LEVEL::INFO, String(F("TURN ON CENTER = ")) + String(ledLineC->getState()));
	wifiMqtt->log(LOG_LEVEL::INFO, String(F("TURN ON RIGHT = ")) + String(ledLineR->getState()));
}

void FastLEDThreeLinesMQTT::turnOffLeds()
{
	FastLEDThreeLines::turnOffLeds();

	wifiMqtt->log(LOG_LEVEL::INFO, String(F("TURN OFF")));
}

void FastLEDThreeLinesMQTT::holdNextEffect()
{
	FastLEDThreeLines::holdNextEffect();

	wifiMqtt->log(LOG_LEVEL::INFO, String(F("HOLD EFFECT LEFT = ")) + String(ledLineL->getState()));
	wifiMqtt->log(LOG_LEVEL::INFO, String(F("HOLD EFFECT CENTER = ")) + String(ledLineC->getState()));
	wifiMqtt->log(LOG_LEVEL::INFO, String(F("HOLD EFFECT RIGHT = ")) + String(ledLineR->getState()));
}

bool FastLEDThreeLinesMQTT::holdEffectByName(char* data)
{
	if (FastLEDThreeLines::holdEffectByName(data))
	{
		wifiMqtt->log(LOG_LEVEL::INFO, String(F("HOLD EFFECT LEFT = ")) + String(ledLineL->getState()));
		wifiMqtt->log(LOG_LEVEL::INFO, String(F("HOLD EFFECT CENTER = ")) + String(ledLineC->getState()));
		wifiMqtt->log(LOG_LEVEL::INFO, String(F("HOLD EFFECT RIGHT = ")) + String(ledLineR->getState()));

		return true;
	}

	wifiMqtt->log(LOG_LEVEL::INFO, String(F("WRONG EFFECT NAME = ")) + String(data));

	return false;

}

void FastLEDThreeLinesMQTT::nextEffect()
{
	FastLEDThreeLines::nextEffect();

	wifiMqtt->log(LOG_LEVEL::INFO, String(F("EFFECT LEFT = ")) + String(ledLineL->getState()));
	wifiMqtt->log(LOG_LEVEL::INFO, String(F("EFFECT CENTER = ")) + String(ledLineC->getState()));
	wifiMqtt->log(LOG_LEVEL::INFO, String(F("EFFECT RIGHT = ")) + String(ledLineR->getState()));
}
