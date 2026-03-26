/*
 * imu.c
 *
 *  Created on: Mar 24, 2026
 *      Author: Onur Fidan
 */


#include "imu.h"
#include <math.h>

#define RAD_TO_DEG 57.2957795131f
#define ALPHA 0.96f // Tamamlayıcı filtre katsayısı (%96 Gyro, %2 Accel)

void IMU_Calculate_Attitude(SensorData_t *mpuData, Attitude_t *attitude, double dt) {

	// 1. Jiroskop Ofsetlerini Çıkar (Kalibrasyon verisini kullanıyoruz)
	    // Önemli: MPU6050'de Pitch genellikle Y ekseni etrafındaki dönüştür, Roll ise X.
	    double gyro_rate_x = mpuData->gyro.X - mpuData->gyro_x_offset;
	    double gyro_rate_y = mpuData->gyro.Y - mpuData->gyro_y_offset;

	    // 2. İvmeölçer ile Açı Hesapla (Statik Açı)
	    // Pitch: X ekseninin eğimi
	    double accel_pitch = atan2(mpuData->acc.Y, sqrt(mpuData->acc.X * mpuData->acc.X + mpuData->acc.Z * mpuData->acc.Z)) * RAD_TO_DEG;
	    // Roll: Y ekseninin eğimi
	    double accel_roll = atan2(-mpuData->acc.X, -mpuData->acc.Z) * RAD_TO_DEG;

	    // 3. Tamamlayıcı Filtre (Complementary Filter)
	    // Pitch ve Roll eksenlerinin jiroskop karşılıkları sensör montajına göre değişebilir.
	    // Genellikle: gyro_x -> Roll hızı, gyro_y -> Pitch hızıdır.
	    attitude->pitch = ALPHA * (attitude->pitch + gyro_rate_y * dt) + (1.0f - ALPHA) * accel_pitch;
	    attitude->roll = ALPHA * (attitude->roll + gyro_rate_x * dt) + (1.0f - ALPHA) * accel_roll;
}
