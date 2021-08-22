/*********** WS2812B leds *******************/
#if defined(ESP8266)
#define LED_PIN  D5 // leds pin
#elif defined(ESP32)
#define LED_PIN  16 // leds pin
#endif

#define NUM_LEDS 256
#define CURRENT_LIMIT 8000
#define EFFECT_DURATION_SEC 45

#include <FastLED.h>
CRGB leds[NUM_LEDS];

/*********** LED Line Effects ***************/
#include "LEDLine256.h"
LEDLine256<leds, NUM_LEDS> ledLine;

#include <Ticker.h>
Ticker effectsTicker;

uint8_t brightness = 127;

void setup_LED()
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
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

void turnOnLeds()
{
	effectsTicker.detach();

	ledLine.setEffectByIdx(0);
	ledLine.turnOn();

	effectsTicker.attach(EFFECT_DURATION_SEC, changeEffect);

	publishState();
}

void turnOffLeds()
{
	effectsTicker.detach();

	ledLine.turnOff();

	FastLED.clear(true);

	publishState();
}

void turnOnFlashLeds()
{
	ledLine.setEffectByName("FLASH");

	ledLine.turnOn();
}

void changeEffect()
{
	effectsTicker.detach();

	ledLine.setNextEffect();

	effectsTicker.attach(EFFECT_DURATION_SEC, changeEffect);

	publishState();
}

void setEffect(const char* data)
{
	if (ledLine.setEffectByName(data))
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
