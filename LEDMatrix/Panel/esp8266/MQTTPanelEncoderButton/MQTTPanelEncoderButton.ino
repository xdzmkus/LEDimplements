#include "my_data_sensitive.h"

#include "SerialDebug.h"

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
#endif

#define MATRIX_H 8
#define MATRIX_W 32

#define CURRENT_LIMIT 8000
#define START_BRIGHTNESS 20
#define EFFECT_DURATION_SEC 60

#include <Ticker.h>
Ticker builtinLedTicker;

#include <ArduinoDebounceButton.h>
ArduinoDebounceButton btn(BTN_PIN, BUTTON_CONNECTED::GND, BUTTON_NORMAL::OPEN);

#include <ArduinoRotaryEncoder.h>
ArduinoRotaryEncoder encoder(ENC2_PIN, ENC1_PIN);

#include <EventsQueue.hpp>
EventsQueue<ENCODER_EVENT, 16> queue;

/*********** WIFI MQTT Manager ***************/

#include "WifiMQTTMatrix.h"
WifiMQTTMatrix wifiMqtt;

/*********** LED Matrix Effects *************/

#include <FastLED.h>
CRGB leds[(MATRIX_H * MATRIX_W)];

#include <MatrixLineConverters.h>
#include "LEDPanel.hpp"

LEDPanel<ZigZagFromBottomRightToUpAndLeft, leds, MATRIX_W, MATRIX_H> ledMatrix;

#include "FastLEDLineMQTT.h"
FastLEDLineMQTT mqttLeds(&wifiMqtt, &ledMatrix, START_BRIGHTNESS, EFFECT_DURATION_SEC);

void setup_FastLED()
{
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, (MATRIX_H * MATRIX_W)).setCorrection(TypicalSMD5050);
	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
}

IRAM_ATTR void blinkLED()
{
	//toggle LED state
	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

IRAM_ATTR void catchEncoderTicks()
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
				mqttLeds.adjustBrightness(-5);
				break;
			case ENCODER_EVENT::RIGHT:
				mqttLeds.adjustBrightness(5);
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
		mqttLeds.holdNextEffect();
		break;
	case BUTTON_EVENT::DoubleClicked:
		mqttLeds.turnOnLeds();
		break;
	case BUTTON_EVENT::RepeatClicked:
		mqttLeds.adjustBrightness(-START_BRIGHTNESS);
		break;
	case BUTTON_EVENT::LongPressed:
		mqttLeds.turnOffLeds();
		break;
	default:
		break;
	}
}

void setAction_callback(uint32_t x)
{
	wifiMqtt.log(LOG_LEVEL::DEBUG, String(F("new action requested = ")) + String(x));

	switch (x)
	{
	case ON_CODE:
		mqttLeds.turnOnLeds();
		break;
	case OFF_CODE:
		mqttLeds.turnOffLeds();
		break;
	case NEXT_CODE:
		mqttLeds.holdNextEffect();
		break;
	default:
		break;
	}
}

void setEffect_callback(char* data, uint16_t len)
{
	wifiMqtt.log(LOG_LEVEL::DEBUG, String(F("new effect requested = ")) + String(data));

	mqttLeds.holdEffectByName(data);
}

void setRunningString_callback(char* data, uint16_t len)
{
	wifiMqtt.log(LOG_LEVEL::DEBUG, String(F("new running string received = ")) + String(data));

	ledMatrix.setRunningString(data, len);

	wifiMqtt.log(LOG_LEVEL::INFO, String(F("RUNNING_STRING = ")) + String(data));
}

void setup()
{
	Serial.begin(115200);

	pinMode(LED_BUILTIN, OUTPUT);        // Initialize the BUILTIN_LED pin as an output
	digitalWrite(LED_BUILTIN, LOW);      // Turn the LED on by making the voltage LOW

	randomSeed(analogRead(UNPINNED_ANALOG_PIN));

	setup_FastLED();

	mqttLeds.setup();

	btn.initPin();

	delay(wifiMqtt.BOOT_TIMEOUT);

	builtinLedTicker.attach_ms(500, blinkLED);  // Blink led while setup

	bool f_setupMode = btn.check();

	if (f_setupMode) log_println(F("BUTTON PRESSED - RECONFIGURE WIFI"));

	wifiMqtt.init(f_setupMode);

	btn.setEventHandler(handleButtonEvent);

	encoder.initPins();

	encoder.setEventHandler(handleEncoderEvent);

	attachInterrupt(digitalPinToInterrupt(ENC1_PIN), catchEncoderTicks, CHANGE);
	attachInterrupt(digitalPinToInterrupt(ENC2_PIN), catchEncoderTicks, CHANGE);

	builtinLedTicker.detach();
	digitalWrite(LED_BUILTIN, HIGH);    // Turn the LED off by making the voltage HIGH

	mqttLeds.turnOnLeds();
}

void loop()
{
	btn.check();

	processEncoder();

	wifiMqtt.process();

	mqttLeds.process();
}

