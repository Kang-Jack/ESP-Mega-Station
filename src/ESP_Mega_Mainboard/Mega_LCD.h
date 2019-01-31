#ifndef MEGA_LCD
#define MEGA_LCD

#include <LiquidCrystal_I2C.h>
#define DispDelay 5000        //displayTime
#define ActiveMins 3        //displayTime

LiquidCrystal_I2C lcd(0x3f, 20, 4);  // set the LCD address to 0x3f for a 16 chars and 2 line display
bool lightStatus = false;
char emptyLine[40] = "                    ";

void lcdCleanLine(int row) {
    lcd.setCursor(0, row);
    lcd.print(emptyLine);
}

void turnOn() {
    if (lightStatus == false) {
        lcd.backlight();
        lightStatus = true;
        Serial.print("turn on light");
        ActivedTime = 0;
    }
}

void turnOff() {
    if (lightStatus == true) {
        lcd.noBacklight();
        lightStatus = false;
        Serial.print("turn off light");
        ActivedTime = 0;
    }
}

void lcdFirstLine()
{
    if (msg[0] != '\0')
    {
        lcd.setCursor(0, 0);
        lcd.print("Waitting for Esp12 connecting");
    }
    else {
        lcd.setCursor(0, 0);
        lcd.print("Esp12 connected");
    }
}

void lcdBMEInfo() {
    lcd.clear();
    if (lightStatus == false) {
        lcd.backlight();
        lightStatus = true;
        Serial.print("turn on light");
        ActivedTime = 0;
    }
    char line1[40];
    char line2[40];
    char line3[40];
    char str_temp[7];
    /* 4 is mininum width, 2 is precision; float value is copied onto str_temp*/
    dtostrf(temp, 4, 2, str_temp);
    sprintf(line1, "Temperature: %s", str_temp);
    dtostrf(hum, 3, 2, str_temp);
    sprintf(line2, "Humidity: %s%%", str_temp);
    dtostrf(pres, 5, 2, str_temp);
    sprintf(line3, "Pressure: %shPa", str_temp);
    lcd.setCursor(0, 1);
    lcd.print(line1);
    lcd.write(0xdf);
    lcd.print("C");
    lcd.setCursor(0, 2);
    lcd.print(line2);
    lcd.setCursor(0, 3);
    lcd.print(line3);
}

void lcdStartScreen()
{
    char line0[40];
    char line1[40];
    char line2[40];
    char line3[40];
    sprintf(line0, "Hello, world!");
    sprintf(line1, "ESP Arduino Mega!");
    sprintf(line2, "Arduino wifi system");
    sprintf(line3, "Power By Laserk");
    lcd.clear();
    lcd.backlight();//noBacklight()
    lcd.setCursor(3, 0);// col ,row start from 0
    lcd.print(line0);
    lcd.setCursor(2, 1);
    lcd.print(line1);
    lcd.setCursor(0, 2);
    lcd.print(line2);
    lcd.setCursor(2, 3);
    lcd.print(line3);
    delay(DispDelay);
}

void lcdHumAtm(bool bemStatus)
{
    char line3[40];
    if (bemStatus) {
        /*
        Serial.println("hum:");
        Serial.println(hum);
        Serial.println("pres:");
        Serial.println(pres);
        */
        sprintf(line3, "H:%i%% ATM:%ihPa", (int)hum, (int)pres);
    }
    else
        sprintf(line3, "Not find BME280 sensor");
    lcdCleanLine(3);
    lcd.setCursor(0, 3);
    lcd.print(line3);
}

void lcdTimeTemp(bool bemStatus) {
    lcdCleanLine(1);
    lcdCleanLine(2);
    int second, minute, hour, date, month, dow, year, temperature;
    second = Clock.getSecond();
    minute = Clock.getMinute();
    hour = Clock.getHour(h12, PM);
    date = Clock.getDate();
    month = Clock.getMonth(Century);
    year = Clock.getYear();
    dow = Clock.getDoW();
    //temperature = Clock.getTemperature();

    Serial.print("20");
    Serial.print(year, DEC);
    Serial.print('-');
    Serial.print(month, DEC);
    Serial.print('-');
    Serial.print(date, DEC);
    Serial.print(' ');
    Serial.print(hour, DEC);
    Serial.print(':');
    Serial.print(minute, DEC);
    Serial.print(':');
    Serial.print(second, DEC);
    Serial.print('\n');
    //Serial.print("Temperature=");
    //Serial.print(temperature);
    //Serial.print('\n');

    lcd.setCursor(0, 1);
    lcd.print("20");
    if (year >= 10)
    {
        lcd.print(year, DEC);
    }
    else
    {
        lcd.print("0");
        lcd.print(year, DEC);
    }
    lcd.print('-');

    lcd.setCursor(5, 1);
    if (month >= 10)
    {
        lcd.print(month, DEC);
    }
    else
    {
        lcd.print("0");
        lcd.print(month, DEC);
    }
    lcd.print('-');

    lcd.setCursor(8, 1);
    if (date >= 10)
    {
        lcd.print(date, DEC);
    }
    else
    {
        lcd.print("0");
        lcd.print(date, DEC);
    }

    lcd.setCursor(11, 1);
    switch (dow)
    {
    case 1:
        lcd.print("Mon");
        break;
    case 2:
        lcd.print("Tue");
        break;
    case 3:
        lcd.print("Wed");
        break;
    case 4:
        lcd.print("Thu");
        break;
    case 5:
        lcd.print("Fri");
        break;
    case 6:
        lcd.print("Sat");
        break;
    case 7:
        lcd.print("Sun");
        break;
    }

    lcd.setCursor(0, 2);
    if (hour >= 10)
    {
        lcd.print(hour, DEC);
    }
    else
    {
        lcd.print("0");
        lcd.print(hour, DEC);
    }
    lcd.print(':');

    lcd.setCursor(3, 2);
    if (minute >= 10)
    {
        lcd.print(minute, DEC);
    }
    else
    {
        lcd.print("0");
        lcd.print(minute, DEC);
    }
    /*
  lcd.print(':');

  lcd.setCursor(6, 2);
  if (second >= 10)
  {
      lcd.print(second, DEC);
  }
  else
  {
      lcd.print("0");
      lcd.print(second, DEC);
  }
  */
    lcd.setCursor(5, 2);
    if (Clock.checkAlarmEnabled(1))
    {
        lcd.write(0x00);
    }
    else
    {
        lcd.print(' ');
    }
    if (bemStatus)
    {
        Serial.println("temp:");
        Serial.println(temp);
        lcd.setCursor(8, 2);
        lcd.print("Temp:");
        lcd.print(temp);
        lcd.write(0xdf);
        lcd.print("C");
    }

}

void setupLcd() {
    lcd.init();
    lcd.noAutoscroll();
    lcdStartScreen();
    lcd.clear();
    lcdFirstLine();
}


void handleLight(int nowS, int nowH, bool isNeedLight) {
    if (lightStatus == true) {
        if (nowS % 60 == 0){
            ActivedTime = ActivedTime + 1;
        }
    }
    if ((ActivedTime < ActiveMins) && lightStatus == true)
        return;
    if (!isNeedLight) {
        turnOff();
    }
    else {
        turnOn();
    }
}



#endif