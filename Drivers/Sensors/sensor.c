/*
 * sensor.c
 *
 *  Created on: Mar 8, 2026
 *      Author: Onur Fidan
 */

#include "mpu6050.h"
#include "sensor.h"
#include "bmp280.h"
#include "imu.h"
#include <math.h>

extern UART_HandleTypeDef huart2;
static SensorData_t sensorData;
static BMP280_Data_t BMP280Data;
static Attitude_t currentAttitude = {0.0, 0.0, 0.0};

void sensor_MPU6050_init(void)
{
    MPU6050_initialize(&sensorData, FS_1000, AFS_4G);
    MPU6050_Calibrate(&sensorData);
}

void sensor_MPU6050_print_acc_values(void)
{
    char buffer[100];
    int32_t len;

    MPU6050_read_data(&sensorData);

    len = sprintf(buffer,"Acc: X:%f Y:%f Z:%f\n",sensorData.acc.X, sensorData.acc.Y, sensorData.acc.Z );
    HAL_UART_Transmit(&huart2,(uint8_t *)buffer, len,100);
}

void sensor_MPU6050_print_gyro_values(void)
{
    char buffer[100];
    int32_t len;

    MPU6050_read_data(&sensorData);

    len = sprintf(buffer,"Gyro: X:%f Y:%f Z:%f\n",sensorData.gyro.X, sensorData.gyro.Y, sensorData.gyro.Z );
    HAL_UART_Transmit(&huart2,(uint8_t *)buffer, len,100);
}

void sensor_MPU6050_print_attitude(void) {
    char buffer[100];
    int32_t len;

    MPU6050_read_data(&sensorData);

    // Timer6 her 100ms'de bir çalıştığı için dt = 0.1
    IMU_Calculate_Attitude(&sensorData, &currentAttitude, 0.1);

    len = sprintf(buffer, "Roll: %.2f | Pitch: %.2f\r\n", currentAttitude.roll, currentAttitude.pitch);
    HAL_UART_Transmit(&huart2, (uint8_t *)buffer, len, 100);
}

void sensor_BMP280_init(void)
{
    BMP280_Init();
}

void sensor_BMP280_print_all_values(void)
{
	char uart_buf[200];
	float pressure_hPa;
	float altitude;
    BMP280_Read_All(&BMP280Data);

    pressure_hPa = BMP280Data.Pressure / 25600.0f;
    altitude = sensor_Calculate_Altitude(pressure_hPa);

    sprintf(uart_buf, "TEMP: %.2f °C | PRESS: %.2f hPa | ALT: %.2f m\r\n", BMP280Data.Temperature / 100.0,pressure_hPa,altitude);
    HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, strlen(uart_buf), 100);
}

void sensor_BMP280_print_temperature(void)
{
	char uart_buf[200];

    BMP280_Read_All(&BMP280Data);

    sprintf(uart_buf, "TEMP: %.2f °C \r\n", BMP280Data.Temperature / 100.0);
    HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, strlen(uart_buf), 100);
}

void sensor_BMP280_print_pressure(void)
{
	char uart_buf[200];

    BMP280_Read_All(&BMP280Data);

    sprintf(uart_buf, "PRESS: %.2f hPa\r\n",BMP280Data.Pressure / 25600.0);
    HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, strlen(uart_buf), 100);
}

float sensor_Calculate_Altitude(float currentPressure_hPa) {
    const float seaLevelPressure = 1013.25f;

    // Uluslararası barometrik formül
    // Altitude = 44330 * (1 - (P/P0)^(1/5.255))
    float altitude = 44330.0f * (1.0f - powf((currentPressure_hPa / seaLevelPressure), 0.190295f));

    return altitude;
}
