
#define LED_PIN1  5   // leds pin
#define LED_PIN2  6   // leds pin
#define LED_PIN3  7   // leds pin
#define LED_PIN4  8   // leds pin
#define LED_PIN5  11  // leds pin
#define LED_PIN6  12  // leds pin
#define LED_PIN7  9   // leds pin
#define LED_PIN8  10  // leds pin
#define BTN_PIN  4  // button pin
#define ENC1_PIN 2  // encoder S1 pin
#define ENC2_PIN 3	// encoder S2 pin
#define UNPINNED_ANALOG_PIN A0 // not connected analog pin


#define MATRIX_H 30
#define MATRIX_W 8

#define CURRENT_LIMIT 15000
#define MIN_BRIGHTNESS 20
#define EFFECT_DURATION_SEC 60

#include <ArduinoDebounceButton.h>
ArduinoDebounceButton btn(BTN_PIN, BUTTON_CONNECTED::GND, BUTTON_NORMAL::OPEN);

#include <ArduinoRotaryEncoder.h>
ArduinoRotaryEncoder encoder(ENC2_PIN, ENC1_PIN);

#include <EventsQueue.hpp>
EventsQueue<ENCODER_EVENT, 8> queue;

#include <FastLED.h>
CRGB leds[(MATRIX_H * MATRIX_W)];

/*********** LED Matrix Effects *************/
#include <MatrixLineConverters.h>
#include "LEDWallGarland.hpp"

LEDWallGarland<ParallelFromBottomRightToLeft, leds, MATRIX_W, MATRIX_H> ledMatrix;

#include "FastLEDLine.h"
FastLEDLine timerLeds(&ledMatrix, MIN_BRIGHTNESS, EFFECT_DURATION_SEC);

void setup_FastLED()
{
	FastLED.addLeds<WS2812B, LED_PIN1, GRB>(leds + 0 * MATRIX_H, MATRIX_H).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN2, GRB>(leds + 1 * MATRIX_H, MATRIX_H).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN3, GRB>(leds + 2 * MATRIX_H, MATRIX_H).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN4, GRB>(leds + 3 * MATRIX_H, MATRIX_H).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN5, GRB>(leds + 4 * MATRIX_H, MATRIX_H).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN6, GRB>(leds + 5 * MATRIX_H, MATRIX_H).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN7, GRB>(leds + 6 * MATRIX_H, MATRIX_H).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN8, GRB>(leds + 7 * MATRIX_H, MATRIX_H).setCorrection(TypicalSMD5050);

	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
}

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
				break;
			case ENCODER_EVENT::RIGHT:
				timerLeds.adjustBrightness(5);
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
		break;
	case BUTTON_EVENT::DoubleClicked:
		timerLeds.turnOnLeds();
		break;
	case BUTTON_EVENT::RepeatClicked:
		timerLeds.adjustBrightness(-MIN_BRIGHTNESS);
		break;
	case BUTTON_EVENT::LongPressed:
		timerLeds.turnOffLeds();
		break;
	default:
		break;
	}
}

void setup()
{
	randomSeed(analogRead(UNPINNED_ANALOG_PIN));

	setup_FastLED();

	timerLeds.setup();

	btn.initPin();

	btn.setEventHandler(handleButtonEvent);

	encoder.initPins();

	encoder.setEventHandler(handleEncoderEvent);

	attachInterrupt(digitalPinToInterrupt(ENC1_PIN), catchEncoderTicks, CHANGE);
	attachInterrupt(digitalPinToInterrupt(ENC2_PIN), catchEncoderTicks, CHANGE);

	timerLeds.turnOnLeds();
}

void loop()
{
	btn.check();

	processEncoder();

	timerLeds.process();
}
