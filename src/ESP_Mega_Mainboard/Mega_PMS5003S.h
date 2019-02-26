#ifndef MEGA_PMS5003S
#define MEGA_PMS5003S

#include "LASERK_PMS5003S.h"
int WakeMins = 0;
bool isWake = false;
int lastM = 0;
int lastH = 0;
LASERK_PMS5003S pms(&Serial3);

/*
# I = { (I(high) - I(low)) / C(high) - C(low) }*(C - C(low)) + I(low)
# C concentration
# I the(Air Quality) index
'''
US Standard
PM 2.5 (ug / m 3)                       AQI             Category
0.0 - 12.0   (24hr)                0 - 50           Good
12.1 - 35.4   (24hr)                51 - 100         Moderate
35.5 - 55.4   (24hr)                101 - 150        Unhealthy for sensutive groups
55.5 - 150.4  (24hr)                151 - 200        Unhealthy
150.5 - 250.4 (24hr)                201 - 300        Very Unhealth
250.5 - 350.4 (24hr)                301 - 400        Hazardous
350.5 - 500.4 (24hr)                401 - 500        Hazardous
'''
'''
US Standard
PM 10 (ug / m 3)                       AQI             Category
0 - 54   (24hr)                0 - 50           Good
55 - 154   (24hr)                51 - 100         Moderate
155 - 254  (24hr)                101 - 150        Unhealthy for sensutive groups
255 - 354  (24hr)                151 - 200        Unhealthy
355 - 424 (24hr)                201 - 300        Very Unhealth
425 - 504 (24hr)                301 - 400        Hazardous
505 - 604 (24hr)                401 - 500        Hazardous
'''
'''
CHN Standard HJ633-2012
PM 2.5 (ug/m 3)                       AQI             Category
0.0  - 35   (24hr)                  0 - 50           Excellent
35.1- 75   (24hr)                   51 - 100         Good
75.1 - 115   (24hr)                 101 - 150        Lightly
115.1 - 150  (24hr)                 151 - 200        Moderately
150.1 - 250 (24hr)                  201 - 300        Heavily
250.1 - 350 (24hr)                  301 - 400        Severely Polluted
350.1 - 500 (24hr)                  401 - 500        Severely Polluted
'''

'''
CHN Standard HJ633-2012
PM 1.0 (ug/m 3)                       AQI             Category
0.0  - 50   (24hr)                   0 - 50           Excellent
50.1- 150   (24hr)                  51 - 100         Good
150.1 - 250   (24hr)                101 - 150        Lightly
250.1 - 350  (24hr)                 151 - 200        Moderately
350.1 - 420 (24hr)                  201 - 300        Heavily
420.1 - 500 (24hr)                  301 - 400        Severely Polluted
500.1 - 600 (24hr)                  401 - 500        Severely Polluted
'''
*/
double us_pm2_5_category[7][4] = { {0.0, 12.0, 0, 50}, {12.1, 35.4, 51, 100},
{35.5, 55.4, 101, 150}, {55.5, 150.4, 151, 200},
{150.5, 250.4, 201, 300}, {250.5, 350.4, 301, 400}, {350.5, 500.4, 401, 500} };
double us_pm10_category[7][4] = { {0.0, 54.0, 0, 50}, {55.0, 154, 51, 100},
{155, 254, 101, 150}, {255, 354, 151, 200},
{355, 424, 201, 300}, {425, 504, 301, 400}, {505, 604, 401, 500} };

double chn_pm2_5_category[7][4] = { {0.0, 35, 0, 50}, {35.1, 75.0, 51, 100},
                      {75.1, 115, 101, 150}, {115.1, 150, 151, 200},
                      {150.1, 250, 201, 300}, {250.1, 350, 301, 400}, {350.1, 500, 401, 500} };
double chn_pm10_category[7][4] = { {0.0, 50, 0, 50}, {50.1, 150, 51, 100},
                     {150.1, 250, 101, 150}, {250.1, 350, 151, 200},
                     {350.1, 420, 201, 300}, {420.1, 500, 301, 400}, {500.1, 600, 401, 500} };

double caculator(const double* match_category, int iaqi) {
    return (match_category[3] - match_category[2]) / (match_category[1] - match_category[0])*(iaqi - match_category[0]) + match_category[2];
}

int us_pm2_5_aqi_caculator(int pm2_5_iaqi) {
    //clean char array
    memset(aqiString, 0, sizeof(aqiString));
    if (pm2_5_iaqi < us_pm2_5_category[0][1]) {
        snprintf(aqiString, sizeof(aqiString) - 1, "%s", "Good");
        return caculator(us_pm2_5_category[0], pm2_5_iaqi);
    }
    if (pm2_5_iaqi < us_pm2_5_category[1][1]) {
        snprintf(aqiString, sizeof(aqiString) - 1, "%s", "Lightly");
        return caculator(us_pm2_5_category[1], pm2_5_iaqi);
    }
    if (pm2_5_iaqi < us_pm2_5_category[2][1]) {
        snprintf(aqiString, sizeof(aqiString) - 1, "%s", "Moderately");
        return caculator(us_pm2_5_category[2], pm2_5_iaqi);
    }
    if (pm2_5_iaqi < us_pm2_5_category[3][1]) {
        snprintf(aqiString, sizeof(aqiString) - 1, "%s", "Unhealthy");
        return caculator(us_pm2_5_category[3], pm2_5_iaqi);
    }
    if (pm2_5_iaqi < us_pm2_5_category[4][1]) {
        snprintf(aqiString, sizeof(aqiString) - 1, "%s", "Heavily");
        return caculator(us_pm2_5_category[4], pm2_5_iaqi);
    }
    if (pm2_5_iaqi < us_pm2_5_category[5][1]) {
        snprintf(aqiString, sizeof(aqiString) - 1, "%s", "Hazardous");
        return caculator(us_pm2_5_category[5], pm2_5_iaqi);
    }
    if (pm2_5_iaqi < us_pm2_5_category[6][1]) {
        snprintf(aqiString, sizeof(aqiString) - 1, "%s", "Severely");
        return caculator(us_pm2_5_category[6], pm2_5_iaqi);
    }
    else {
        snprintf(aqiString, sizeof(aqiString) - 1, "%s", "Severely");
        return caculator(us_pm2_5_category[6], pm2_5_iaqi);
    }
}

int us_pm10_aqi_caculator(int pm10_iaqi) {
    //clean char array
    memset(aqiString, 0, sizeof(aqiString));
    if (pm10_iaqi < us_pm10_category[0][1]) {
        snprintf(aqiString, sizeof(aqiString) - 1, "%s", "Good");
        return caculator(us_pm10_category[0], pm10_iaqi);
    }
    if (pm10_iaqi < us_pm10_category[1][1]) {
        snprintf(aqiString, sizeof(aqiString) - 1, "%s", "Lightly");
        return caculator(us_pm10_category[1], pm10_iaqi);
    }
    if (pm10_iaqi < us_pm10_category[2][1]) {
        snprintf(aqiString, sizeof(aqiString) - 1, "%s", "Moderately");
        return caculator(us_pm10_category[2], pm10_iaqi);
    }
    if (pm10_iaqi < us_pm10_category[3][1]) {
        snprintf(aqiString, sizeof(aqiString) - 1, "%s", "Unhealthy");
        return caculator(us_pm10_category[3], pm10_iaqi);
    }
    if (pm10_iaqi < us_pm10_category[4][1]) {
        snprintf(aqiString, sizeof(aqiString) - 1, "%s", "Heavily");
        return caculator(us_pm10_category[4], pm10_iaqi);
    }
    if (pm10_iaqi < us_pm10_category[5][1]) {
        snprintf(aqiString, sizeof(aqiString) - 1, "%s", "Hazardous");
        return caculator(us_pm10_category[5], pm10_iaqi);
    }
    if (pm10_iaqi < us_pm10_category[6][1]) {
        snprintf(aqiString, sizeof(aqiString) - 1, "%s", "Severely");
        return caculator(us_pm10_category[6], pm10_iaqi);
    }
    else {
        snprintf(aqiString, sizeof(aqiString) - 1, "%s", "Severely");
        return caculator(us_pm10_category[6], pm10_iaqi);
    }
}

void setupPMS5003S() {
    Serial3.begin(9600);
    pms.begin();
    //pms.setMode(ACTIVE);
    pms.setMode(PASSIVE);
    pms.wakeUp();
    delay(500);
    isWake = true;
    WakeMins = 0;
}

void wake(int pms_mins) {
    Serial2.print("pms wake");
    isWake = true;
    pms.wakeUp();
    delay(500);
    WakeMins = pms_mins;
}
void sleep(int pms_mins) {
    Serial2.print("pms sleep");
    isWake = false;
    pms.sleep();
    delay(500);
    WakeMins = pms_mins;
}
void logPMS(int aqi2_5, int aqi10) {
    char logMesg[100];
    memset(float_str_temp, 0, sizeof(float_str_temp));
    dtostrf(pmsForm, 4, 3, float_str_temp);
    snprintf(logMesg, sizeof(logMesg), "(%s) HCHO:%s AQI2_5:%i AQI10:%i Level:%s: PM2.5:%i PM10:%i PM1.0:%i", aqiTime, float_str_temp, aqi2_5, aqi10, aqiString, pmsAto2_5, pmsAto10, pmsAto1);
    logging2SD(logMesg);
}
void pmsPlayload() {
    turnOff();
    memset(float_str_temp, 0, sizeof(float_str_temp));
    dtostrf(pmsForm, 4, 3, float_str_temp);
    memset(playload, 0, sizeof(playload));
    snprintf(playload, sizeof(playload), "{A:%i,2_5:%i,10_:%i,1_0:%i,H:%s}", aqi, pmsAto2_5, pmsAto10, pmsAto1, float_str_temp);
    Serial2.print((char*)playload);
}
void handlePMS5003S(int pms_min, int pms_hour) {
    if (WakeMins == 0 && isWake == true) {
        WakeMins = pms_min;
    }
    else if (WakeMins > 52 && isWake == true) {
        if (WakeMins - pms_min > 0)
            WakeMins = pms_min;
    }
    else {
        if (pms_min - WakeMins > 6 && isWake == true) {
            sleep(pms_min);
            delay(1000);
            pmsPlayload();// very hour send to mqtt server once after pms sleep
        }
        else if (pms_min <3 && isWake == false) {
            wake(pms_min);
        }
    }

    if (isWake) {
        pms.request();
        if (!pms.read()) {
            return;
        }
        snprintf(aqiTime, sizeof(aqiTime), "%i:%i", pms_hour, pms_min);
        pmsAto1 = pms.getPmAto(1.0);
        pmsAto2_5 = pms.getPmAto(2.5);
        pmsAto10 = pms.getPmAto(10.0);
        pmsForm = pms.getForm();
        int aqi2_5;
        int aqi10;
        aqi2_5 = us_pm2_5_aqi_caculator(pmsAto2_5);
        aqi10 = us_pm10_aqi_caculator(pmsAto10);
        // aqi = Max {aqi2_5,aqi10}
        if (aqi2_5 >= aqi10)
            aqi = us_pm2_5_aqi_caculator(pmsAto2_5);
        else
            aqi = us_pm10_aqi_caculator(pmsAto10);

        if (lastH != pms_hour || lastM != pms_min) {
            lastH = pms_hour;
            lastM = pms_min;
            logPMS(aqi2_5, aqi10);
        }
    }
}

#endif