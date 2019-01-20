#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

/*mqtt*/
#include <PubSubClient.h>
//#include "test.h"

WiFiClient mainESP;
PubSubClient MQTT(mainESP);
/*mqtt message */
const int max_length = 40;
char msg[40];

char mqttCharServer[20];

void set_end_mark(int i) {
    if (max_length > i) msg[i] = '\0';
    else  msg[max_length - 1] = '\0';
}
void callback(char* topic, byte* payload, unsigned int length) {
    //Serial.print(topic);
    int i = 0;
    for (i = 0; i < length; i++) {
        if (i < max_length) msg[i] = (char)payload[i];
    }
    set_end_mark(i);
    //Serial.println();
}
void Set_mqtt_server(boolean adminEnabled) {
    if (!adminEnabled)
        if (needInitMQTT) {
            MQTT.setServer(mqttCharServer, config.MqttPort);
            MQTT.setCallback(callback);
            needInitMQTT = false;
        }
}

void reconnect() {
    int tryTime = 3;
    while (!MQTT.connected()) {
        if (tryTime <= 0)
            break;
        Serial.print("MQTT connecting");
        if (MQTT.connect(string2char(config.DeviceName))) {
            Serial.println("connected MQTT");
            // Once connected, publish an announcement...
            MQTT.publish(string2char(config.mqtt_topic + config.mqtt_subtopic), "hello-ESPMega");
            // ... and resubscribe
            MQTT.subscribe(string2char(config.mqtt_sub));
        }
        else {
            //Serial.println(MQTT.state());
            Serial.println("try again in 5's");
            for (int i = 0; i < 5000; i++) {
                delay(1);
            }
        }
        tryTime = tryTime - 1;
    }
}

void publish_msg(char* msg_str) {
    if (WiFi.status() != WL_CONNECTED) {
        ConfigureWifi();
        Set_mqtt_server(AdminEnabled);
    }
    if (!MQTT.connected()) reconnect();
    MQTT.publish(string2char(config.mqtt_main_topic), msg_str);
    delay(200);
}

void listen_master() {
    int i = 0;
    while (Serial.available() > 0)
    {
        char a = Serial.read();
        if (a == '\0')
            continue;
        if (a != '\r' && a != '\n' && (a < 32))
            continue;
        if (i < max_length) msg[i] = a;

        i++;
    }
    if (i == 0) return;
    set_end_mark(i);
    String msgString((char*)msg);
    if (msgString == "received")  msg[0] = '\0';
    else {
        if (WiFi.status() != WL_CONNECTED) {
            ConfigureWifi();
            Set_mqtt_server(AdminEnabled);
        }
        publish_msg((char*)msg);
        msg[0] = '\0';
    }
}

void talk_master() {
    if (msg[0] != '\0') {
        Serial.print((char*)msg);
        msg[0] = '\0';
    }
}

void Setup_Pins() {
    //pin_setup();
}

void Handle_mqtt() {
    if (WiFi.status() != WL_CONNECTED) {
        ConfigureWifi();
        Set_mqtt_server(AdminEnabled);
    }
    talk_master();
    listen_master();
    if (!MQTT.connected()) reconnect();
    MQTT.loop();
    delay(200);
}

#endif