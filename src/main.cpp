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
Timer updateDataDHT(10000); // 10s
float temperature = 0.0;
float humidity = 0.0;

// ------------- NTP -----------------------------------------------------------------------------------------------
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7 * 3600;
const int daylightOffset_sec = 0; // Indonesia tidak pakai Daylight Saving
Timer printClock(1000); // Needed for colon blink
bool showColon = true; 

// ------------- Dot Matrix -----------------------------------------------------------------------------------------------
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 5
MD_Parola Dmatrix = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// ------------- OLED Display -----------------------------------------------------------------------------------------------
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire, -1);
Timer breathEffect(80);   //0.08s
Timer updateTrend(30000); // 30s

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

bool isDisplayOn = true;
int howLongDisplayOn = 10000;
Timer displayOn(howLongDisplayOn);
int isDisplayInverted = false;

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
  
  // Setup DHT
  dht.begin();
  
  // Setup Oled
  display.begin(0x3c, true); 
  display.setContrast(0);
  display.clearDisplay();
  display.display();

  // Setup Dot Matrix
  Dmatrix.begin();
  Dmatrix.setTextAlignment(PA_CENTER); 
  Dmatrix.setIntensity(0);             
  
  initWiFi(ssid_wifi, pw_wifi, Dmatrix);
  delay(1000);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  Dmatrix.displayClear();
  
  delay(500);
}

void loop(){
  int currentTouch = touchRead(4);
  unsigned long currentTime = millis();

  // Update data DHT and for trend
  if (updateDataDHT.isReady()) {
    prevHumidity = humidity;
    prevTemperature = temperature;

    temperature = dht.readTemperature();
    humidity = dht.readHumidity();

    debugPrintf("prevTemp: %0.2f  | Temp: %0.2f\n", prevTemperature, temperature);
    debugPrintf("prevHumid: %0.2f  | Humid: %0.2f\n", prevHumidity, humidity);

  }

  // OLED Animation
  if(isDisplayOn){
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
      display.clearDisplay(); 
      display.setTextColor(SH110X_WHITE);
      
      // Decorative Display
      drawTimeoutProgressBar(display, startTouch, 10000);
      drawDecorativeDots(display, animFrame);
      drawTemperatureContainer(display, breatheOffset);
      drawHumidityContainer(display, breatheOffset);

      // Data Display
      updateTempDisplay(display, temperature);
      updateHumidDisplay(display, humidity);
      drawTrendIndicator(display, 4, 45, temperature, prevTemperature); // Temperature trend
      drawTrendIndicator(display, 68, 45, humidity, prevHumidity);      // Humidity trend
      
      display.display();
    }

  }
  
  // Dot Matrix
  if (printClock.isReady()) {
    showColon = !showColon;
    printLocalTime(Dmatrix, showColon);
  }

  if(currentTouch  < thresholdTouched && touched== false){
    debugPrintf("Disentuh! ");
    touched = true;
    isDisplayOn = true;

    
    display.invertDisplay(isDisplayInverted ? isDisplayInverted=false:isDisplayInverted=true);
    display.oled_command(SH110X_DISPLAYON);

    startTouch = millis();
    displayOn.reset();
  }

  if(currentTouch  > thresholdReleased && touched == true) {
    debugPrintf("Dilepas\n");
    touched = false;
    debugPrint("Disentuh selama: ");
    howLongTouched = millis()-startTouch;
    debugPrintln(howLongTouched);
  }


  if (isDisplayOn && displayOn.isReady()){
      isDisplayOn = false;
      display.clearDisplay();
      display.display();
      display.oled_command(SH110X_DISPLAYOFF); // Matikan panel secara fisik
  }
}
