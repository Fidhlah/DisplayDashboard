#ifndef LOGIC_MANAGER_H
#define LOGIC_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <DHT.h>
#include "DebugMacros.h"
#include <MD_Parola.h>
#include <MD_MAX72xx.h>

void initWiFi(const char* ssid, const char* pass, MD_Parola &matrix) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    debugPrintf("Connecting to %s\n", ssid);
    String loading = ".";
    while (WiFi.status() != WL_CONNECTED){
        debugPrint('.');
        matrix.print(loading);
        loading += ".";

        delay(1000);
    }
    debugPrintln("");
    debugPrintln(WiFi.RSSI());
    debugPrintln("");
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
// void syncNTP()




#endif