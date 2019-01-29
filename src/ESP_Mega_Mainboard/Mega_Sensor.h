#ifndef MEGA_Sensor
#define MEGA_Sensor

Adafruit_BME280 bme; // I2C
bool bmeStatus = false;

int ripPin = 8;               // choose the input pin (for PIR sensor)
int ripVal = LOW;                    // variable for reading the pin status


void setupSensor() {
    pinMode(ripPin, INPUT);
    bmeStatus = bme.begin(0x76);
}

void handleBME280() {
    hum = bme.readHumidity();  // Get Humidity value
    temp = bme.readTemperature();  // Get Temperature value
    //char str_temp[6];
    /* 4 is mininum width, 2 is precision; float value is copied onto str_temp*/
    //dtostrf(temp, 4, 2, str_temp);
    pres = bme.readPressure() / 100.0F;
}

bool isAnyOneHere() {
    ripVal = digitalRead(ripPin);  // read input value
    if (ripVal == LOW) {
        return false;
    }
    return true;
}
#endif