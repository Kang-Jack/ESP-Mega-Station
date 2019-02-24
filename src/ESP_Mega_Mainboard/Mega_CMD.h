#ifndef MEGA_CMD
#define MEGA_CMD

bool needLightOn(int nowH) {
    if (!isWorkingTime(nowH)) {
        return false;
    }
    return isAnyOneHere();
}

void handleESPCmd()
{
    if (msg[0] != '\0') {
        Serial.print((char*)msg);
        lcdCmdEcho(msg);

        String msgString((char*)msg);
        if (msgString == "dark") {
            turnOff();
        }
        if (msgString == "light") {
            turnOn();
        }
        if (msgString == "bme") {
            handleBME280(60,0,8);
            bmePlayload();
            lcdBMEInfo();
        }
        if (msgString == "pms") {
            handlePMS5003S(0, 0);
            pmsPlayload();
            lcdPMSInfo();
        }
        if (msgString == "disable_log") {
            disableLogger();
        }
        if (msgString == "enable_log") {
            enableLogger();
        }
        if (msgString.startsWith("timego")) {
            for (int a = 0; a < 14; a++)
            {
                data[a] = msg[a + 6];
            }
            setDS3231();
        }
        Serial2.print("received");
        memset(msg, 0, sizeof(msg));
        msg[0] = '\0';
    }
}

void majorWorkOnCondition() {
    int nowSeconds = getNowSeconds();
    int nowHour = getNowHour();
    int nowMins = getNowMinute();
    bool isNeedLight = needLightOn(nowHour);
    handleLight(nowSeconds, nowHour, isNeedLight);
    handlePMS5003S(nowMins, nowHour);
    handleBME280(nowSeconds, nowMins, nowHour);
    handleDisplay(nowSeconds, nowHour, bmeStatus);
}
#endif
