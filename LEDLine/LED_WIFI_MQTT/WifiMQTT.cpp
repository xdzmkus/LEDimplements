#include "WifiMQTT.h"

#define MQTT_DEBUG

#define SERIAL_DEBUG
#include "SerialDebug.h"

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

#endif

extern void performAction_callback(uint32_t x);
extern void setEffect_callback(char* data, uint16_t len);

WifiMQTT::WifiMQTT()
{
	mqttTicker.attach(60.0, pingMQTT_callback);

	setEffect.setCallback(setEffect_callback);
	mqtt.subscribe(&setEffect);

	performAction.setCallback(performAction_callback);
	mqtt.subscribe(&performAction);
}

WifiMQTT::~WifiMQTT()
{
}

void WifiMQTT::init(bool configure)
{
	WiFiManager wm;

	if (configure)
	{
		wm.setConfigPortalTimeout(180);

		wm.setConfigPortalBlocking(true);
		if (wm.startConfigPortal(WLAN_SSID, WLAN_PASS))
		{
			log_println(F("WiFi Reconfigured! Rebooting..."));
			delay(BOOT_TIMEOUT);
			ESP.restart();
		}
	}

	// Set the ESP8266 to be a WiFi-client
	WiFi.mode(WIFI_STA);
	WiFi.hostname(WLAN_HOSTNAME);

	log_println(WLAN_HOSTNAME);

	if (!wm.autoConnect(WLAN_SSID, WLAN_PASS))
	{
		log_println(F("Connection Failed! Rebooting..."));
		delay(BOOT_TIMEOUT);
		ESP.restart();
	}

	log_println(F("WiFi connected"));
	log_print(F("IP address: "));
	log_println(WiFi.localIP());
}

void WifiMQTT::process()
{
	if (!keepAliveMQTT()) return;

	if (f_pingMQTT && mqtt.ping())
	{
		f_pingMQTT = false;

		log_println(F("Ping MQTT... "));
	}

	mqtt.processPackets(10);
}

void WifiMQTT::log(LOG_LEVEL level, String msg)
{
	String msg_keyword = WLAN_HOSTNAME" ";
	String log_topic = MQTT_TOPIC_LOG;

	switch (level)
	{
	case LOG_LEVEL::INFO: {
		msg_keyword += "INFO: ";
		log_topic += "/info";
		break; }
	case LOG_LEVEL::WARN: {
		msg_keyword += "WARNING: ";
		log_topic += "/warning";
		break; }
	case LOG_LEVEL::ERROR: {
		msg_keyword += "ERROR: ";
		log_topic += "/error";
		break; }
	case LOG_LEVEL::DEBUG: {
		msg_keyword += "DEBUG: ";
		log_topic += "/debug";
		break; }
	default: {
		return; }
	}

	msg_keyword += msg;

	log_println(msg_keyword);

#ifndef MQTT_DEBUG
	if (level == LOG_LEVEL::DEBUG) return;
#endif

	Adafruit_MQTT_Publish mqttLog(&mqtt, log_topic.c_str());

	if (!keepAliveMQTT() || !mqttLog.publish(msg_keyword.c_str()))
	{
		log_println(F("Publish Message Failed"));
	}
}

bool WifiMQTT::keepAliveMQTT()
{
	if (!mqtt.connected())
	{
		log_println(F("Connecting to MQTT... "));

		uint8_t ret = mqtt.connect();

		if (ret != 0)
		{
			log_println(mqtt.connectErrorString(ret));
			log_println(F("Retry MQTT connection ..."));
			mqtt.disconnect();
			return false;
		}
		else
		{
			log_println(F("MQTT Connected!"));
			return true;
		}
	}

	return true;
}

void WifiMQTT::pingMQTT_callback()
{
	f_pingMQTT = true;
}

volatile bool WifiMQTT::f_pingMQTT = false;

WiFiClient              WifiMQTT::client;
Adafruit_MQTT_Client    WifiMQTT::mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_KEY);
Adafruit_MQTT_Subscribe WifiMQTT::setEffect(&mqtt, MQTT_TOPIC_SUB1, MQTT_QOS_1);
Adafruit_MQTT_Subscribe WifiMQTT::performAction(&mqtt, MQTT_TOPIC_SUB2, MQTT_QOS_1);



