#include "LEDMatrixTimer.h"

#include "ledMatrix_definitions.h"

#include "SerialDebug.h"

MillisTimer timerEffects(EFFECT_DURATION_SEC* MillisTimer::CLOCKS_IN_SEC);

void adjustBrightness(int8_t delta)
{
	brightness += delta;
	FastLED.setBrightness(brightness);

	log_println(String(F("BRIGHTNESS = ")) + String(brightness));
}

void changeEffect()
{
	ledMatrix.setNextEffect();

	log_println(String(F("EFFECT = ")) + String(ledMatrix.getState()));
}

void turnOnLeds()
{
	ledMatrix.setEffectByIdx(0);
	ledMatrix.turnOn();

	timerEffects.start();

	log_println(String(F("TURN ON = ")) + String(ledMatrix.getState()));
}

void turnOffLeds()
{
	timerEffects.stop();

	ledMatrix.turnOff();

	FastLED.clear(true);

	log_println(String(F("TURN OFF")));
}

void holdNextEffect()
{
	timerEffects.stop();

	ledMatrix.setNextEffect();

	log_println(String(F("HOLD EFFECT = ")) + String(ledMatrix.getState()));
}

void processLED()
{
	if (timerEffects.isReady())
		changeEffect();

	if (ledMatrix.refresh())
	{
		FastLED.show();
	}
}
