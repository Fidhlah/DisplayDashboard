| Komponen | Pin Modul | Pin ESP32 | Jalur Daya | Protokol | Catatan Advisor |
|---|---|---|---|---|---|
| Dot Matrix (MAX7219) | VCC | VIN / 5V | 5V | SPI | Ambil daya langsung dari jalur USB. |
|  | GND | GND | - | - | Pastikan Ground menyatu (Common Ground). |
|  | DIN | GPIO 23 | - | MOSI | Jalur data utama untuk Matrix. |
|  | CS / LOAD | GPIO 5 | - | CS | Chip Select untuk kontrol modul. |
|  | CLK | GPIO 18 | - | SCK | Jalur detak (clock) SPI. |
| OLED (SSD1306) | VCC | 3V3 | 3.3V | I2C | JANGAN PERNAH colok ke 5V. |
|  | GND | GND | - | - | - |
|  | SDA | GPIO 21 | - | I2C Data | Paralel (berbagi jalur) dengan RTC. |
|  | SCL | GPIO 22 | - | I2C Clock | Paralel (berbagi jalur) dengan RTC. |
| RTC (DS3231) | VCC | 3V3 | 3.3V | I2C | Gunakan baterai koin agar waktu tetap jalan. |
|  | GND | GND | - | - | - |
|  | SDA | GPIO 21 | - | I2C Data | Hubungkan ke jalur yang sama dengan OLED. |
|  | SCL | GPIO 22 | - | I2C Clock | Hubungkan ke jalur yang sama dengan OLED. |
| DHT22 (Suhu/Hum) | VCC | 3V3 | 3.3V | Digital | Lebih stabil di 3.3V untuk ESP32. |
|  | Data | GPIO 14 | - | 1-Wire | Pin digital tanpa gangguan I2C/SPI. |
|  | GND | GND | - | - | - |
| Touch | - | GPIO 4 | - | Kapasitif | Cukup kabel jumper yang ujungnya terbuka. |