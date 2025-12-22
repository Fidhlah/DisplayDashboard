#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include "time.h"
#include "DebugMacros.h"
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include "LogicManager.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

void printLocalTime(MD_Parola &display, bool isColonVisible){
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)){
    display.print("Failed!");   debugPrintln("Failed!");
    return;
  }
  char timeString[10];

  sprintf(timeString, "%02d%c%02d", timeinfo.tm_hour,(isColonVisible ? ':':' '), timeinfo.tm_min);

  display.print(timeString);
}

void drawThermometerIcon(Adafruit_SH1106 &display, int x, int y ) {
  display.drawRect(x+1, y, 2, 5, WHITE);
  display.drawCircle(x+1, y+7, 2, WHITE);
  display.drawPixel(x+1, y+5, WHITE);
}
void drawDropletIcon(Adafruit_SH1106 &display, int x, int y) {
  display.drawPixel(x+2, y, WHITE);
  display.drawLine(x+1, y+1, x+3, y+1, WHITE);
  display.drawLine(x, y+2, x+4, y+2, WHITE);
  display.drawLine(x, y+3, x+4, y+3, WHITE);
  display.drawLine(x, y+4, x+4, y+4, WHITE);
  display.drawLine(x+1, y+5, x+3, y+5, WHITE);
  display.drawPixel(x+2, y+6, WHITE);
}

void drawTrendIndicator(Adafruit_SH1106 &display, int x, int y, float currentVal, float prevVal) {
  float diff = currentVal - prevVal;
  debugPrint("trend: ");
  debugPrintln(diff);
  int arrowX = x + 8;
  int arrowY = y + 5;
  
  display.setTextSize(1);
  
  if (diff > 0.3) {
    // Panah naik ▲ (segitiga lancip di atas)
    display.fillTriangle(arrowX, arrowY-3, arrowX-4, arrowY+4, arrowX+4, arrowY+4, WHITE);
    
    // Selisih nilai di sebelah kanan panah
    display.setCursor(x + 20, y + 3);
    display.print("+");
    display.print(diff, 1);
    
  } else if (diff < -0.3) {
    // Panah turun ▼ (segitiga lancip di bawah)
    display.fillTriangle(arrowX, arrowY+7, arrowX-4, arrowY, arrowX+4, arrowY, WHITE);
    
    // Selisih nilai di sebelah kanan panah
    display.setCursor(x + 20, y + 3);
    display.print(diff, 1);
    
  } else {
    // Stabil ▶ (segitiga lancip di kanan)
    display.fillTriangle(arrowX+4, arrowY+2, arrowX-3, arrowY-2, arrowX-3, arrowY+6, WHITE);
    
    // Selisih hampir nol
    display.setCursor(x + 20, y + 3);
    display.print(" 0.0");
  }
}

void drawTemperatureContainer(Adafruit_SH1106 &display, int breathe) {
  int boxWidth = 62;
  int boxHeight = 58;
  int boxX = 1;
  int boxY = 3;
  
  // Box dengan corner radius dan efek breathe
  display.drawRoundRect(boxX, boxY + breathe/2, boxWidth, boxHeight - breathe, 6, WHITE);
  
  // Icon termometer di pojok kiri atas
  drawThermometerIcon(display, boxX + 12, boxY + 4);
  
  // Label SUHU kecil di sebelah icon
  display.setTextSize(1);
  display.setCursor(boxX + 20, boxY + 5);
  display.print("TEMP");
  
}

void updateTempDisplay(Adafruit_SH1106 &display, float temperature){
  // debugPrintf("UpdateTemp: %0.2f\n", temperature);
  int boxWidth = 62;
  int boxX = 1;
  int boxY = 3;

  // Nilai temperature BESAR di tengah
  display.setTextSize(2);
  char tempStr[6];
  dtostrf(temperature, 4, 1, tempStr);
  int textWidth = strlen(tempStr) * 12;
  display.setCursor(boxX + (boxWidth - textWidth)/2, boxY + 18);
  display.print(tempStr);
}



void drawHumidityContainer(Adafruit_SH1106 &display,int breathe) {
  int boxWidth = 62;
  int boxHeight = 58;
  int boxX = 65;
  int boxY = 3;
  
  // Box dengan corner radius dan efek breathe
  display.drawRoundRect(boxX, boxY + breathe/2, boxWidth, boxHeight - breathe, 6, WHITE);
  
  // Icon droplet di pojok kiri atas
  drawDropletIcon(display, boxX + 12, boxY + 4);
  
  // Label HUMID kecil di sebelah icon
  display.setTextSize(1);
  display.setCursor(boxX + 20, boxY + 5);
  display.print("HUMID");
  
}

void updateHumidDisplay(Adafruit_SH1106 &display, float humidity){
  int boxWidth = 62;
  int boxHeight = 58;
  int boxX = 65;
  int boxY = 3;
  
  // Nilai humidity BESAR di tengah
  display.setTextSize(2);
  char humidStr[6];
  dtostrf(humidity, 4, 1, humidStr);
  int textWidth = strlen(humidStr) * 12;
  display.setCursor(boxX + (boxWidth - textWidth)/2, boxY + 18);
  display.print(humidStr);
}

void drawDecorativeDots(Adafruit_SH1106 &display, int frame) {
  // Dots atas
  for (int i = 0; i < 5; i++) {
    int x = 24 + i * 16;
    int brightness = (frame + i * 10) % 30;
    if (brightness < 15) {
      display.fillCircle(x, 1, 1, WHITE);
    } else {
      display.drawPixel(x, 1, WHITE);
    }
  }
  
  // Dots bawah
  for (int i = 0; i < 5; i++) {
    int x = 24 + i * 16;
    int brightness = (frame + i * 10 + 15) % 30;
    if (brightness < 15) {
      display.fillCircle(x, 62, 1, WHITE);
    } else {
      display.drawPixel(x, 62, WHITE);
    }
  }
}

#endif 