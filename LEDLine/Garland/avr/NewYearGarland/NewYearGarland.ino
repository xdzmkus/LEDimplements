#define SERIAL_DEBUG
#include"SerialDebug.h"

#if defined(ESP8266)
#define LED_PIN  D5 // leds pin
#define BTN_PIN  D0 // button pin
#define ENC1_PIN D1 // encoder S1 pin
#define ENC2_PIN D2	// encoder S2 pin
#define BRIGHTNESS_ANALOG_PIN A0 // auto brightness analog pin
#elif defined(ESP32)
#define LED_PIN  16 // leds pin
#define BTN_PIN  5  // button pin
#define ENC1_PIN 19 // encoder S1 pin
#define ENC2_PIN 18	// encoder S2 pin
#define BRIGHTNESS_ANALOG_PIN 35 // auto brightness analog pin
#else
#define LED_PIN  9  // leds pin
#define BTN_PIN  4  // button pin
#define ENC1_PIN 3  // encoder S1 pin
#define ENC2_PIN 2	// encoder S2 pin
#define BRIGHTNESS_ANALOG_PIN A7 // auto brightness analog pin
#endif

#define NUM_LEDS 50
#define CURRENT_LIMIT 2000
#define MIN_BRIGHTNESS 20
#define EFFECT_DURATION_SEC 120
#define AUTO_BRIGHTNESS_INTERVAL_MS 3000 // 3 seconds

/*********** LED Line Effects ***************/

#include <FastLED.h>
CRGB leds[NUM_LEDS];

#include "LEDNewYearLine.hpp"
LEDNewYearLine<leds, NUM_LEDS> ledLine;

#ifdef SERIAL_DEBUG
#include "FastLEDLineLog.h"
FastLEDLineLog timerLeds(&ledLine, MIN_BRIGHTNESS, EFFECT_DURATION_SEC);
#else
#include "FastLEDLine.h"
FastLEDLine timerLeds(&ledLine, MIN_BRIGHTNESS, EFFECT_DURATION_SEC);
#endif // SERIAL_DEBUG


MillisTimer brightnessTimer(AUTO_BRIGHTNESS_INTERVAL_MS);

/********** Encoder module ***********/

#include <ArduinoDebounceButton.h>
ArduinoDebounceButton btn(BTN_PIN, BUTTON_CONNECTED::GND, BUTTON_NORMAL::OPEN);

#include <ArduinoRotaryEncoder.h>
ArduinoRotaryEncoder encoder(ENC2_PIN, ENC1_PIN);

#include <EventsQueue.hpp>
EventsQueue<ENCODER_EVENT, 10> queue;

void setup_FastLED()
{
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
}

#if defined(ESP8266)
IRAM_ATTR
#endif
void catchEncoderTicks()
{
	encoder.catchTicks();
}

void handleEncoderEvent(const RotaryEncoder* enc, ENCODER_EVENT eventType)
{
	queue.push(eventType);
}

void processEncoder()
{
	bool processEncEvent;
	ENCODER_EVENT encEvent;

	do
	{
		noInterrupts();

		processEncEvent = queue.length();

		if (processEncEvent)
		{
			encEvent = queue.pop();
		}

		interrupts();

		if (processEncEvent)
		{
			switch (encEvent)
			{
			case ENCODER_EVENT::LEFT:
				timerLeds.adjustBrightness(-5);
				brightnessTimer.stop();
				break;
			case ENCODER_EVENT::RIGHT:
				timerLeds.adjustBrightness(5);
				brightnessTimer.stop();
				break;
			default:
				break;
			}
		}
	} while (processEncEvent);
}

void handleButtonEvent(const DebounceButton* button, BUTTON_EVENT eventType)
{
	switch (eventType)
	{
	case BUTTON_EVENT::Clicked:
		timerLeds.holdNextEffect();
		brightnessTimer.start();
		break;
	case BUTTON_EVENT::DoubleClicked:
		timerLeds.turnOnLeds();
		brightnessTimer.start();
		break;
	case BUTTON_EVENT::RepeatClicked:
		brightnessTimer.start();
		break;
	case BUTTON_EVENT::LongPressed:
		timerLeds.turnOffLeds();
		brightnessTimer.stop();
		break;
	default:
		break;
	}
}

void setup()
{
	Serial.begin(115200);

	randomSeed(analogRead(BRIGHTNESS_ANALOG_PIN));

	setup_FastLED();

	timerLeds.setup();

	btn.initPin();

	btn.setEventHandler(handleButtonEvent);

	encoder.initPins();

	encoder.setEventHandler(handleEncoderEvent);

	attachInterrupt(digitalPinToInterrupt(ENC1_PIN), catchEncoderTicks, CHANGE);
	attachInterrupt(digitalPinToInterrupt(ENC2_PIN), catchEncoderTicks, CHANGE);

	timerLeds.turnOnLeds();

	brightnessTimer.start();
}

void loop()
{
	btn.check();

	processEncoder();

	timerLeds.process();

	if (brightnessTimer.isReady())
	{
		uint8_t newBrightness = constrain(map(analogRead(BRIGHTNESS_ANALOG_PIN), 0, 1023, 0, 255), MIN_BRIGHTNESS, 255);
		timerLeds.setBrightness(newBrightness);
	}
}
