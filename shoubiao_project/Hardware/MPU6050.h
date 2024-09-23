#ifndef _MPU6050_H
#define _MPU6050_H
#include "stm32f10x.h"



#define	MPU6050_SMPLRT_DIV		0x19
#define	MPU6050_CONFIG			0x1A
#define	MPU6050_GYRO_CONFIG		0x1B
#define	MPU6050_ACCEL_CONFIG	0x1C

#define	MPU6050_ACCEL_XOUT_H	0x3B
#define	MPU6050_ACCEL_XOUT_L	0x3C
#define	MPU6050_ACCEL_YOUT_H	0x3D
#define	MPU6050_ACCEL_YOUT_L	0x3E
#define	MPU6050_ACCEL_ZOUT_H	0x3F
#define	MPU6050_ACCEL_ZOUT_L	0x40
#define	MPU6050_TEMP_OUT_H		0x41
#define	MPU6050_TEMP_OUT_L		0x42
#define	MPU6050_GYRO_XOUT_H		0x43
#define	MPU6050_GYRO_XOUT_L		0x44
#define	MPU6050_GYRO_YOUT_H		0x45
#define	MPU6050_GYRO_YOUT_L		0x46
#define	MPU6050_GYRO_ZOUT_H		0x47
#define	MPU6050_GYRO_ZOUT_L		0x48

#define	MPU6050_PWR_MGMT_1		0x6B
#define	MPU6050_PWR_MGMT_2		0x6C
#define	MPU6050_WHO_AM_I		0x75

uint8_t  MPU6050_ReadReg(uint8_t RegAddress);
void MPU6050_Init(void);
void  MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data);
typedef struct 
{
	int16_t accel_x;
	int16_t accel_y;
	int16_t accel_z;
	
}MPU6050_accel;
typedef struct 
{
	int16_t gyro_x;
	int16_t gyro_y;
	int16_t gyro_z;
	
}MPU6050_gyro;
void MPU6050_getdata(MPU6050_accel * accel,MPU6050_gyro * gyro);
#endif
