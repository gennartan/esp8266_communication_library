#include "Communication_Manager.hpp"

#include <Arduino.h>
#include <ArduinoMqttClient.h>
#include <ESP8266WiFi.h>


#define FAILED_WIFI_RESET 10
#define FAILED_MQTT_RESET 15

WiFiClient wifi_client;

Communication_Manager::Communication_Manager(std::string ssid, std::string pwd, std::string broker, int port, std::string client_id):
	ssid(ssid), pwd(pwd), broker(broker), port(port), client_id(client_id), client(MqttClient(wifi_client))
{
	connect_wifi(true);

	client.setId(client_id.c_str());
	check_mqtt_connection();

	Serial.printf("Mqtt client initalized with id %s\n", client_id.c_str());
}

int Communication_Manager::connect_wifi(bool first_connect)
{
	unsigned long startwifi=micros();
	unsigned int retry_count = 0;

	if(!first_connect)
	{
		WiFi.reconnect();
	}
	else
	{
		WiFi.mode(WIFI_STA);
		WiFi.begin(ssid.c_str(), pwd.c_str()); // Start WiFI
	}

	Serial.printf("Connecting to %s\n", ssid.c_str());

	while ((WiFi.status() != WL_CONNECTED) && (retry_count < 40)) {
		delay(500);
		retry_count++;
	}
	int success = WiFi.status();
	if(success == WL_CONNECTED)
	{
		Serial.printf("Connected to %s\n", WiFi.localIP().toString().c_str());
		failedWifiConsecutive = 0;
	}
	else
	{
		Serial.printf("Failed to connect\n");
	}
	Serial.printf("Number of tries: %d\n", retry_count);
	Serial.printf("Connecting time (ms): %lu\n", micros()-startwifi);

	return success;
}

void Communication_Manager::check_wifi_connection()
{
	if(WiFi.status()!=WL_CONNECTED) { // Lost connection
		failedWifi++;
		failedWifiConsecutive++;
		connect_wifi(false);
	}
	if(failedWifiConsecutive>FAILED_WIFI_RESET) {
		ESP.restart();
	}
}

void Communication_Manager::check_mqtt_connection()
{
	if (!client.connected())
	{
		client.connect(broker.c_str(), port);
	}

	if (!client.connected())
	{
		failedMqtt++;
		failedMqttConsecutive++;
	}
	else
	{
		failedMqttConsecutive = 0;
	}

	if(failedMqttConsecutive>FAILED_MQTT_RESET) {
		ESP.restart();
	}
}

void Communication_Manager::publish(std::string topic, std::string message)
{
	client.beginMessage(topic.c_str());
	client.print(message.c_str());
	client.endMessage();
}

void subscribe(std::string topic, int qos)
{
}

