/*
 * sensor.h
 *
 *  Created on: Mar 8, 2026
 *      Author: Onur Fidan
 */

#ifndef SENSORS_SENSOR_H_
#define SENSORS_SENSOR_H_

void sensor_MPU6050_init(void);
void sensor_MPU6050_print_acc_values(void);
void sensor_MPU6050_print_gyro_values(void);
void sensor_BMP280_init(void);
void sensor_BMP280_print_all_values(void);
void sensor_BMP280_print_temperature(void);
void sensor_BMP280_print_pressure(void);
float sensor_Calculate_Altitude(float currentPressure_hPa);
#endif /* SENSORS_SENSOR_H_ */
