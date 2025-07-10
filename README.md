# Capacitive-Soil-Moisture-Sensor-with-Relay
Mengukur dan membaca kelembapan tanah, mengkonversikannya ke persentase, dan kemudian secara otomatis menyiram tanaman berdasarkan tingkat kekeringan tanah. Semakin kering tanah (persentase kelembapan rendah), semakin lama sistem akan menyiram. Jika tanah sudah cukup basah, sistem tidak akan menyiram sama sekali.

---

### Inisialisasi dan Pengaturan Awal

Pada bagian awal kode, beberapa variabel global didefinisikan:

* `const int soilMoisturePin = 27;`: Mendefinisikan pin analog **GPIO 27** sebagai input untuk membaca data dari **sensor kelembapan tanah**.
* `const int relayPin = 26;`: Mendefinisikan pin **GPIO 26** sebagai output untuk mengontrol **modul relay**. Modul relay ini kemungkinan akan terhubung ke pompa air atau perangkat penyiram lainnya.
* `int Treshold1 = 30;`, `int Treshold2 = 60;`, `int Treshold3 = 90;`: Mendefinisikan tiga nilai **ambang batas (threshold)** untuk persentase kelembapan tanah. Nilai-nilai ini akan digunakan untuk menentukan kapan dan berapa lama sistem penyiram harus aktif.

Pada fungsi `void setup()`:

* `Serial.begin(115200);`: Memulai komunikasi serial dengan baud rate 115200. Ini digunakan untuk menampilkan data kelembapan tanah dan status sistem ke **Serial Monitor**, yang membantu dalam debugging dan pemantauan.
* `pinMode(relayPin, OUTPUT);`: Mengatur pin `relayPin` (GPIO 26) sebagai **output**, yang berarti mikrokontroler dapat mengirim sinyal HIGH atau LOW untuk mengontrol relay.

---

### Logika Utama Program (Fungsi `void loop()`)

Fungsi `void loop()` adalah bagian utama program yang akan berjalan secara terus-menerus:

1.  **Membaca Nilai Kelembapan Tanah:**
    * `int moistureValue = analogRead(soilMoisturePin);`: Membaca nilai analog dari sensor kelembapan tanah yang terhubung ke pin `soilMoisturePin` (GPIO 27). Nilai analog ini biasanya berkisar antara 0 hingga 4095 pada mikrokontroler ESP32 (yang sering menggunakan GPIO 27 untuk ADC). Nilai yang lebih rendah menunjukkan tanah yang lebih kering, dan nilai yang lebih tinggi menunjukkan tanah yang lebih basah.

2.  **Konversi Nilai ke Persentase:**
    * `int moisturePercent = map(moistureValue, 900, 2500, 100, 0);`: Mengkonversi nilai ADC `moistureValue` menjadi persentase kelembapan (0-100%).
        * Fungsi `map()` mengambil nilai `moistureValue` dan memetakannya dari rentang input (900 hingga 2500) ke rentang output (100 hingga 0).
        * Asumsi di sini adalah:
            * Jika `moistureValue` sekitar 900, itu dianggap 100% lembap (tanah sangat basah).
            * Jika `moistureValue` sekitar 2500, itu dianggap 0% lembap (tanah sangat kering).
        * Rentang 900-2500 kemungkinan ditemukan melalui kalibrasi sensor, karena nilai ini bisa bervariasi tergantung jenis sensor dan kondisi lingkungan.

3.  **Menampilkan Data ke Serial Monitor:**
    * `Serial.print("Soil Moisture: "); Serial.print(moisturePercent); Serial.println("%"); Serial.println(moistureValue);`: Mencetak persentase kelembapan tanah dan nilai ADC mentah ke Serial Monitor. Ini membantu Anda memantau kondisi tanah secara real-time.

4.  **Logika Pengontrolan Relay Berdasarkan Kelembapan:**
    Bagian ini adalah inti dari sistem penyiram otomatis, menggunakan serangkaian pernyataan `if-else if` untuk menentukan tindakan berdasarkan `moisturePercent`:

    * `if (moisturePercent <= Treshold1)` (Jika kelembapan $\leq$ 30% - **Sangat Kering**):
        * `digitalWrite(relayPin, HIGH);`: Mengaktifkan relay (menyalakan pompa air/penyiram).
        * `delay(10000);`: Menunggu selama 10 detik. Ini adalah durasi penyiraman.
        * `digitalWrite(relayPin, LOW);`: Menonaktifkan relay (mematikan pompa air/penyiram).
        * Ini berarti jika tanah sangat kering, sistem akan menyiram selama 10 detik.

    * `else if (moisturePercent > Treshold1 && moisturePercent <= Treshold2)` (Jika kelembapan > 30% dan $\leq$ 60% - **Cukup Kering**):
        * `digitalWrite(relayPin, HIGH);`: Mengaktifkan relay.
        * `delay(7000);`: Menunggu selama 7 detik.
        * `digitalWrite(relayPin, LOW);`: Menonaktifkan relay.
        * Jika tanah cukup kering, sistem akan menyiram selama 7 detik.

    * `else if (moisturePercent > Treshold2 && moisturePercent <= Treshold3)` (Jika kelembapan > 60% dan $\leq$ 90% - **Sedikit Kering/Lembap Ideal**):
        * `digitalWrite(relayPin, HIGH);`: Mengaktifkan relay.
        * `delay(3000);`: Menunggu selama 3 detik.
        * `digitalWrite(relayPin, LOW);`: Menonaktifkan relay.
        * Jika tanah sedikit kering atau mendekati lembap ideal, sistem akan menyiram selama 3 detik.

    * `else if (moisturePercent > Treshold3)` (Jika kelembapan > 90% - **Sangat Basah**):
        * `digitalWrite(relayPin, LOW);`: Memastikan relay mati (pompa air/penyiram tidak aktif).
        * Ini berarti jika tanah sudah sangat basah, tidak ada penyiraman yang akan dilakukan.

5.  **Penundaan Loop:**
    * `delay(5000);`: Setelah setiap siklus pembacaan dan kontrol, program akan berhenti selama 5 detik sebelum mengulang `void loop()` kembali. Ini mencegah sensor dibaca dan relay dikontrol terlalu cepat, memberikan waktu bagi air untuk menyerap dan kelembapan tanah berubah.
