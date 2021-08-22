/*********** WS2812B leds *******************/
#define LED_PIN_1 12  // D6 - GPIO12 - left strip
#define LED_PIN_2 13  // D7 - GPIO13 - center strip
#define LED_PIN_3 14  // D5 - GPIO14 - right strip

#define RELAY_PIN 15  // D8 - GPIO15 - relay pin

#define NUM_LEDS_LEFT   15
#define NUM_LEDS_CENTER 24
#define NUM_LEDS_RIGHT  8

#define CURRENT_LIMIT 2000
#define EFFECT_DURATION_SEC 60

#include <FastLED.h>
CRGB ledsLeft[NUM_LEDS_LEFT];
CRGB ledsCenter[NUM_LEDS_CENTER];
CRGB ledsRight[NUM_LEDS_RIGHT];

/*********** LED Line Effects ***************/
#include "LEDMultiLine.h"
LEDLine15leds ledLineLeft(ledsLeft, NUM_LEDS_LEFT);
LEDLine ledLineCenter(ledsCenter, NUM_LEDS_CENTER);
LEDLine8leds  ledLineRight(ledsRight, NUM_LEDS_RIGHT);

#include <Ticker.h>
Ticker effectsTicker;

uint8_t brightness = 127;

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

void turnOnFlashLeds()
{
	char flash[] = "FLASH";

	ledLineLeft.setEffectByName(flash);
	ledLineCenter.setEffectByName(flash);
	ledLineRight.setEffectByName(flash);

	ledLineLeft.turnOn();
	ledLineCenter.turnOn();
	ledLineRight.turnOn();

	digitalWrite(RELAY_PIN, HIGH);
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

	publishState();
}

void turnOffLeds()
{
	effectsTicker.detach();

	ledLineLeft.turnOff();
	ledLineCenter.turnOff();
	ledLineRight.turnOff();

	digitalWrite(RELAY_PIN, LOW);

	publishState();
}

void changeEffect()
{
	effectsTicker.detach();

	ledLineLeft.setNextEffect();
	ledLineCenter.setNextEffect();
	ledLineRight.setNextEffect();

	effectsTicker.attach(EFFECT_DURATION_SEC, changeEffect);

	publishState();
}

void setEffect(const char* data)
{
	bool success = false;

	success |= ledLineLeft.setEffectByName(data);
	success |= ledLineCenter.setEffectByName(data);
	success |= ledLineRight.setEffectByName(data);

	if (success)
	{
		effectsTicker.detach();
	}

	publishState();
}

const char* getEffect(uint8_t ledLine)
{
	switch (ledLine)
	{
	case 0:
		return !ledLineLeft.isOn() ? nullptr : ledLineLeft.getEffectName();
	case 1:
		return !ledLineCenter.isOn() ? nullptr : ledLineCenter.getEffectName();
	case 2:
		return !ledLineRight.isOn() ? nullptr : ledLineRight.getEffectName();
	default:
		return nullptr;
	}

	return nullptr;
}

void adjustBrightness(int8_t delta)
{
	brightness += delta;
	FastLED.setBrightness(brightness);
}
