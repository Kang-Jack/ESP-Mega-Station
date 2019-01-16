#include <Wire.h> 


#include <DS3231.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <LiquidCrystal_I2C.h>

#define DispDelay 5000        //displayTime

DS3231 Clock;

bool Century = false;
bool h12 = false;
bool PM;
byte ADay, AHour, AMinute, ASecond, ABits;
bool ADy, A12h, Apm;
byte year, month, date, DoW, hour, minute, second;
char data[15];
char Str_year[3], Str_month[3], Str_date[3], Str_DoW[3], Str_hour[3], Str_minute[3], Str_second[3];
//uint8_t bell[8] = { 0x4,0xe,0xe,0xe,0x1f,0x0,0x4 };

Adafruit_BME280 bme; // I2C
bool bmeStatus = false; 
float hum;    // Stores humidity value in percent
float temp;   // Stores temperature value in Celcius
float pres;

LiquidCrystal_I2C lcd(0x3f,20,4);  // set the LCD address to 0x3f for a 16 chars and 2 line display
bool lightStatus = false;
char emptyLine[40]="                    ";
const int max_length=40;
char msg[max_length];   // initialise storage buffer (i haven't tested to this capacity.)

void setup()
{
  bmeStatus = bme.begin(0x76);
  lcd.init();
  lcd.noAutoscroll();
  // Print a message to the LCD.
  lcdStartScreen();
  lcd.clear();
  lcdFirstLine();
  Serial.begin(9600);
  Serial2.begin(9600);

  Serial.println("Please enter the time: year[2]month[2]date[2]DoW[2]hour[2]minute[2]second[2]");
  Serial.println("example: 2014-12-3 Wednesday 14:15:15 enter:14120303141515");
}

void loop()
{
  setTime();
    delay(500);
    int hour = Clock.getHour(h12, PM);
    if (hour > 1 & hour < 6){
        if (lightStatus==true){
          lcd.noBacklight();
          lightStatus =false;
       }
    }
    else{
       if (lightStatus==false){
          lcd.backlight();
          lightStatus =true;
       }
       if (Clock.getSecond()%60 == 0)
       {
          handleBME280();
          displayHumAtm(bmeStatus);
          displayTimeTemp(bmeStatus);
       }
    }   
    if(msg[0]!='\0'){ 
      Serial.print((char*)msg);
      lcdCleanLine(0);
      lcd.setCursor(0,0);
      lcd.print(msg);
      /*
      String msgString((char*)msg);
      if (msgString == "red") color(255, 0, 0); // red
      if (msgString == "blue") color(0, 0, 255); // blue  
      if (msgString == "green") color(0,255, 0); //green
      if (msgString == "yellow") color(255,255,0); // yellow  
      if (msgString == "purple") color(128,0,255); // purple 
      if (msgString == "close") color(0,0,0); // close 
      */
      Serial2.print("received");
      msg[0] ='\0';
    }
    if (msg[0]!='\0') color(255,255,255); // white  
    listen_esp();
    talk_esp();
    delay(100);
}
void listen_esp()
{
    int i=0;
    while(Serial2.available() > 0) 
    {
      char a = Serial2.read();
      if(a == '\0')
        continue;
      if(a != '\r' && a != '\n' && (a < 32))
        continue;
      if (i < max_length) msg[i] = a;

      Serial.print(a);
      i++;
    }
    if (i<max_length) msg[i] = '\0';
    else  msg[max_length-1] = '\0';
}

void talk_esp()
{
    while(Serial.available() > 0)
    {
      char a = Serial.read();
      Serial.write(a);
      Serial2.write(a);
    }
 }
 
void color (unsigned char red, unsigned char green, unsigned char blue) 
{
  lcd.setCursor(3,0);
}
void lcdFirstLine()
{
  if(msg[0]!='\0')
  {
    lcd.setCursor(0,0);
    lcd.print("Waitting for Esp12 connecting");
  }
  else {
    lcd.setCursor(0,0);
    lcd.print("Esp12 connected");
  }
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
    lcd.setCursor(3,0);// col ,row start from 0
    lcd.print(line0);
    lcd.setCursor(2,1);
    lcd.print(line1);
    lcd.setCursor(0,2);
    lcd.print(line2);
    lcd.setCursor(2,3);
    lcd.print(line3);
    delay (DispDelay);
}

void handleBME280() {
    hum = bme.readHumidity();  // Get Humidity value
    temp = bme.readTemperature();  // Get Temperature value
    //char str_temp[6];
    /* 4 is mininum width, 2 is precision; float value is copied onto str_temp*/
    //dtostrf(temp, 4, 2, str_temp);
    pres = bme.readPressure()/ 100.0F;
  }
void displayHumAtm(bool bemStatus)
{
  char line3[40];
  if (bemStatus){
    Serial.println("hum:");
    Serial.println(hum);
    Serial.println("pres:");
    Serial.println(pres);
    sprintf(line3, "H:%i%% ATM:%ihPa",(int)hum,(int)pres);
  }
  else
    sprintf(line3, "Not find BME280 sensor");
    lcdCleanLine(3);
    lcd.setCursor(0, 3);
    lcd.print(line3);
}
void lcdCleanLine(int row){
  lcd.setCursor(0, row);
  lcd.print(emptyLine);
}
void setTime() {
    if (Serial.available() >= 14)     //串口读取数据
    {
        for (int a = 0; a < 14; a++)
        {
            data[a] = Serial.read();
        }
        Str_year[0] = data[0];    //拆包
        Str_year[1] = data[1];
        Str_month[0] = data[2];
        Str_month[1] = data[3];
        Str_date[0] = data[4];
        Str_date[1] = data[5];
        Str_DoW[0] = data[6];
        Str_DoW[1] = data[7];
        Str_hour[0] = data[8];
        Str_hour[1] = data[9];
        Str_minute[0] = data[10];
        Str_minute[1] = data[11];
        Str_second[0] = data[12];
        Str_second[1] = data[13];

        //Str to byte
        year = atoi(Str_year);    //转换数据类型
        month = atoi(Str_month);
        date = atoi(Str_date);
        DoW = atoi(Str_DoW);
        hour = atoi(Str_hour);
        minute = atoi(Str_minute);
        second = atoi(Str_second);

        Clock.setSecond(second);//Set the second 
        Clock.setMinute(minute);//Set the minute 
        Clock.setHour(hour);  //Set the hour 
        Clock.setDoW(DoW);    //Set the day of the week
        Clock.setDate(date);  //Set the date of the month
        Clock.setMonth(month);  //Set the month of the year
        Clock.setYear(year);  //Set the year (Last two digits of the year)     
    }
}

void displayTimeTemp(bool bemStatus) {
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
    lcd.print("20");  // 显示20世纪
    if (year >= 10)  // 显示年份
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
    if (month >= 10)  // 显示月份
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
    if (date >= 10)  // 显示日期
    {
        lcd.print(date, DEC);
    }
    else
    {
        lcd.print("0");
        lcd.print(date, DEC);
    }

    lcd.setCursor(11, 1);
    switch (dow)  // 显示星期
    {
    case 1:  // 当dow等于1时，执行以下语句
        lcd.print("Mon");
        break;
    case 2:  // 当dow等于2时，执行以下语句
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

    lcd.setCursor(0, 2);  //光标移至第2行
    if (hour >= 10)  // 显示小时
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
    if (minute >= 10)  // 显示分钟
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
    if (second >= 10)  // 显示秒钟
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
    if (Clock.checkAlarmEnabled(1))  // 显示闹钟标识
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
