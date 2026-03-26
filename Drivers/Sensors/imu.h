/*
 * imu.h
 *
 *  Created on: Mar 24, 2026
 *      Author: Onur Fidan
 */

#ifndef SENSORS_IMU_H_
#define SENSORS_IMU_H_

#include "mpu6050.h"

typedef struct {
    double roll;
    double pitch;
    double yaw; // Yaw için genellikle manyetometre gerekir ama jiroskopla tahmini bakılabilir
} Attitude_t;

// Açı hesaplama fonksiyonu (dt: geçen süre, saniye cinsinden)
void IMU_Calculate_Attitude(SensorData_t *mpuData, Attitude_t *attitude, double dt);


#endif /* SENSORS_IMU_H_ */
