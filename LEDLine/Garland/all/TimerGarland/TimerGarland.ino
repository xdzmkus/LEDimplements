#define DYNAMIC_EFFECTS

#if defined(ESP8266)
#define LED_PIN D5  // leds pin
#define UNPINNED_ANALOG_PIN A0 // not connected analog pin
#elif defined(ESP32)
#define LED_PIN  16 // leds pin
#define UNPINNED_ANALOG_PIN 35 // not connected analog pin
#else
#define LED_PIN 9   // leds pin
#define UNPINNED_ANALOG_PIN A0 // not connected analog pin
#endif

#include <FastLED.h>
#define NUM_LEDS 256
#define CURRENT_LIMIT 8000

uint8_t brightness = 127;

CRGB leds[NUM_LEDS];

#ifdef DYNAMIC_EFFECTS
#include <DynamicLEDLine.hpp>
DynamicLEDLine<leds, NUM_LEDS> ledLine;
#else
#include <StaticLEDLine.hpp>
StaticLEDLine<leds, NUM_LEDS> ledLine;
#endif

#include <EffectTimer.hpp>

#define EFFECT_DURATION_SEC 60
MillisTimer timerEffects(EFFECT_DURATION_SEC * MillisTimer::CLOCKS_IN_SEC);

void changeEffect()
{
	ledLine.setNextEffect();

	Serial.print(F("EFFECT: "));
	Serial.println(ledLine.getEffectName());
}

void setupLED()
{
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
	FastLED.setBrightness(brightness);
	FastLED.clear(true);
}

void setup()
{
	randomSeed(analogRead(UNPINNED_ANALOG_PIN));

	Serial.begin(115200);

	Serial.println(F("LEDLine EFFECTS:"));
	for (uint8_t var = 0; var < ledLine.howManyEffects(); var++)
	{
		Serial.println(ledLine.getAllEffectsNames()[var]);
	}

	setupLED();

	ledLine.turnOn();

	Serial.print(F("EFFECT: "));
	Serial.println(ledLine.getEffectName());

	timerEffects.start();
}

void loop()
{
	if (timerEffects.isReady())
		changeEffect();

	if (ledLine.refresh())
	{
		FastLED.show();
	}
}
