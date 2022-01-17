#ifndef _WIFIMQTT_H_
#define _WIFIMQTT_H_

#define MQTT_DEBUG
#define SERIAL_DEBUG

#if true //__has_include("my_data_sensitive.h")
#include "my_data_sensitive.h"
#else
#pragma message "default credentials are used"

#define WLAN_SSID           "AP wifi name"
#define WLAN_PASS           "and password"
#define WLAN_HOSTNAME       "set hostname"

#define MQTT_SERVER         "127.0.0.1"
#define MQTT_SERVERPORT     1883
#define MQTT_USERNAME       "your mqtt username"
#define MQTT_KEY            "and password"

#define MQTT_TOPIC_LOG      MQTT_USERNAME"/log"
#define MQTT_TOPIC_SUB1     MQTT_USERNAME"/set/effect"
#define MQTT_TOPIC_SUB2     MQTT_USERNAME"/set/action"

#define ON_CODE             6735
#define OFF_CODE            2344
#define NEXT_CODE           2747

#endif

#include <WiFiManager.h>

#include <WiFiClient.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#include <Ticker.h>

enum class LOG_LEVEL
{
	INFO,
	WARN,
	ERROR,
	DEBUG
};


class WifiMQTTClass
{
private:

	static WiFiClient client;
	
	static Adafruit_MQTT_Client mqtt;

	static Adafruit_MQTT_Subscribe setEffect;
	static Adafruit_MQTT_Subscribe performAction;

	volatile static bool f_pingMQTT;

	Ticker mqttTicker;

public:

	static const uint16_t BOOT_TIMEOUT = 5000;

	WifiMQTTClass();

	void init(bool configure = false);

	void process();

	void log(LOG_LEVEL level, String msg);

protected:
	
	bool keepAliveMQTT();

	static void pingMQTT_callback();

};

extern WifiMQTTClass WifiMQTT;

#endif

