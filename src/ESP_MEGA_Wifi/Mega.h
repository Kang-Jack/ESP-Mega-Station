#ifndef MEGA_H
#define MEGA_H

void SyncWithValidNPT() {
    if (DateTime.year > 2010) {
        delay(3000);
        char timego[21];
        //"example: 2014-12-3 Wednesday 14:15:15 cmd:'timego14120303141515'"
        String time_cmd = "timego";
        int int_Y = DateTime.year - 2000;
        sprintf(timego, "%s%i%02u%02u%02u%02u%02u%02u", time_cmd.c_str(), int_Y, DateTime.month, DateTime.day, DateTime.wday, DateTime.hour, DateTime.minute, DateTime.second);
        Serial.println(timego);
        delay(3000);
    }
}
#endif