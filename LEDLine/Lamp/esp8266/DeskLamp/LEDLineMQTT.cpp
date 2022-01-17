#include "LEDLineMQTT.h"

#include "WifiMQTT.h"

#include "ledLines_definitions.h"

uint8_t brightness = START_BRIGHTNESS;

#include <FastLED.h>

CRGB ledsLeft[NUM_LEDS_LEFT];
CRGB ledsCenter[NUM_LEDS_CENTER];
CRGB ledsRight[NUM_LEDS_RIGHT];

#include "LEDMultiLine.hpp"

LEDMultiLine<ledsLeft, NUM_LEDS_LEFT> ledLineLeft;
LEDMultiLine<ledsCenter, NUM_LEDS_CENTER> ledLineCenter;
LEDMultiLine<ledsRight, NUM_LEDS_RIGHT> ledLineRight;

#include <Ticker.h>

Ticker effectsTicker;

void setup_LED()
{
	pinMode(RELAY_PIN, OUTPUT);          // Initialize the RELAY pin as an output
	digitalWrite(RELAY_PIN, LOW);        // Turn off leds

	FastLED.addLeds<WS2812B, LED_PIN_1, GRB>(ledsLeft, NUM_LEDS_LEFT).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN_2, GRB>(ledsCenter, NUM_LEDS_CENTER).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN_3, GRB>(ledsRight, NUM_LEDS_RIGHT).setCorrection(TypicalSMD5050);

	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
	FastLED.setBrightness(brightness);
	FastLED.clear(true);
}

void processLED()
{
	bool refresh = false;

	refresh |= ledLineLeft.refresh();
	refresh |= ledLineCenter.refresh();
	refresh |= ledLineRight.refresh();

	if (refresh)
	{
		FastLED.show();
	}
}

void adjustBrightness(int8_t delta)
{
	brightness += delta;
	FastLED.setBrightness(brightness);

	WifiMQTT.log(LOG_LEVEL::INFO, String(F("BRIGHTNESS = ")) + String(brightness));
}

void changeEffect()
{
	ledLineLeft.setNextEffect();
	ledLineCenter.setNextEffect();
	ledLineRight.setNextEffect();

	WifiMQTT.log(LOG_LEVEL::INFO, String(F("EFFECT LEFT = ")) + String(ledLineLeft.getState()));
	WifiMQTT.log(LOG_LEVEL::INFO, String(F("EFFECT CENTER = ")) + String(ledLineCenter.getState()));
	WifiMQTT.log(LOG_LEVEL::INFO, String(F("EFFECT RIGHT = ")) + String(ledLineRight.getState()));
}

void turnOnLeds()
{
	effectsTicker.detach();

	ledLineLeft.setEffectByIdx(0);
	ledLineCenter.setEffectByIdx(0);
	ledLineRight.setEffectByIdx(0);

	ledLineLeft.turnOn();
	ledLineCenter.turnOn();
	ledLineRight.turnOn();

	digitalWrite(RELAY_PIN, HIGH);

	effectsTicker.attach(EFFECT_DURATION_SEC, changeEffect);

	WifiMQTT.log(LOG_LEVEL::INFO, String(F("TURN ON LEFT = ")) + String(ledLineLeft.getState()));
	WifiMQTT.log(LOG_LEVEL::INFO, String(F("TURN ON CENTER = ")) + String(ledLineCenter.getState()));
	WifiMQTT.log(LOG_LEVEL::INFO, String(F("TURN ON RIGHT = ")) + String(ledLineRight.getState()));
}

void turnOffLeds()
{
	effectsTicker.detach();

	ledLineLeft.turnOff();
	ledLineCenter.turnOff();
	ledLineRight.turnOff();

	digitalWrite(RELAY_PIN, LOW);

	WifiMQTT.log(LOG_LEVEL::INFO, String(F("TURN OFF ALL")));
}

void holdNextEffect()
{
	effectsTicker.detach();

	ledLineLeft.setNextEffect();
	ledLineCenter.setNextEffect();
	ledLineRight.setNextEffect();

	digitalWrite(RELAY_PIN, HIGH);

	WifiMQTT.log(LOG_LEVEL::INFO, String(F("HOLD EFFECT LEFT = ")) + String(ledLineLeft.getState()));
	WifiMQTT.log(LOG_LEVEL::INFO, String(F("HOLD EFFECT CENTER = ")) + String(ledLineCenter.getState()));
	WifiMQTT.log(LOG_LEVEL::INFO, String(F("HOLD EFFECT RIGHT = ")) + String(ledLineRight.getState()));
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

	bool success = false;

	success |= ledLineLeft.setEffectByName(data);
	success |= ledLineCenter.setEffectByName(data);
	success |= ledLineRight.setEffectByName(data);

	if (success)
	{
		effectsTicker.detach();
	}

	WifiMQTT.log(LOG_LEVEL::INFO, String(F("SET EFFECT LEFT = ")) + String(ledLineLeft.getState()));
	WifiMQTT.log(LOG_LEVEL::INFO, String(F("SET EFFECT CENTER = ")) + String(ledLineCenter.getState()));
	WifiMQTT.log(LOG_LEVEL::INFO, String(F("SET EFFECT RIGHT = ")) + String(ledLineRight.getState()));
}
