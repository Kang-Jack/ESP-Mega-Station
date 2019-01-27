#ifndef WEBSERVER_H
#define WEBSERVER_H


void Setup_web_page() {
    server.on("/", processExample);
    server.on("/admin/filldynamicdata", filldynamicdata);
    server.on("/favicon.ico", []() {server.send(200, "text/html", "");});
    server.on("/admin.html", []() {server.send(200, "text/html", PAGE_AdminMainPage);   });
    server.on("/config.html", send_network_configuration_html);
    server.on("/info.html", []() {server.send(200, "text/html", PAGE_Information);   });
    server.on("/ntp.html", send_NTP_configuration_html);
    server.on("/general.html", send_general_html);
    server.on("/mqttconfig.html", send_mqtt_configuration_html);
    server.on("/style.css", []() {server.send(200, "text/plain", PAGE_Style_css);  });
    server.on("/microajax.js", []() {server.send(200, "text/plain", PAGE_microajax_js);  });
    server.on("/admin/values", send_network_configuration_values_html);
    server.on("/admin/connectionstate", send_connection_state_values_html);
    server.on("/admin/infovalues", send_information_values_html);
    server.on("/admin/ntpvalues", send_NTP_configuration_values_html);
    server.on("/admin/generalvalues", send_general_configuration_values_html);
    server.on("/admin/devicename", send_devicename_value_html);
    server.on("/admin/mqttvalues", send_mqtt_configuration_values_html);


    server.onNotFound([]() { Serial.println("Page Not Found"); server.send(400, "text/html", "Page not Found");   });
    server.begin();

}
void Handle_NTP() {
    if (config.Update_Time_Via_NTP_Every > 0)
    {
        if (cNTP_Update > 5 && firstStart)
        {
            NTPRefresh();
            cNTP_Update = 0;
            firstStart = false;
        }
        else if (cNTP_Update > (config.Update_Time_Via_NTP_Every * 60))
        {

            NTPRefresh();
            cNTP_Update = 0;
        }
    }
}

void Handle_auto_switch() {
    if (DateTime.minute != Minute_Old)
    {
        Minute_Old = DateTime.minute;
        if (config.AutoTurnOn)
        {
            if (DateTime.hour == config.TurnOnHour && DateTime.minute == config.TurnOnMinute)
            {
                Serial.println("SwitchON");
            }
        }


        Minute_Old = DateTime.minute;
        if (config.AutoTurnOff)
        {
            if (DateTime.hour == config.TurnOffHour && DateTime.minute == config.TurnOffMinute)
            {
                Serial.println("SwitchOff");
            }
        }
    }
}

#endif