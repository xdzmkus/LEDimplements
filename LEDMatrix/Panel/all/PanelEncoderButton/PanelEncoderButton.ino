#define SERIAL_DEBUG
#include"SerialDebug.h"

#if defined(ESP8266)
#define LED_PIN  D5 // leds pin
#define BTN_PIN  D0 // button pin
#define ENC1_PIN D1 // encoder S1 pin
#define ENC2_PIN D2	// encoder S2 pin
#define UNPINNED_ANALOG_PIN A0 // not connected analog pin
#elif defined(ESP32)
#define LED_PIN  16 // leds pin
#define BTN_PIN  5  // button pin
#define ENC1_PIN 19 // encoder S1 pin
#define ENC2_PIN 18	// encoder S2 pin
#define UNPINNED_ANALOG_PIN A0 // not connected analog pin
#else
#define LED_PIN  9  // leds pin
#define BTN_PIN  4  // button pin
#define ENC1_PIN 3  // encoder S1 pin
#define ENC2_PIN 2	// encoder S2 pin
#define UNPINNED_ANALOG_PIN A0 // not connected analog pin
#endif

#define MATRIX_H 8
#define MATRIX_W 32

#define CURRENT_LIMIT 8000
#define MIN_BRIGHTNESS 20
#define EFFECT_DURATION_SEC 60

#include <ArduinoDebounceButton.h>
ArduinoDebounceButton btn(BTN_PIN, BUTTON_CONNECTED::GND, BUTTON_NORMAL::OPEN);

#include <ArduinoRotaryEncoder.h>
ArduinoRotaryEncoder encoder(ENC2_PIN, ENC1_PIN);

#include <EventsQueue.hpp>
EventsQueue<ENCODER_EVENT, 16> queue;

#include <FastLED.h>
CRGB leds[(MATRIX_H * MATRIX_W)];

/*********** LED Matrix Effects *************/
#include <MatrixLineConverters.h>
#include "LEDPanel.hpp"

LEDPanel<ZigZagFromBottomRightToUpAndLeft, leds, MATRIX_W, MATRIX_H> ledMatrix;

#ifdef SERIAL_DEBUG
#include "FastLEDLineLog.h"
FastLEDLineLog timerLeds(&ledMatrix, MIN_BRIGHTNESS, EFFECT_DURATION_SEC);
#else
#include "FastLEDLine.h"
FastLEDLine timerLeds(&ledMatrix, MIN_BRIGHTNESS, EFFECT_DURATION_SEC);
#endif // SERIAL_DEBUG

void setup_FastLED()
{
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, (MATRIX_H * MATRIX_W)).setCorrection(TypicalSMD5050);
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
	Serial.begin(115200);

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
