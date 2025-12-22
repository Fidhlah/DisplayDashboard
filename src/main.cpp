#include <Arduino.h>
#include <DHT.h>
#include <WiFi.h>
#include "include/Timer.h"
#include "include/DebugMacros.h"
#include "time.h"
#include <Wire.h>
#include <SPI.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "include/DisplayManager.h"
#include "include/LogicManager.h"

// ------------- WiFI -------------------------------------------------------------------------------------------------
const char *ssid_wifi = "RumahBapa_3";
const char *pw_wifi = "kurupukseblak";

// ------------- DHT 22 -----------------------------------------------------------------------------------------------
#define DHT_PIN 14
DHT dht(DHT_PIN, DHT22);
Timer printDHT(10000);
float temperature = 0.0;
float humidity = 0.0;

// ------------- Clock -----------------------------------------------------------------------------------------------
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7 * 3600;
const int daylightOffset_sec = 0; // Indonesia tidak pakai Daylight Saving
Timer printClock(1000);
bool showColon = true; 

// ------------- Dot Matrix -----------------------------------------------------------------------------------------------
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 5
MD_Parola Dmatrix = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// ------------- OLED Display -----------------------------------------------------------------------------------------------
Adafruit_SH1106 display(-1); 
Timer breathEffect(80);
Timer updateTrend(30000);

// Variabel animasi
int animFrame = 0;
unsigned long lastUpdate = 0;
int breatheOffset = 0;
bool breatheUp = true;


// Data sebelumnya untuk trend
float prevTemperature = 0.0;
float prevHumidity = 0.0;
unsigned long lastTrendUpdate = 0;
const unsigned long TREND_INTERVAL = 30000;


unsigned long onhowlong=0; 

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
  
  // Setup Oled
  display.begin(SH1106_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();
  display.display();

  // Setup Dot Matrix
  Dmatrix.begin();
  Dmatrix.setTextAlignment(PA_CENTER); 
  Dmatrix.setIntensity(0);             
  Dmatrix.displayClear();
  
  initWiFi(ssid_wifi, pw_wifi, Dmatrix);
  delay(1000);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  Dmatrix.displayClear();
  
  delay(500);
}

void loop(){
  int currentTouch = touchRead(4);
  unsigned long currentTime = millis();

  if (printDHT.isReady()) {
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    // Update trend secara rasionil di sini
    prevHumidity = humidity;
    prevTemperature = temperature;
  }

  // 2. VISUAL RENDERING (Tiap 80ms) - SATU-SATUNYA TEMPAT UNTUK OLED
  if (breathEffect.isReady()) {
    // Logika animasi
    if (breatheUp) {
      breatheOffset++;
      if (breatheOffset >= 3) breatheUp = false;
    } else {
      breatheOffset--;
      if (breatheOffset <= 0) breatheUp = true;
    }
    animFrame++;

    // PROSES DRAWING (Wajib di dalam sini!)
    display.clearDisplay(); 
    display.setTextColor(WHITE);
    
    drawDecorativeDots(display, animFrame);
    drawTemperatureSection(display, prevTemperature, temperature, breatheOffset);
    drawHumiditySection(display, prevHumidity, humidity, breatheOffset);
    
    display.display(); // Tampilkan HANYA setelah selesai menggambar
  }

  // 3. DOT MATRIX (Tiap 1 detik)
  if (printClock.isReady()) {
    showColon = !showColon;
    printLocalTime(Dmatrix, showColon);
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
