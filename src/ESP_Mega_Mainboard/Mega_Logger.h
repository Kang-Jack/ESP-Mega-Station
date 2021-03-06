#ifndef MEGA_Logger
#define MEGA_Logger
#include <SPI.h>
#include <SD.h>

File myFile;
bool isSDReady = false;
bool isSDEnable = false;
void setupLogger() {
    if (!SD.begin(4)) {
        Serial.println("SD failed!");
        isSDReady = false;
    }
    else
    {
        isSDReady = true;
        Serial.println("SD done");
    }
    if (SD.exists("log.txt")) {
        SD.remove("log.txt");
        Serial.println("log removed.");
    }

    Serial.println("Creating...");
    myFile = SD.open("log.txt", FILE_WRITE);
    myFile.close();
}
void disableLogger() {
    isSDEnable = false;
}
void enableLogger() {
    isSDEnable = true;
    if (!isSDReady) {
        setupLogger();
    }
}
void logging2SD(const char* message) {
    if (isSDReady && isSDEnable)
    {
        myFile = SD.open("log.txt", FILE_WRITE);

        if (myFile) {
            myFile.println(message);
            // close the file:
            myFile.close();
            Serial.println("logging done.");
        }
        else {
            // if the file didn't open, print an error:
            Serial.println("error logging");
        }
    }
}
#endif