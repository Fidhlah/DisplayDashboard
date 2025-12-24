#ifndef LOGIC_MANAGER_H
#define LOGIC_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <DHT.h>
#include "DebugMacros.h"
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <RTClib.h>

void initWiFi(const char* ssid, const char* pass, MD_Parola &matrix) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    debugPrintf("Connecting to %s\n", ssid);

    matrix.getGraphicObject()->setPoint(7, 0, true);
    unsigned long startAttempt = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 15000){
        yield();
    }
    matrix.getGraphicObject()->setPoint(7, 0, false);

    if (WiFi.status() == WL_CONNECTED) {
        debugPrintf("Connected! %s\n", WiFi.RSSI());
    } else {
        debugPrintln("WiFi Timeout. Running on RTC only.");
    }
}

bool isInternetAvailable() {
    IPAddress remote_ip;
    // Mencoba melakukan DNS lookup ke google.com
    // Jika berhasil (return 1), berarti internet tersedia.
    if (WiFi.hostByName("google.com", remote_ip)) {
        return true;
    }
    return false;
}
bool syncNtpToRtc(RTC_DS3231 rtc){
    struct tm timeinfo;
    unsigned long startWait = millis();
    
    debugPrint("Waiting for NTP sync...");

    // Tunggu maksimal 5 detik, tapi JANGAN membuat jam freeze
    while (!getLocalTime(&timeinfo) && millis() - startWait < 5000) {

        yield(); 
    }

    if (getLocalTime(&timeinfo)) {
        rtc.adjust(DateTime(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, 
                            timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec));
        debugPrintln("\nSYNC SUCCESS!");
        return true;
    }
    
    debugPrintln("\nSYNC TIMEOUT!");
    return false;
}



#endif