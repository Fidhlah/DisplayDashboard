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
#include <RTClib.h>

// ------------- WiFI -------------------------------------------------------------------------------------------------
const char *ssid_wifi = "RumahBapa_3";
const char *pw_wifi = "kurupukseblak";

// ************** Temp & Humid ********************************************************************************************

// Prev data for trend
float prevTemperature = 0.0;
float prevHumidity = 0.0;
unsigned long lastTrendUpdate = 0;
const unsigned long TREND_INTERVAL = 30000;

// ------------- DHT 22 -----------------------------------------------------------------------------------------------
#define DHT_PIN 14
DHT dht(DHT_PIN, DHT22);
Timer updateDataDHT(10000); // 10s
float temperature = 0.0;
float humidity = 0.0;

// ------------- OLED Display -----------------------------------------------------------------------------------------------
Adafruit_SH1106G OLED = Adafruit_SH1106G(128, 64, &Wire, -1);
Timer breathEffect(80);   //0.08s
Timer updateTrend(30000); // 30s

// Variabel animasi
int animFrame = 0;
unsigned long lastUpdate = 0;
int breatheOffset = 0;
bool breatheUp = true;

// Timer for OLED
bool isOledDisplayOn = true;
Timer oledDisplayOff(10000);
int isDisplayInverted = false;

// ************** Clock ********************************************************************************************
// ------------- NTP -----------------------------------------------------------------------------------------------
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7 * 3600;
const int daylightOffset_sec = 0; // Indonesia tidak pakai Daylight Saving

// ------------- RTC -----------------------------------------------------------------------------------------------
RTC_DS3231 rtc;
Timer RTCupdate(1000);
Timer syncTime(60000 * 60);
bool isLastSyncSuccess = true;

// ------------- Time -----------------------------------------------------------------------------------------------
Timer printClock(1000); // Needed for colon blink
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
unsigned long startTouch = 0;
unsigned long howLongTouched = 0;


// Start
void setup(){
  debugBegin(115200);
  
  // RTC setup
  if(!rtc.begin()) debugPrintln("Couldn't find RTC"); // Not connected
  if(rtc.lostPower()) debugPrintln("RTC lost power"); // Amnesia

  // Setup DHT
  dht.begin();
  
  // Setup Oled
  OLED.begin(0x3c, true); 
  OLED.setContrast(0);
  OLED.clearDisplay();
  OLED.display();

  // Setup Dot Matrix
  Dmatrix.begin();
  Dmatrix.setTextAlignment(PA_CENTER); 
  Dmatrix.setIntensity(0);             
  
  // Setup for NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  Dmatrix.displayClear();
  
  delay(500);
}

void loop(){
  int currentTouch = touchRead(4);

  // Update data DHT (for trend)
  if (updateDataDHT.isReady()) {
    prevHumidity = humidity;
    prevTemperature = temperature;

    temperature = dht.readTemperature();
    humidity = dht.readHumidity();

    // debugPrintf("prevTemp: %0.2f  | Temp: %0.2f\n", prevTemperature, temperature);
    // debugPrintf("prevHumid: %0.2f  | Humid: %0.2f\n", prevHumidity, humidity);

  }

  // OLED Animation
  if(isOledDisplayOn){
    if (breathEffect.isReady()) {
      if (breatheUp) {
        breatheOffset++;
        if (breatheOffset >= 3) breatheUp = false;
      } else {
        breatheOffset--;
        if (breatheOffset <= 0) breatheUp = true;
      }
      animFrame++;

      // PROSES DRAWING
      OLED.clearDisplay(); 
      OLED.setTextColor(SH110X_WHITE);
      
      // Decorative Display
      drawTimeoutProgressBar(OLED, startTouch, 10000);
      drawDecorativeDots(OLED, animFrame);
      drawTemperatureContainer(OLED, breatheOffset);  // Temperature Container
      drawHumidityContainer(OLED, breatheOffset);     // Humidity Container

      // Data Display
      updateDataTempDisplay(OLED, temperature);
      updateDataHumidDisplay(OLED, humidity);
      drawTrendIndicator(OLED, 4, 45, temperature, prevTemperature); // Temperature trend
      drawTrendIndicator(OLED, 68, 45, humidity, prevHumidity);      // Humidity trend
      
      OLED.display();
    }

  }
  
  // Syncing RTC to NTP
  if(syncTime.isReady()){
    debugPrintln("SYNC start");
    initWiFi(ssid_wifi, pw_wifi, Dmatrix);
    
    isLastSyncSuccess = syncNtpToRtc(rtc);

    // Turn WiFi OFF
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    syncTime.reset();
  }

  // Dot Matrix
  if (printClock.isReady()) {
    showColon = !showColon;
    printLocalTime(rtc, Dmatrix, showColon);
    if(!isLastSyncSuccess){
      Dmatrix.getGraphicObject()->setPoint(7, 31, true);
    }
  }

  // if touched
  if(currentTouch  < thresholdTouched && touched== false){
    debugPrintf("Disentuh! ");
    touched = true;
    isOledDisplayOn = true;

    // Turning on display and change inverted
    OLED.invertDisplay(isDisplayInverted ? isDisplayInverted=false:isDisplayInverted=true);
    OLED.oled_command(SH110X_DISPLAYON);

    startTouch = millis();
    oledDisplayOff.reset();
  }

  if(currentTouch  > thresholdReleased && touched == true) {
    debugPrintf("Dilepas\n");
    touched = false;
    debugPrint("Disentuh selama: ");
    howLongTouched = millis()-startTouch;
    debugPrintln(howLongTouched);
  }

  // Timer for oled display
  if (isOledDisplayOn && oledDisplayOff.isReady()){
      isOledDisplayOn = false;
      OLED.clearDisplay();
      OLED.display();
      OLED.oled_command(SH110X_DISPLAYOFF); // Matikan panel secara fisik
  }
}
