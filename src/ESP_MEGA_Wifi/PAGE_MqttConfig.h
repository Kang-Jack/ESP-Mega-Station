//
//  HTML PAGE
//
const char PAGE_MQTTConfiguration[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<a href="admin.html"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>OTA and MQTT</strong>
<hr>
MQTT settings:<br>
<form action="" method="get">
<table border="0"  cellspacing="0" cellpadding="3" style="width:310px" >
<tr><td align="right">OTA:</td><td><input type="checkbox" id="ota" name="ota"></td></tr>
<tr><td align="right">MqttServer:</td><td><input type="text" id="server" name="server" value=""> (local) means use ip as server </td></tr>
<tr><td align="right">MqttIP:</td><td><input type="text" id="ip_0" name="ip_0" size="3">.<input type="text" id="ip_1" name="ip_1" size="3">.<input type="text" id="ip_2" name="ip_2" size="3">.<input type="text" id="ip_3" name="ip_3" value="" size="3"></td></tr>
<tr><td align="right">MqttPort:</td><td><input type="text" id="port" name="port" value="" size="6"></td></tr>
<tr><td align="right">User:</td><td><input type="text" id="user" name="user" value=""></td></tr>
<tr><td align="right">Password:</td><td><input type="text" id="pass" name="pass" value=""></td></tr>
<tr><td align="right">MainTopic:</td><td><input type="text" id="main_t" name="main_t" value=""></td></tr>
<tr><td align="right">Subtopic:</td><td><input type="text" id="sub_t" name="sub_t" value=""></td></tr>
<tr><td align="right">Subscription:</td><td><input type="text" id="subs" name="subs" value=""></td></tr>
<tr><td colspan="2" align="center"><input type="submit" style="width:150px" class="btn btn--m btn--blue" value="Save"></td></tr>
</table>
</form>
<script>

function GetState()
{
	setValues("/admin/mqttvalues");
}

window.onload = function ()
{
	load("style.css","css", function() 
	{
		load("microajax.js","js", function() 
		{
				setValues("/admin/mqttvalues");
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}

</script>

)=====";

//
//  SEND HTML PAGE OR IF A FORM SUMBITTED VALUES, PROCESS THESE VALUES
// 

void send_mqtt_configuration_html()
{

    if (server.args() > 0)  // Save Settings
    {
        String temp = "";
        config.ota = false;
        for (uint8_t i = 0; i < server.args(); i++) {
            if (server.argName(i) == "main_t") config.mqtt_topic = urldecode(server.arg(i));
            if (server.argName(i) == "sub_t") config.mqtt_subtopic = urldecode(server.arg(i));
            if (server.argName(i) == "subs") config.mqtt_sub = urldecode(server.arg(i));
            if (server.argName(i) == "user") config.mqtt_user = urldecode(server.arg(i));
            if (server.argName(i) == "pass") config.mqtt_password = urldecode(server.arg(i));
            if (server.argName(i) == "server") config.mqtt_server_domain = urldecode(server.arg(i));
            if (server.argName(i) == "ip_0") if (checkRange(server.arg(i))) 	config.MqttIP[0] = server.arg(i).toInt();
            if (server.argName(i) == "ip_1") if (checkRange(server.arg(i))) 	config.MqttIP[1] = server.arg(i).toInt();
            if (server.argName(i) == "ip_2") if (checkRange(server.arg(i))) 	config.MqttIP[2] = server.arg(i).toInt();
            if (server.argName(i) == "ip_3") if (checkRange(server.arg(i))) 	config.MqttIP[3] = server.arg(i).toInt();
            if (server.argName(i) == "port") config.MqttPort = server.arg(i).toInt();
            if (server.argName(i) == "ota") config.ota = true;
        }
        WriteConfig();
        //firstStart = true;
        //ConfigureWifi();
        //AdminTimeOutCounter=0;
    }
    server.send(200, "text/html", PAGE_MQTTConfiguration);
    //ConfigureWifi();
    //Set_serverName();
    //Set_mqtt_server(false);
    //AdminTimeOutCounter=0;

    //Serial.println(__FUNCTION__);
}


void send_mqtt_configuration_values_html()
{

    String values = "";

    values += "main_t|" + (String)config.mqtt_topic + "|input\n";
    values += "sub_t|" + (String)config.mqtt_subtopic + "|input\n";
    values += "subs|" + (String)config.mqtt_sub + "|input\n";
    values += "ip_0|" + (String)config.MqttIP[0] + "|input\n";
    values += "ip_1|" + (String)config.MqttIP[1] + "|input\n";
    values += "ip_2|" + (String)config.MqttIP[2] + "|input\n";
    values += "ip_3|" + (String)config.MqttIP[3] + "|input\n";
    values += "port|" + (String)config.MqttPort + "|input\n";
    values += "user|" + (String)config.mqtt_user + "|input\n";
    values += "pass|" + (String)config.mqtt_password + "|input\n";
    values += "server|" + (String)config.mqtt_server_domain + "|input\n";
    values += "ota|" + (String)(config.ota ? "checked" : "") + "|chk\n";
    server.send(200, "text/plain", values);
    //Serial.println(__FUNCTION__);
}
