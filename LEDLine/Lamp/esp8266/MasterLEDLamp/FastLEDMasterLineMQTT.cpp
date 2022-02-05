#include "FastLEDMasterLineMQTT.h"

FastLEDMasterLineMQTT::FastLEDMasterLineMQTT(WifiMQTTLineMaster* wifiMqtt, LEDLine* ledLine, uint8_t startBrightness, uint16_t effectDuration)
	: FastLEDLineMQTT(wifiMqtt, ledLine, startBrightness, effectDuration)
{
}

void FastLEDMasterLineMQTT::turnOnLeds()
{
	FastLEDLineMQTT::turnOnLeds();

	static_cast<WifiMQTTLineMaster*>(wifiMqtt)->controlEffect(ledLine->getState());
}

void FastLEDMasterLineMQTT::holdNextEffect()
{
	FastLEDLineMQTT::holdNextEffect();

	static_cast<WifiMQTTLineMaster*>(wifiMqtt)->controlEffect(ledLine->getState());
}

void FastLEDMasterLineMQTT::nextEffect()
{
	FastLEDLineMQTT::nextEffect();

	static_cast<WifiMQTTLineMaster*>(wifiMqtt)->controlEffect(ledLine->getState());
}
