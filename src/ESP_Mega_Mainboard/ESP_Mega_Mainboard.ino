#include <Wire.h> 



#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "datad.h"
#include "MEGA_Time.h"
#include "Mega_LCD.h"
#include "Mega_ESP.h"
#include "Mega_Sensor.h"
#include "Mega_CMD.h"
void setup()
{
    setupSensor();
    setupLcd();
    Serial.begin(9600);
    Serial2.begin(115200);
    printTimeFormat();
}

void loop()
{
    setTime();
    delay(500);
    majorWorkOnCondition();
    handleESPCmd();
    listen_esp();
    talk_esp();
    delay(100);
}
