#ifndef __MQTT_MANAGER_HPP__
#define __MQTT_MANAGER_HPP__

#include <iostream>
#include <ArduinoMqttClient.h>
#include <ESP8266WiFi.h>

class Communication_Manager
{
	public:
		Communication_Manager(std::string ssid, std::string pwd, std::string broker, int port, std::string client_id);
		int connect_wifi(bool first_connect);
		void publish(std::string topic, std::string message);
		void subscribe(std::string topic, int qos);
		void check_wifi_connection();
		void check_mqtt_connection();
	private:
		std::string ssid;
		std::string pwd;

		MqttClient client;
		std::string broker;
		int port;
		std::string client_id;

		int failedWifi = 0;
		int failedWifiConsecutive = 0;
		int failedMqtt = 0;
		int failedMqttConsecutive = 0;
};

#endif
