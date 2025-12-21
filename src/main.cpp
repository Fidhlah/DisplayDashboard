#include <Arduino.h>
#include <DHT.h>
#include <WiFi.h>
#include "include/Timer.h"
#include "include/DebugMacros.h"
#include "time.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// ------------- WiFI -------------------------------------------------------------------------------------------------
const char *ssid_wifi = "RumahBapa_3";
const char *pw_wifi = "kurupukseblak";
void initWiFi();

// ------------- DHT 22 -----------------------------------------------------------------------------------------------
#define DHT_PIN 14
DHT dht(DHT_PIN, DHT22);
Timer printDHT(10000);

// ------------- Clock -----------------------------------------------------------------------------------------------
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7 * 3600;
const int daylightOffset_sec = 0; // Indonesia tidak pakai Daylight Saving
void printLocalTime();
Timer printClock(1000);
bool showColon = true; 

// ------------- Dot Matrix -----------------------------------------------------------------------------------------------
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 5
MD_Parola Dmatrix = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// ------------- Touch Sensor -----------------------------------------------------------------------------------------------
bool touched = false;
int thresholdTouched = 35;
int thresholdReleased = 50;
Timer printTouch(1000);
unsigned long howlongtouched = 0;



// Start
void setup(){
  debugBegin(115200);
  dht.begin();
  Dmatrix.begin();
  Dmatrix.setTextAlignment(PA_CENTER); 
  Dmatrix.setIntensity(0);             
  Dmatrix.displayClear();
  Dmatrix.print("Wait!");
  initWiFi();
  
  // Clock
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  Dmatrix.displayClear();
  delay(1000);
}

void loop(){
  int currentTouch = touchRead(4);

  if (printClock.isReady()){
    showColon = (showColon == false) ? true : false;
    printLocalTime();
  }

  if (printDHT.isReady()){
    float humid = dht.readHumidity();
    float temp = dht.readTemperature();

    if (isnan(humid) || isnan(temp)){
      debugPrintln("Gagal baca sensor");
    }
    else{
      debugPrintf("Humidity: %f | Temperature: %f\n", humid, temp);
    }
  }

  if(currentTouch  < thresholdTouched && touched== false){
    debugPrintf("Disentuh! ");
    touched = true;
    howlongtouched = millis();
  }

  if(currentTouch  > thresholdReleased && touched == true) {
    debugPrintf("Dilepas\n");
    touched = false;
    debugPrint("Disentuh selama: ");
    debugPrintln(millis() - howlongtouched);
  }
}

void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid_wifi, pw_wifi);
  debugPrintf("Connecting to %s\n", ssid_wifi);
  while (WiFi.status() != WL_CONNECTED)
  {
    debugPrint('.');
    delay(1000);
  }
  debugPrintln("");
}

void printLocalTime()
{
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)){
    Dmatrix.print("Failed!");
    debugPrintln("Failed!");
    return;
  }
  char timeString[10];
  if (showColon) {
    // Menampilkan Menit dan Detik dengan pemisah titik dua
    sprintf(timeString, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
  } else {
    // Menghilangkan titik dua (diganti spasi) agar terlihat kedip
    sprintf(timeString, "%02d %02d", timeinfo.tm_hour, timeinfo.tm_min);
  }

  // 3. Update Tampilan ke Dot Matrix
  Dmatrix.print(timeString);
  
}