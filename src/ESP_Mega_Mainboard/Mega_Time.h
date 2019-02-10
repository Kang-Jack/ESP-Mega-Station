#ifndef MEGA_Time
#define MEGA_Time

#include <DS3231.h>

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
int ActivedTime = 0;

int getNowSeconds() {
    return Clock.getSecond();
}
int getNowHour() {
    return Clock.getHour(h12, PM);
}
int getNowMinute() {
    return Clock.getMinute();
}
void printTimeFormat() {
    Serial.println("Please enter the time: year[2]month[2]date[2]DoW[2]hour[2]minute[2]second[2]");
    Serial.println("example: 2014-12-3 Wednesday 14:15:15 enter:14120303141515");
}
void setDS3231() {
    Str_year[0] = data[0];
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
    year = atoi(Str_year);
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

void setTime() {
    if (Serial.available() >= 14)
    {
        for (int a = 0; a < 14; a++)
        {
            data[a] = Serial.read();
        }
        setDS3231();
    }
}
bool isWorkingTime(int hour) {
    if (hour > 1 & hour < 6) {
        return false;
    }
    return true;
}



#endif