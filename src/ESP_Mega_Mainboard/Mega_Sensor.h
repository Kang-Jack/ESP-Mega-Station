#ifndef MEGA_Sensor
#define MEGA_Sensor

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; // I2C
bool bmeStatus = false;

int ripPin = 8;               // choose the input pin (for PIR sensor)
int ripVal = LOW;                    // variable for reading the pin status


void setupSensor() {
    pinMode(ripPin, INPUT);
    bmeStatus = bme.begin(0x76);
}

void bmePlayload() {
    turnOff();
    memset(float_str_temp, 0, sizeof(float_str_temp));
    dtostrf(temp, 4, 1, float_str_temp);
    memset(playload, 0, sizeof(playload));
    snprintf(playload, sizeof(playload), "{Temp:%s,Humi:%i,Pres:%i}", float_str_temp, (int)hum, (int)pres);
    Serial2.print((char*)playload);
}

void handleBME280(int pms_sec, int pms_min, int pms_hour) {
    if (isWorkingTime(pms_hour)) {
        if (pms_sec % 60 == 0)
        {
            hum = bme.readHumidity();  // Get Humidity value
            temp = bme.readTemperature();  // Get Temperature value
            //char str_temp[6];
            /* 4 is mininum width, 2 is precision; float value is copied onto str_temp*/
            //dtostrf(temp, 4, 2, str_temp);
            pres = bme.readPressure() / 100.0F;
        }
    }
    if (pms_min == 25 && pms_sec<2) {
        bmePlayload();// mqtt sever every hour
    }
}

bool isAnyOneHere() {
    ripVal = digitalRead(ripPin);  // read input value
    if (ripVal == LOW) {
        return false;
    }
    return true;
}
#endif