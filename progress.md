# 📌 Progress.md
# Progress Aktivitas & Checklist Barang

## ⚙️ Progress Teknis Saat Ini

### ✅ Sudah Beres
- ESP32 nyala & komunikasi serial OK
- Koneksi WiFi berhasil
- Sinkronisasi waktu via **NTP**
- Jam berhasil diprint ke Serial
- DHT22 terbaca & update normal
- Memahami bahwa:
  - DHT22 **tidak instan**
  - Refresh ideal: **2–5 detik**
  - Jam **tidak boleh terganggu delay**

---

### 🔄 Sedang Dikerjakan
- Menggabungkan **NTP + DHT22 tanpa blocking delay**
- Merapikan struktur `loop()` (non-blocking)
- Pemahaman praktis:
  - Kapan pakai resistor
  - Pull-up internal vs eksternal
  - Alur data sensor → logika → output

---

### 🧪 Belum / Rencana
- Pasang **RTC DS3231**
- Tambah **OLED**
- Tambah **Dot Matrix**
- Eksperimen ekspresi visual
- Input interaksi (touch / gesture)
- Power via baterai (TP4056 + regulasi)

---

## 📚 Insight Penting yang Sudah Dipahami
- DHT22 bukan sensor cepat
- Jam tidak boleh pakai `delay()`
- ESP32 punya **touch input internal**
- ESP32 ≠ ESP32-C3
- DevKit / NodeMCU = **board**
- WROOM = **modul**
- Female header penting biar ESP32 aman

---

## 🛒 Checklist Barang yang Dibutuhkan

### 1️⃣ Kontrol Utama
- [X] ESP32 DevKit / NodeMCU **USB Type-C**

### 2️⃣ Media Prototyping
- [ ] Breadboard 830 holes
- [x] Kabel jumper:
  - [x] Male to Male
  - [x] Male to Female
  - (Female to Female tidak wajib)

### 3️⃣ Sensor & Waktu
- [x] DHT22 (Temperature & Humidity) ✅
- [ ] RTC DS3231 + baterai CR2032

### 4️⃣ Display Output
- [ ] MAX7219 LED Dot Matrix 8×8 **4-in-1 (32×8)**
- [ ] OLED I2C (pilih satu):
  - [ ] OLED 0.96" 128×64
  - [ ] OLED 0.91" 128×32

### 5️⃣ Daya (Sementara)
- [x] Kabel USB Type-C *Bukan dedicated untuk project ini, cari kabel yang ada aja
- [ ] Adapter 5V / powerbank

### 6️⃣ Daya (Baterai – Nanti)
- [ ] Baterai Li-ion / Li-Po 3.7V
- [ ] Modul charger **TP4056 + protection**
- [ ] Step-down / regulator (jika diperlukan)

### 7️⃣ Komponen Pendukung
- [ ] Resistor **4.7kΩ – 10kΩ** (pull-up DHT22)
- [ ] Resistor kit assorted (opsional)
- [ ] Female header / socket header
- [ ] Header pin male cadangan

### 8️⃣ Tools Dasar
- [ ] Solder
- [ ] Timah solder
- [ ] Flux
