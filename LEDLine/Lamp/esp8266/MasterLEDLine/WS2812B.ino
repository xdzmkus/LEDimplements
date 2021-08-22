/*********** WS2812B leds *******************/
#define LED_PIN_1 12  // D6 - GPIO12 - left strip
#define LED_PIN_2 13  // D7 - GPIO13 - center strip
#define LED_PIN_3 14  // D5 - GPIO14 - right strip

#define RELAY_PIN 15  // D8 - GPIO15 - relay pin

#define NUM_LEDS 8

#define CURRENT_LIMIT 3500
#define EFFECT_DURATION_SEC 60

#include <FastLED.h>
CRGB leds[NUM_LEDS];

/*********** LED Line Effects ***************/
#include "LEDSingleLine.h"
LEDLine8leds ledLine(leds, NUM_LEDS);

#include <Ticker.h>
Ticker effectsTicker;

uint8_t brightness = 127;

void setup_LED()
{
	pinMode(RELAY_PIN, OUTPUT);          // Initialize the RELAY pin as an output
	digitalWrite(RELAY_PIN, LOW);        // Turn off leds

	FastLED.addLeds<WS2812B, LED_PIN_1, GRB>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN_2, GRB>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN_3, GRB>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);

	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
    FastLED.setBrightness(brightness);
    FastLED.clear(true);
}

void processLED()
{
	if (ledLine.refresh())
	{
		FastLED.show();
	}
}

void turnOnFlashLeds()
{
	char flash[] = "FLASH";

	ledLine.setEffectByName(flash);
	ledLine.turnOn();

	digitalWrite(RELAY_PIN, HIGH);

	publishState();
}

void turnOnLeds()
{
	effectsTicker.detach();

	ledLine.setEffectByIdx(0);

	ledLine.turnOn();

	digitalWrite(RELAY_PIN, HIGH);

	effectsTicker.attach(EFFECT_DURATION_SEC, changeEffect);

	publishState();

	publishSlaveEffect();
}

void turnOffLeds()
{
	effectsTicker.detach();

	ledLine.turnOff();

	digitalWrite(RELAY_PIN, LOW);

	publishState();
}

void changeEffect()
{
	effectsTicker.detach();

	ledLine.setNextEffect();

	effectsTicker.attach(EFFECT_DURATION_SEC, changeEffect);

	publishState();

	publishSlaveEffect();
}

void setEffect(const char* data)
{
	bool success = ledLine.setEffectByName(data);

	if (success)
	{
		effectsTicker.detach();
	}

	publishState();
}

const char* getEffect()
{
	return !ledLine.isOn() ? nullptr : ledLine.getEffectName();
}

void adjustBrightness(int8_t delta)
{
	brightness += delta;
	FastLED.setBrightness(brightness);
}
