#include "WifiMQTT.h"

#include "my_data_sensitive.h"

#if !defined(WLAN_SSID) || !defined(WLAN_PASS) || !defined(WLAN_HOSTNAME) || !defined(MQTT_SERVER) || !defined(MQTT_SERVERPORT) || !defined(MQTT_USERNAME) || !defined(MQTT_KEY) || !defined(MQTT_TOPIC_LOG)
#pragma message "Default credentials are used"
#endif

#ifndef WLAN_SSID
#define WLAN_SSID           "AP wifi name"
#endif

#ifndef WLAN_SSID
#define WLAN_SSID           "AP wifi name"
#endif

#ifndef WLAN_PASS
#define WLAN_PASS           "and password"
#endif

#ifndef WLAN_HOSTNAME
#define WLAN_HOSTNAME       "set hostname"
#endif

#ifndef MQTT_SERVER
#define MQTT_SERVER         "127.0.0.1"
#endif

#ifndef MQTT_SERVERPORT
#define MQTT_SERVERPORT     1883
#endif

#ifndef MQTT_USERNAME
#define MQTT_USERNAME       "your mqtt username"
#endif

#ifndef MQTT_KEY
#define MQTT_KEY            "and password"
#endif

#ifndef MQTT_TOPIC_LOG
#define MQTT_TOPIC_LOG      MQTT_USERNAME"/log"
#endif

#include "SerialDebug.h"

WifiMQTT::WifiMQTT() 
	: mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_KEY)
{
	mqttTicker.attach(60.0, pingMQTT_callback);
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
		wm.startConfigPortal(WLAN_SSID, WLAN_PASS);

		log_println(F("WiFi Reconfigured! Rebooting..."));

		delay(BOOT_TIMEOUT);
		ESP.restart();
	}

	// Set the ESP8266 to be a WiFi-client
	WiFi.mode(WIFI_STA);
	WiFi.hostname(WLAN_HOSTNAME);

	log_println(WLAN_HOSTNAME);

	wm.setEnableConfigPortal(false);

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
		log_println(F("Publish LOG Message Failed"));
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



