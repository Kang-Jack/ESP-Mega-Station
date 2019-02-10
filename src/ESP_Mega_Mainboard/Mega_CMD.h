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
        lcdCleanLine(0);
        lcd.setCursor(0, 0);
        lcd.print(msg);

        String msgString((char*)msg);
        if (msgString == "dark") {
            turnOff();
        }
        if (msgString == "light") {
            turnOn();
        }
        if (msgString == "bme") {
            handleBME280();
            lcdBMEInfo();
        }
        if (msgString == "pms") {
            handlePMS5003S(0);
            lcdPMSInfo();
        }
        //int index = msgString.indexOf("time:");
        if (msgString.startsWith("timego")) {
            for (int a = 0; a < 14; a++)
            {
                data[a] = msg[a + 6];
            }
            setDS3231();
        }
        Serial2.print("received");
        msg[0] = '\0';
    }
}

void majorWorkOnCondition() {
    int nowSeconds = getNowSeconds();
    int nowHour = getNowHour();
    int nowMins = getNowMinute();
    bool isNeedLight = needLightOn(nowHour);
    handleLight(nowSeconds, nowHour, isNeedLight);
    if (isWorkingTime(nowHour)) {
        if (nowSeconds % 60 == 0)
        {
            Serial.println("time");
            Serial.println(nowSeconds);

            handleBME280();
            lcdHumAtm(bmeStatus);
            lcdTimeTemp(bmeStatus);

        }
        handlePMS5003S(nowMins);
        if (nowSeconds % 60 == 30)
        {
            Serial.println("pms");
            Serial.println(nowSeconds);
            lcdPMSInfo();
        }
    }
}
#endif
