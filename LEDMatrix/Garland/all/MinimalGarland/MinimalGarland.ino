//#define DEBUG_OUTPUT

#ifndef DEBUG_OUTPUT
#define log_print(msg)
#define log_println(msg)
#else
#define log_print(msg) Serial.print(msg)
#define log_println(msg) Serial.println(msg)
#endif

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
#define UNPINNED_ANALOG_PIN 35 // not connected analog pin
#else
#define LED_PIN  9  // leds pin
#define BTN_PIN  4  // button pin
#define ENC1_PIN 3  // encoder S1 pin
#define ENC2_PIN 2	// encoder S2 pin
#define UNPINNED_ANALOG_PIN A0 // not connected analog pin
#endif

#include <ArduinoDebounceButton.h>
ArduinoDebounceButton btn(BTN_PIN, BUTTON_CONNECTED::GND, BUTTON_NORMAL::OPEN);

#include <ArduinoRotaryEncoder.h>
ArduinoRotaryEncoder encoder(ENC2_PIN, ENC1_PIN);

#include <EventsQueue.hpp>
EventsQueue<ENCODER_EVENT, 10> queue;

#include <EEPROM.h>
#define EEPROM_ADDRESS_EFFECT 0
#define EEPROM_EFFECT_LENGTH 15

char EFFECT_NAME[EEPROM_EFFECT_LENGTH + 1];

#include <FastLED.h>

#define MATRIX_H 11
#define MATRIX_W 36

#define CURRENT_LIMIT 15000

uint8_t brightness = 100;

CRGB leds[(MATRIX_H * MATRIX_W)];

#include <ZigZagFromTopLeftToBottomAndRight.hpp>

#include "MinimalLEDMatrix.hpp"
MinimalLEDMatrix<ZigZagFromTopLeftToBottomAndRight, leds, MATRIX_W, MATRIX_H> ledMatrix;


#include <EffectTimer.hpp>
#define EFFECT_DURATION_SEC 300
MillisTimer tickerEffects(EFFECT_DURATION_SEC* MillisTimer::CLOCKS_IN_SEC);


void loadState()
{
	log_println(F("ledMatrix EFFECTS:"));
	for (uint8_t var = 0; var < ledMatrix.howManyEffects(); var++)
	{
		log_println(ledMatrix.getAllEffectsNames()[var]);
	}

#if defined(ESP32) || defined(ESP8266)
	EEPROM.begin(EEPROM_EFFECT_LENGTH);
#endif

	for (int i = 0; i < EEPROM_EFFECT_LENGTH; i++)
	{
		EFFECT_NAME[i] = EEPROM.read(EEPROM_ADDRESS_EFFECT + i);
	}

	EFFECT_NAME[EEPROM_EFFECT_LENGTH] = '\0';

	if (ledMatrix.setEffectByName(EFFECT_NAME))
	{
		log_print(F("LOADED EFFECT: "));
	}
	else
	{
		log_print(F("WRONG EFFECT: "));
	}

	log_println(EFFECT_NAME);

	ledMatrix.turnOn();
}

void saveState()
{
	strncpy(EFFECT_NAME, (ledMatrix.getEffectName() == nullptr || !ledMatrix.isOn()) ? "OFF" : ledMatrix.getEffectName(), EEPROM_EFFECT_LENGTH);

#if defined(ESP32) || defined(ESP8266)
	EEPROM.begin(EEPROM_EFFECT_LENGTH + 1);
#endif

	for (uint8_t i = 0; i < EEPROM_EFFECT_LENGTH + 1; i++)
	{
		EEPROM.write(EEPROM_ADDRESS_EFFECT + i, EFFECT_NAME[i]);
	}
#if defined(ESP32) || defined(ESP8266)
	EEPROM.commit();
#endif

	ledMatrix.turnOff();

	log_print(F("SAVED EFFECT: "));
	log_println(EFFECT_NAME);
}

void changeEffect()
{
	ledMatrix.setNextEffect();
	ledMatrix.turnOn();

	log_print(F("EFFECT: "));
	log_println(ledMatrix.getEffectName());
}

void turnOffLeds()
{
	ledMatrix.turnOff();
	FastLED.clear(true);

	log_println(F("OFF"));
}

void adjustBrightness(int8_t delta)
{
	brightness += delta;
	FastLED.setBrightness(brightness);

	log_print(F("BRIGHTNESS: "));
	log_println(brightness);
}

void handleButtonEvent(const DebounceButton* button, BUTTON_EVENT eventType)
{
	switch (eventType)
	{
	case BUTTON_EVENT::Clicked:
		changeEffect();
		break;
	case BUTTON_EVENT::DoubleClicked:
		saveState();
		break;
	case BUTTON_EVENT::LongPressed:
		turnOffLeds();
		break;
	default:
		break;
	}
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
				adjustBrightness(-5);
				break;
			case ENCODER_EVENT::RIGHT:
				adjustBrightness(5);
				break;
			default:
				break;
			}
		}
	} while (processEncEvent);

}

void setupLED()
{
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, (MATRIX_H * MATRIX_W)).setCorrection(TypicalSMD5050);
	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
	FastLED.setBrightness(brightness);
	FastLED.clear(true);
}

void setup()
{
	randomSeed(analogRead(UNPINNED_ANALOG_PIN));

#ifdef DEBUG_OUTPUT
    Serial.begin(115200);
#endif

	setupLED();

	btn.initPin();

	btn.setEventHandler(handleButtonEvent);

	encoder.initPins();

	encoder.setEventHandler(handleEncoderEvent);

	attachInterrupt(digitalPinToInterrupt(ENC1_PIN), catchEncoderTicks, CHANGE);
	attachInterrupt(digitalPinToInterrupt(ENC2_PIN), catchEncoderTicks, CHANGE);

	loadState();

	tickerEffects.start();
}

void loop()
{
	btn.check();

	processEncoder();

	if (ledMatrix.refresh())
	{
		FastLED.show();
	}

	if (tickerEffects.isReady())
	{
		changeEffect();
	}
}
