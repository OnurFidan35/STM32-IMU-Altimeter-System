/*
 * bmp280.c
 *
 *  Created on: Mar 8, 2026
 *      Author: Onur Fidan
 */
#include "bmp280.h"
#include "sensordriver.h"

BMP280_Calib_t calib;
static BMP280_S32_t bmp280_compensate_T_int32(BMP280_S32_t adc_T);
static BMP280_U32_t bmp280_compensate_P_int64(BMP280_S32_t adc_P);
static void bmp280_read_calibration(void);

sensor_status_e BMP280_Init()
{

	sensor_status_e retVal;
    uint8_t id, settings = 0x27; // 0x27 = 001 001 11 (Temp x1, Press x1, Normal Mode) x1 hassasiyette ölçüm yapıyoruz
    id=sensor_read_register8(BMP280_ADDR, BMP280_REG_ID);
    if(id == 0x58) {
    	 bmp280_read_calibration();
         retVal=sensor_write_register8(BMP280_ADDR, BMP280_REG_CTRL_MEAS,settings);
         return retVal;
     }
    else
     return SENSOR_ERROR;



}

sensor_status_e BMP280_Read_All(BMP280_Data_t *Data)
{
	sensor_status_e retVal;
	uint8_t raw[6];
	retVal = sensor_read_bytes(BMP280_ADDR, BMP280_REG_PRESS_MSB, raw, 6);
	int32_t press_raw = (raw[0] << 12) | (raw[1] << 4) | (raw[2] >> 4);
	int32_t temp_raw = (raw[3] << 12) | (raw[4] << 4) | (raw[5] >> 4);


	Data->Temperature = bmp280_compensate_T_int32(temp_raw);
	Data->Pressure = bmp280_compensate_P_int64(press_raw);

	return retVal;
}

void bmp280_read_calibration(void) {
    uint8_t calib_data[24];

    sensor_read_bytes(BMP280_ADDR, BMP280_REG_CALIB_START, calib_data, 24);

    calib.dig_T1 = (uint16_t)(calib_data[1] << 8 | calib_data[0]);
    calib.dig_T2 = (int16_t)(calib_data[3] << 8 | calib_data[2]);
    calib.dig_T3 = (int16_t)(calib_data[5] << 8 | calib_data[4]);

    calib.dig_P1 = (uint16_t)(calib_data[7] << 8 | calib_data[6]);
    calib.dig_P2 = (int16_t)(calib_data[9] << 8 | calib_data[8]);
    calib.dig_P3 = (int16_t)(calib_data[11] << 8 | calib_data[10]);
    calib.dig_P4 = (int16_t)(calib_data[13] << 8 | calib_data[12]);
    calib.dig_P5 = (int16_t)(calib_data[15] << 8 | calib_data[14]);
    calib.dig_P6 = (int16_t)(calib_data[17] << 8 | calib_data[16]);
    calib.dig_P7 = (int16_t)(calib_data[19] << 8 | calib_data[18]);
    calib.dig_P8 = (int16_t)(calib_data[21] << 8 | calib_data[20]);
    calib.dig_P9 = (int16_t)(calib_data[23] << 8 | calib_data[22]);

}

BMP280_S32_t bmp280_compensate_T_int32(BMP280_S32_t adc_T)
{
BMP280_S32_t var1, var2, T;
var1  = ((((adc_T>>3) - ((BMP280_S32_t)calib.dig_T1<<1))) * ((BMP280_S32_t)calib.dig_T2)) >> 11;
var2  = (((((adc_T>>4) - ((BMP280_S32_t)calib.dig_T1)) * ((adc_T>>4) - ((BMP280_S32_t)calib.dig_T1))) >> 12) *
((BMP280_S32_t)calib.dig_T3)) >> 14;
calib.t_fine = var1 + var2;
T  = (calib.t_fine * 5 + 128) >> 8;
return T;
}

BMP280_U32_t bmp280_compensate_P_int64(BMP280_S32_t adc_P)
{
BMP280_S64_t var1, var2, p;
var1 = ((BMP280_S64_t)calib.t_fine) - 128000;
var2 = var1 * var1 * (BMP280_S64_t)calib.dig_P6;
var2 = var2 + ((var1*(BMP280_S64_t)calib.dig_P5)<<17);
var2 = var2 + (((BMP280_S64_t)calib.dig_P4)<<35);
var1 = ((var1 * var1 * (BMP280_S64_t)calib.dig_P3)>>8) + ((var1 * (BMP280_S64_t)calib.dig_P2)<<12);
var1 = (((((BMP280_S64_t)1)<<47)+var1))*((BMP280_S64_t)calib.dig_P1)>>33;
if (var1 == 0)
{
return 0; // avoid exception caused by division by zero
}
p = 1048576-adc_P;
p = (((p<<31)-var2)*3125)/var1;
var1 = (((BMP280_S64_t)calib.dig_P9) * (p>>13) * (p>>13)) >> 25;
var2 = (((BMP280_S64_t)calib.dig_P8) * p) >> 19;
p = ((p + var1 + var2) >> 8) + (((BMP280_S64_t)calib.dig_P7)<<4);
return (BMP280_U32_t)p;
}

