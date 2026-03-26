# STM32-IMU-Altimeter-System
STM32F407 tabanlı, MPU6050 ve BMP280 sensörlerini kullanan; katmanlı mimariye sahip, Complementary Filter ve barometrik yükseklik hesaplama algoritmalarını içeren gerçek zamanlı oryantasyon sistemi.



🚀 Proje Adı: STM32 Tabanlı Akıllı IMU ve Altimetre Sistemi
1. Proje Özeti
Bu proje, STM32F407VGT6 mikrodenetleyicisi kullanılarak MPU6050 (Akselerometre & Jiroskop) ve BMP280 (Sıcaklık & Basınç) sensörlerinden gelen verilerin yüksek hassasiyetle işlenmesini kapsar. Projenin temel amacı, bir hava aracının veya otonom sistemin uzaydaki konumunu (Roll, Pitch) ve yüksekliğini (Altitude) gerçek zamanlı olarak hesaplamaktır.

2. Yazılım Mimarisi
Proje, modülerliği ve sürdürülebilirliği artırmak amacıyla Katmanlı Mimari (Layered Architecture) prensibiyle geliştirilmiştir:

Sürücü Katmanı (Sensordriver): I2C haberleşme protokolünü soyutlayarak alt seviye register okuma/yazma işlemlerini yönetir.

Sensör Sürücüleri: MPU6050 ve BMP280 sensörlerine özel yapılandırma, kalibrasyon ve ham veri okuma işlevlerini içerir.

Hesaplama (IMU) Katmanı: Ham sensör verilerini anlamlı fiziksel birimlere ve açılara dönüştürür.

Yönetici Katmanı (Sensor Manager): Sensörlerin başlatılması ve verilerin UART üzerinden kullanıcıya sunulması sürecini koordine eder.

3. Öne Çıkan Teknik Özellikler
Zamanlayıcı Tabanlı Veri Akışı: Sistem, Timer6 kesmesini (interrupt) kullanarak tam olarak 100ms periyotlarla (10Hz) stabil veri okuması yapar.

Tamamlayıcı Filtre (Complementary Filter): Jiroskopun zamanla biriken kaymasını (drift) ivmeölçer verisiyle düzelterek gürültüsüz ve kararlı Roll/Pitch açıları üretir.

Otomatik Kalibrasyon: Cihaz her açılışta MPU6050_Calibrate fonksiyonu ile bulunduğu ortamın gürültü eşiğini (bias) hesaplar ve jiroskop hatalarını sıfırlar.

Barometrik Altimetre: BMP280'den alınan hassas basınç verileri, uluslararası barometrik formül kullanılarak santimetre hassasiyetinde rakım bilgisine dönüştürülür.

4. Sensör ve Eksen Konfigürasyonu
Donanım Kurulumu: Sensör modülü breadboard üzerinde baş aşağı (inverted) şekilde sabitlenmiştir.

Matematiksel Düzeltme: Sensörün fiziksel ters duruşu, IMU katmanındaki atan2 formüllerinde eksen işaretleri normalize edilerek (Z ve X eksenleri için) yazılımsal olarak düzeltilmiştir.

5. Donanım Bileşenleri
Mikrodenetleyici: STM32F407VGT6 (ARM Cortex-M4).

Sensörler: MPU6050 (6-Eksenli IMU), BMP280 (Hassas Barometre).

Haberleşme: I2C (Sensörler için), USART2 (Debug/Terminal çıktısı için).
