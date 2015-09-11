/*
 * mpu6050.h
 *
 *  Created on: 29 окт. 2014 г.
 *      Author: shrike
 */

#ifndef MPU6050_H_
#define MPU6050_H_

#define MPU6050_ADDRES				0xD0

#define MPU6050_SELFTEST_X			0x0D
#define MPU6050_SELFTEST_Y			0x0E
#define MPU6050_SELFTEST_Z			0x0F
#define MPU6050_SELFTEST_A			0x10
#define MPU6050_SMPLRD_DIV			0x19
#define MPU6050_CONFIG				0x1A
#define MPU6050_GYRO_CONFIG			0x1B
#define MPU6050_ACCEL_CONFIG		0x1C
#define MPU6050_FIFO_EN				0x23
#define MPU6050_INT_PIN_CFG			0x37
#define MPU6050_INT_ENABLE			0x38
#define MPU6050_INT_STATUS			0x3A
#define MPU6050_ACCEL_XOUT_H		0x3B
#define MPU6050_ACCEL_XOUT_L		0x3C
#define MPU6050_ACCEL_YOUT_H		0x3D
#define MPU6050_ACCEL_YOUT_L		0x3E
#define MPU6050_ACCEL_ZOUT_H		0x3F
#define MPU6050_ACCEL_ZOUT_L		0x40
#define MPU6050_TEMP_OUT_H			0x41
#define MPU6050_TEMP_OUT_L			0x42
#define MPU6050_GYRO_XOUT_H			0x43
#define MPU6050_GYRO_XOUT_L			0x44
#define MPU6050_GYRO_YOUT_H			0x45
#define MPU6050_GYRO_YOUT_L			0x46
#define MPU6050_GYRO_ZOUT_H			0x47
#define MPU6050_GYRO_ZOUT_L			0x48
#define MPU6050_SIGNAL_PATH_RESET	0x68
#define MPU6050_USER_CTRL			0x6A
#define MPU6050_PWR_MGMT_1			0x6B
#define MPU6050_PWR_MGMT_2			0x6C
#define MPU6050_FIFO_COUNT_H		0x72
#define MPU6050_FIFO_COUNT_L		0x73
#define MPU6050_FIFO_RW				0x74
#define MPU6050_WHO_AM_I			0x75

// structures

// axis test
struct SELFTEST
{
	uint8_t accelTest:3;
	uint8_t gyroTest:5;
};

//config
struct CONFIG
{
	uint8_t :2;
	uint8_t extSyncSet:3;
	uint8_t dlpfCfg:3;
};

// gyro/accel config
struct DEV_CONFIG
{
	uint8_t xSelfTest:1;
	uint8_t ySelfTest:1;
	uint8_t zSelfTest:1;
	uint8_t fsSel:2;
	uint8_t :3;
};

struct FIFO_ENABLE
{
	uint8_t tempEnable:1;
	uint8_t xGyroEnable:1;
	uint8_t yGyroEnable:1;
	uint8_t zGyroEnable:1;
	uint8_t accelEnable:1;
	uint8_t slave1Enable:1;
	uint8_t slave2Enable:1;
	uint8_t slave3Enable:1;
};

struct I2C_MASTER_CONTROL
{
	uint8_t multiMasterEnable:1;
	uint8_t waitForEs:1;
	uint8_t slave3FifoEnable:1;
	uint8_t pNsr:1;
	uint8_t masterClock:4;
};

struct I2C_SLAVE_CONTROL
{
	uint8_t readWrite:1;
	uint8_t addres:7;
	uint8_t slaveRegister:8;
	uint8_t enable:1;
	uint8_t byteSwapping:1;
	uint8_t registerPriority:1;
	uint8_t grouping:1;
	uint8_t len:4;
};

struct I2C_EXTENDED_SLAVE_CONTROL
{
	uint8_t readWrite:1;
	uint8_t addres:7;
	uint8_t slaveRegister:8;
	uint8_t dataOutput:8;
	uint8_t enable:1;
	uint8_t interuptEnable:1;
	uint8_t registerPriority:1;
	uint8_t delay:5;
};

struct I2C_MASTER_STATUS
{
	uint8_t passThrough:1;
	uint8_t slave4Done:1;
	uint8_t lostArb:1;
	uint8_t slave4nack:1;
	uint8_t slave3nack:1;
	uint8_t slave2nack:1;
	uint8_t slave1nack:1;
	uint8_t slavenack:1;
};

struct INT_CONFIG
{
	uint8_t level:1;
	uint8_t openDrain:1;
	uint8_t latchEnable:1;
	uint8_t rwClear:1;
	uint8_t fsyncLevel:1;
	uint8_t fsyncEnable:1;
	uint8_t i2cBypassEnable:1;
	uint8_t :1;
};

struct INT_ENABLE
{
	uint8_t :1;
	uint8_t motion:1;
	uint8_t :1;
	uint8_t fifoOwerflow:1;
	uint8_t i2cMasterInt:1;
	uint8_t :2;
	uint8_t dataReady:1;
};

struct SIGNAL_PATH_RESRET
{
	uint8_t :5;
	uint8_t gyroReset:1;
	uint8_t accelReset:1;
	uint8_t tempReset:1;
};

struct USER_CONTROL
{
	uint8_t :1;
	uint8_t fifoEnable:1;
	uint8_t i2cMasterEnable:1;
	uint8_t i2cDisable:1;
	uint8_t :1;
	uint8_t fifoReset:1;
	uint8_t i2cMasterReset:1;
	uint8_t signalCountReset:1;
};

//PWR_MNGMT
struct PWR_MGMT
{
	uint8_t deviceReset:1;
	uint8_t sleep:1;
	uint8_t cycle:1;
	uint8_t :1;
	uint8_t tempDis:1;
	uint8_t clksel:3;
	uint8_t lpWakeControl:2;
	uint8_t accelXStendby:1;
	uint8_t accelYStendby:1;
	uint8_t accelZStendby:1;
	uint8_t gyroXStendby:1;
	uint8_t gyroYStendby:1;
	uint8_t gyroZStendby:1;
};


struct DeviceRegisters
{
	SELFTEST					xAxis;
	SELFTEST					yAxis;
	SELFTEST					zAxis;
	uint8_t  					accelSelfTest;
	uint8_t						sampleDivider;
	CONFIG						config;
	DEV_CONFIG					gyroConfig;
	DEV_CONFIG					accelConfig;
	FIFO_ENABLE 				fifoEnable;
	I2C_MASTER_CONTROL			i2cMasterControl;
	I2C_SLAVE_CONTROL			i2cslave[4];
	I2C_EXTENDED_SLAVE_CONTROL	i2cslave4;
	INT_CONFIG					intConfig;
	INT_ENABLE					intEnable;
	SIGNAL_PATH_RESRET			signalPathReset;
	USER_CONTROL				userControl;
	PWR_MGMT					powerManagment;
};

union DeviceControl
{
	DeviceRegisters registers;
	uint8_t			data[32];
};


#endif /* MPU6050_H_ */
