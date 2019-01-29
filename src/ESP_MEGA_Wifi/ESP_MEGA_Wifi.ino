#include "FS.h"
#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <Ticker.h>

#include "datad.h"
#include "helpers.h"
#include "WebGlobal.h"
#include "OTA.h"
#include "MQTT_Client.h"

/*
Include the HTML, STYLE and Script "Pages"
*/
#include "Page_Root.h"
#include "Page_Admin.h"
#include "Page_Script.js.h"
#include "Page_Style.css.h"
#include "Page_NTPsettings.h"
#include "Page_Information.h"
#include "Page_General.h"
#include "PAGE_NetworkConfiguration.h"
#include "PAGE_MqttConfig.h"
#include "example.h"
#include "Mega.h"
#include "WebServer.h"

#define AdminTimeOut 300 // Defines the Time in Seconds, when the Admin-Mode will be diabled

void setup(void) {
    if (!SPIFFS.begin()) {
        Serial.println("Failed mount conf");
        return;
    }
    Serial.begin(115200);
    delay(500);
    ReadConfig();
    Serial.println("Config fetched");

    ConfigureWifi();

    Setup_Pins();
    Setup_web_page();

    Serial.println("WebServer started");
    delay(500);
    Serial.println("ESP Ver. 0.17.1");
    tkSecond.attach(1, Second_Tick);
    UDPNTPClient.begin(2390);  // Port for NTP receive

    // Start OTA server.
    SetupOTA(string2char(config.DeviceName));
}


void loop(void) {
    if (!HandleOTA(config.ota))// OTA mode 
    {
        if (AdminEnabled)
        {
            if (AdminTimeOutCounter > AdminTimeOut)
            {
                AdminEnabled = false;
                WiFi.mode(WIFI_STA);
                Set_serverName();
                Set_mqtt_server(AdminEnabled);
                Serial.println("Admin disabled!");
            }
            // sync time setting at beginning 
            Handle_NTP();
            SyncWithValidNPT();
        }

        //Handle_auto_switch();

        server.handleClient();

        // mqtt communication
        if (!AdminEnabled)
        {
            Handle_mqtt();
        }

        if (Refresh)
        {
            Refresh = false;
        }

    }

}
