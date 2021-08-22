#if defined(ESP8266)
#define LED_PIN D5  // leds pin
#define LEFT_PIN A0
#define RIGHT_PIN A0
#elif defined(ESP32)
#define LED_PIN  16 // leds pin
#define LEFT_PIN A0
#define RIGHT_PIN A0
#else
#define LED_PIN 9   // leds pin
#define LEFT_PIN A0
#define RIGHT_PIN A1
#endif

#define MATRIX_H 11
#define MATRIX_W 36
#define CURRENT_LIMIT 16000
#define BRIGHTNESS 120

#include <FastLED.h>
CRGB leds[(MATRIX_H * MATRIX_W)];

#include <ZigZagFromTopLeftToBottomAndRight.hpp>
#include <LEDAudioEffects.h>

VUMeterMatrixLedEffect<ZigZagFromTopLeftToBottomAndRight, leds, MATRIX_W, MATRIX_H> effect(30, 512);

bool whichChannel = false;

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

void setup_LED()
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, (MATRIX_H * MATRIX_W));
    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear(true);
}

void setup()
{

#ifdef ADCSRA

    // ��������� ������� ������ ����������� ����� �� 38.4 ���, �� �������
    // ������������ (���������) ������������ ������� ������������� ����� 19 ���
    // http://yaab-arduino.blogspot.ru/2015/02/fast-sampling-from-analog-input.html

    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);

#endif // ADCSRA

    analogReference(EXTERNAL);

    setup_LED();

    effect.start();
}

void loop()
{
    if (effect.isReady())
    {
        uint16_t r = 0;
        uint16_t l = 0;
        analyzeAudio(r, l);
//      effect.autoGain(max(r, l));
        effect.paint(r, l);
        FastLED.show();
    }
}

void analyzeAudio(uint16_t& r, uint16_t& l)
{
    r = l = 0;
    uint16_t rMin = -1;
    uint16_t lMin = -1;

    bool rightChannel = true;

    for (uint16_t i = 0; i < 256; i++)
    {
        uint16_t value = analogRead(rightChannel ? RIGHT_PIN : LEFT_PIN);

        if (rightChannel)
        {
            if (value > r) r = value;
            if (value < rMin) rMin = value;
        }
        else
        {
            if (value > l) l = value;
            if (value < lMin) lMin = value;
        }
        rightChannel = !rightChannel;
    }

    r -= rMin;
    l -= lMin;
}
