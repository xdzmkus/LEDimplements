#include "FastLEDMasterLineMQTT.h"

FastLEDMasterLineMQTT::FastLEDMasterLineMQTT(WifiMQTTMaster* wifiMqtt, LEDLine* ledLine, uint8_t startBrightness, uint16_t effectDuration)
	: FastLEDLineMQTT(wifiMqtt, ledLine, startBrightness, effectDuration)
{
}

void FastLEDMasterLineMQTT::turnOnLeds()
{
	FastLEDLineMQTT::turnOnLeds();

	static_cast<WifiMQTTMaster*>(wifiMqtt)->pubMaster(ledLine->getState());
}

void FastLEDMasterLineMQTT::turnOffLeds()
{
	FastLEDLineMQTT::turnOffLeds();

	static_cast<WifiMQTTMaster*>(wifiMqtt)->pubMaster(ledLine->getState());
}

void FastLEDMasterLineMQTT::holdNextEffect()
{
	FastLEDLineMQTT::holdNextEffect();

	static_cast<WifiMQTTMaster*>(wifiMqtt)->pubMaster(ledLine->getState());
}

void FastLEDMasterLineMQTT::nextEffect()
{
	FastLEDLineMQTT::nextEffect();

	static_cast<WifiMQTTMaster*>(wifiMqtt)->pubMaster(ledLine->getState());
}
