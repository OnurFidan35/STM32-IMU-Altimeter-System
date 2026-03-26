/*
 * bmp280.h
 *
 *  Created on: Mar 8, 2026
 *      Author: Onur Fidan
 */

#ifndef SENSORS_BMP280_H_
#define SENSORS_BMP280_H_

#include "stm32f4xx_hal.h"
#include "sensordriver.h"

#define BMP280_ADDR (0x76 << 1) // SDO GND'ye bağlıysa

#define BMP280_REG_ID           0xD0
#define BMP280_REG_RESET        0xE0
#define BMP280_REG_CTRL_MEAS    0xF4
#define BMP280_REG_CONFIG       0xF5
#define BMP280_REG_PRESS_MSB    0xF7
#define BMP280_REG_TEMP_MSB     0xFA
#define BMP280_REG_CALIB_START  0x88

typedef long signed int   BMP280_S32_t;   // 32 bit signed integer
typedef long unsigned int BMP280_U32_t;   // 32 bit unsigned integer
typedef long long signed int BMP280_S64_t;

typedef struct {
    uint16_t dig_T1; int16_t dig_T2; int16_t dig_T3;
    uint16_t dig_P1; int16_t dig_P2; int16_t dig_P3;
    int16_t dig_P4;  int16_t dig_P5;  int16_t dig_P6;
    int16_t dig_P7;  int16_t dig_P8;  int16_t dig_P9;
    int32_t t_fine; // Sıcaklık ve basınç ortak hesabı için
} BMP280_Calib_t;

typedef struct {
	int32_t temp_raw;
	int32_t press_raw;
    float Temperature;
    float Pressure;
} BMP280_Data_t;

sensor_status_e BMP280_Init();
sensor_status_e BMP280_Read_All(BMP280_Data_t *Data);
sensor_status_e BMP280_test_sensor(void);


#endif /* SENSORS_BMP280_H_ */
