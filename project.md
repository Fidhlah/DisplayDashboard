# 📌 Project.md
# Ringkasan Project ESP32 (Jam + Suhu + Kelembaban)

## 🎯 Tujuan Project
Membuat **project personal fun** berbasis **ESP32** yang:
- Menampilkan **jam real-time**
- Menampilkan **suhu & kelembaban**
- Fokus belajar **dasar mikrokontroler secara nyata**, bukan lomba / produk
- Bisa dikembangkan jadi interaktif & ekspresif (OLED + “ekspresi robot”)

---

## 🧠 Konsep Inti
- **ESP32 sebagai otak utama**
- **Jam**:
  - Awal: sinkronisasi **NTP**
  - Final: **RTC DS3231** (offline, stabil, real-time)
- **Sensor lingkungan**:
  - Mulai dari **DHT22**
  - Opsional upgrade ke **AHT20 + BMP280**
- **Output visual**:
  - Dot Matrix → jam besar & jelas
  - OLED → info tambahan seperti suhu dan kelembaban

---

## 🔧 Arsitektur Sistem (High Level)
Input → Proses → Output

- Input:
  - Waktu (NTP / RTC)
  - Suhu & kelembaban
  - Interaksi (touch / sentuhan kabel) * In the future
- Proses:
  - Loop non-blocking
  - Update sensor berkala
  - Jam berjalan real-time tanpa delay
- Output:
  - Dot Matrix (jam)
  - OLED (info suhu dan kelembaban)

---

## 🧭 Arah Pengembangan
- Tambah fitur OLED (sensor sentuh, bisa memunculkan emoji atau ekspresi gitu)
- Tambah interaksi touch (tanpa sensor tambahan jika pakai ESP32)
- Mode offline full (RTC + baterai)
- Power portable (Li-ion + TP4056)

---

## 🧠 Mindset Project
- Project ini **buat senang-senang**
- Tidak dikejar:
  - Estetika sempurna
  - Skalabilitas produk
  - Standar lomba
- Batasan cuma:
  - Biaya masuk akal
  - Tidak overkill buat pemula
- Belajar lewat **ngoprek nyata**, bukan teori kosong
