#ifndef DEBUG_MACROS_H
#define DEBUG_MACROS_H

#include <Arduino.h>

// ================= SETTING =================
#define DEBUG_MODE 1  // 0 = Production, 1 = Development
// ===========================================

#if DEBUG_MODE
  #define debugBegin(x)    Serial.begin(x)
  #define debugPrint(x)    Serial.print(x)
  #define debugPrintln(x)  Serial.println(x)
  #define debugPrintf(...) Serial.printf(__VA_ARGS__)
#else
  #define debugBegin(x)
  #define debugPrint(x)
  #define debugPrintln(x)
  #define debugPrintf(...)
#endif

#endif // Akhir dari DEBUG_MACROS_H