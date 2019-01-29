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
    bool isNeedLight = needLightOn(nowHour);
    handleLight(nowSeconds, nowHour, isNeedLight);
    if (isWorkingTime(nowHour)) {
        if (nowSeconds % 60 == 0)
        {
            handleBME280();
            lcdHumAtm(bmeStatus);
            lcdTimeTemp(bmeStatus);
        }
    }
}
#endif
