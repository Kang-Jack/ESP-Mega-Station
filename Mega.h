#ifndef MEGA_H
#define MEGA_H
boolean needSyncTime = true;
void SyncWithValidNPT() {
    if (needSyncTime)
        if (DateTime.year > 2018) {
            delay(3000);
            char timego[21];
            //"example: 2014-12-3 Wednesday 14:15:15 cmd:'timego14120303141515'"
            String time_cmd = "timego";
            int int_Y = DateTime.year - 2000;
            snprintf(timego, sizeof(timego),"%s%i%02u%02u%02u%02u%02u%02u", time_cmd.c_str(), int_Y, DateTime.month, DateTime.day, DateTime.wday, DateTime.hour, DateTime.minute, DateTime.second);
            Serial.println(timego);
            needSyncTime = false;
            delay(3000);
        }
}
#endif