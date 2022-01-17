#include "LEDMatrixMQTT.h"

#include "ledMatrix_definitions.h"

#include "WifiMQTT.h"

#include <Ticker.h>

Ticker effectsTicker;

void adjustBrightness(int8_t delta)
{
	brightness += delta;
	FastLED.setBrightness(brightness);

	WifiMQTT.log(LOG_LEVEL::INFO, String(F("BRIGHTNESS = ")) + String(brightness));
}

void changeEffect()
{
	ledMatrix.setNextEffect();

	WifiMQTT.log(LOG_LEVEL::INFO, String(F("EFFECT = ")) + String(ledMatrix.getState()));
}

void turnOnLeds()
{
	effectsTicker.detach();

	ledMatrix.setEffectByIdx(0);
	ledMatrix.turnOn();

	effectsTicker.attach(EFFECT_DURATION_SEC, changeEffect);

	WifiMQTT.log(LOG_LEVEL::INFO, String(F("TURN ON = ")) + String(ledMatrix.getState()));
}

void turnOffLeds()
{
	effectsTicker.detach();

	ledMatrix.turnOff();

	FastLED.clear(true);

	WifiMQTT.log(LOG_LEVEL::INFO, String(F("TURN OFF")));
}

void holdNextEffect()
{
	effectsTicker.detach();

	ledMatrix.setNextEffect();

	WifiMQTT.log(LOG_LEVEL::INFO, String(F("HOLD EFFECT = ")) + String(ledMatrix.getState()));
}

void performAction_callback(uint32_t x)
{
	WifiMQTT.log(LOG_LEVEL::DEBUG, String(F("new action requested = ")) + String(x));

	switch (x)
	{
	case ON_CODE:
		turnOnLeds();
		break;
	case OFF_CODE:
		turnOffLeds();
		break;
	case NEXT_CODE:
		holdNextEffect();
		break;
	default:
		break;
	}
}

void setEffect_callback(char* data, uint16_t len)
{
	WifiMQTT.log(LOG_LEVEL::DEBUG, String(F("new effect requested = ")) + String(data));

	if (ledMatrix.setEffectByName(data))
	{
		effectsTicker.detach();
	}

	WifiMQTT.log(LOG_LEVEL::INFO, String(F("SET EFFECT = ")) + String(ledMatrix.getState()));
}

void setRunningString_callback(char* data, uint16_t len)
{
	WifiMQTT.log(LOG_LEVEL::DEBUG, String(F("new running string received = ")) + String(data));

	ledMatrix.setRunningString(data, len);

	WifiMQTT.log(LOG_LEVEL::INFO, String(F("RUNNING_STRING = ")) + String(data));
}

void processLED()
{
	if (ledMatrix.refresh())
	{
		FastLED.show();
	}
}
