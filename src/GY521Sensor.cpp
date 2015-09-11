/*
 * GY521Sensor.cpp
 *
 *  Created on: 27 окт. 2014 г.
 *      Author: shrike
 */

#include "GY521Sensor.h"
#include <string.h>

GY521Sensor* sensors[2] = {NULL, NULL} ;


void ErrorHandler(uint32_t num)
{
	if(sensors[num] != NULL)
		sensors[num]->ErrorHandler();
}

void EventHandler(uint32_t num)
{
	if(sensors[num] != NULL)
		sensors[num]->ErrorHandler();
}


GY521Sensor::GY521Sensor(I2C_TypeDef* i2c_in)
{
	i2c = i2c_in;
	gyroX = 0;
	gyroY = 0;
	gyroZ = 0;
	accelX = 0;
	accelY = 0;
	accelZ = 0;
	temperature = 0.0;
	buf16=0;
}

GY521Sensor::~GY521Sensor()
{
	gyroX = 0;
	gyroY = 0;
	gyroZ = 0;
	temperature = 0;
}

void GY521Sensor::ReadData()
{
	ReadTemperature();
	ReadGypoX();
	ReadGypoY();
	ReadGypoZ();
	ReadAccelX();
	ReadAccelY();
	ReadAccelZ();
}

bool GY521Sensor::Init()
{
	LowLevelInit();
	// инициализация датчика
	I2C_InitTypeDef i2cInit;
	i2cInit.I2C_ClockSpeed = 400000;
	i2cInit.I2C_Mode = I2C_Mode_I2C;
	i2cInit.I2C_DutyCycle = I2C_DutyCycle_2;
	i2cInit.I2C_OwnAddress1 = 0;
	i2cInit.I2C_Ack = I2C_Ack_Enable;
	i2cInit.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(i2c , &i2cInit);
	//enabling i2c
	I2C_Cmd(i2c, ENABLE);

	//clearing setup
	memset( deviceControl.data , 0 , sizeof(deviceControl));
	//offsetof(DeviceRegisters, gyroConfig);
	// selftest off
	WriteRegister(MPU6050_SELFTEST_X,deviceControl.data, 4);
	//setting samleDevider to 100 Hz
	deviceControl.registers.sampleDivider = 79;
	WriteRegister(MPU6050_SMPLRD_DIV,&deviceControl.registers.sampleDivider, 1);

	// setting gyro/accel to on selftest 250 0/s 2g/s
	WriteRegister(MPU6050_GYRO_CONFIG,deviceControl.data + offsetof(DeviceRegisters, gyroConfig), 2);

	//disabling fifo and  i2c
	WriteRegister(MPU6050_FIFO_EN,deviceControl.data+offsetof(DeviceRegisters,fifoEnable),18 );
	if( ReadRegister(MPU6050_WHO_AM_I) == (MPU6050_ADDRES>>1) )
		return true;

	return false;
}

float GY521Sensor::Temperature()
{
	return this->temperature;
}

void GY521Sensor::LowLevelInit()
{
	GPIO_InitTypeDef gpioInit;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	if( i2c == I2C2 )
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
		gpioInit.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	}
	else
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
		gpioInit.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	}
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	gpioInit.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init( GPIOB , &gpioInit);

	if( i2c ==  I2C2 )
	{
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);
	}
	else
	{
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
	}
}

uint8_t GY521Sensor::ReadRegister(uint8_t registor)
{
	readBuf[0] = 0;

	I2C_AcknowledgeConfig(i2c, ENABLE);
	while( I2C_GetFlagStatus(i2c, I2C_FLAG_BUSY));
	// генерируем старт посылку
	I2C_GenerateSTART(i2c,ENABLE);
	while( I2C_CheckEvent(i2c , I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);

	I2C_Send7bitAddress( i2c ,MPU6050_ADDRES, I2C_Direction_Transmitter );
	while( I2C_CheckEvent(i2c , I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS );

	I2C_SendData(i2c , registor);
	while(I2C_CheckEvent(i2c ,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS);

	// Получаем данные
	I2C_NACKPositionConfig(i2c, I2C_NACKPosition_Current);
	I2C_GenerateSTART(i2c,ENABLE);
	while( I2C_CheckEvent(i2c , I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);

	I2C_Send7bitAddress( i2c ,MPU6050_ADDRES, I2C_Direction_Receiver );
	while(I2C_CheckEvent(i2c , I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS);

	I2C_AcknowledgeConfig(i2c, DISABLE);
	I2C_GenerateSTOP(i2c,ENABLE);

	while(I2C_CheckEvent(i2c , I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS );
	readBuf[0] = I2C_ReceiveData(i2c);

	while(I2C_GetFlagStatus(i2c, I2C_FLAG_STOPF));

	return readBuf[0];
}

void GY521Sensor::ReadRegister(uint8_t registor,uint16_t& inBuf)
{
	// отправляем адрес и регистр
	I2C_AcknowledgeConfig(i2c, ENABLE);
	while( I2C_GetFlagStatus(i2c, I2C_FLAG_BUSY));
	// генерируем старт
	I2C_GenerateSTART(i2c,ENABLE);
	while( I2C_CheckEvent(i2c , I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
	// посылаем адрес с режимом записи
	I2C_Send7bitAddress( i2c ,MPU6050_ADDRES, I2C_Direction_Transmitter );
	while( I2C_CheckEvent(i2c , I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS );
	//посылаем данные
	I2C_SendData(i2c , registor);
	while(I2C_CheckEvent(i2c ,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS);

	//посылаем старт
	I2C_GenerateSTART(i2c,ENABLE);
	while( I2C_CheckEvent(i2c , I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);

	//посылаем адрес
	I2C_Send7bitAddress( i2c ,MPU6050_ADDRES, I2C_Direction_Receiver );
	I2C_NACKPositionConfig(i2c, I2C_NACKPosition_Next);
	while(I2C_CheckEvent(i2c , I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS);
	I2C_AcknowledgeConfig(i2c, DISABLE);
	//читаем байт
	while(!I2C_GetFlagStatus(i2c , I2C_FLAG_BTF));
	I2C_GenerateSTOP(i2c,ENABLE);

	readBuf[0] = I2C_ReceiveData(i2c);
	readBuf[1] = I2C_ReceiveData(i2c);

	while(I2C_GetFlagStatus(i2c, I2C_FLAG_STOPF));
	I2C_NACKPositionConfig(i2c, I2C_NACKPosition_Current);
	I2C_AcknowledgeConfig(i2c, ENABLE);
	inBuf = readBuf[0]<<8|readBuf[1];
}

void GY521Sensor::WriteRegister(uint8_t registor , uint8_t *data, uint32_t len)
{
	I2C_AcknowledgeConfig(i2c, ENABLE);
	while( I2C_GetFlagStatus(i2c, I2C_FLAG_BUSY));
	// генерируем старт посылку
	I2C_GenerateSTART(i2c,ENABLE);
	while( I2C_CheckEvent(i2c , I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);

	I2C_Send7bitAddress( i2c ,MPU6050_ADDRES, I2C_Direction_Transmitter );
	while( I2C_CheckEvent(i2c , I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS );

	I2C_SendData(i2c , registor);
	while(I2C_CheckEvent(i2c ,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS);

	while(len--)
	{
		I2C_SendData(i2c , *data++);
		while(I2C_CheckEvent(i2c ,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS);
	}

	I2C_GenerateSTOP(i2c,ENABLE);
	while(I2C_GetFlagStatus(i2c, I2C_FLAG_STOPF));
}

void GY521Sensor::ReadTemperature()
{
	ReadRegister(MPU6050_TEMP_OUT_H,buf16);
	temperature = (int16_t)buf16/340.0+36.53;
}

void GY521Sensor::ReadGypoX()
{
	ReadRegister(MPU6050_GYRO_XOUT_H,buf16);
	gyroX = buf16;
}
void GY521Sensor::ReadGypoY()
{
	ReadRegister(MPU6050_GYRO_YOUT_H,buf16);
	gyroY = buf16;
}
void GY521Sensor::ReadGypoZ()
{
	ReadRegister(MPU6050_GYRO_ZOUT_H,buf16);
	gyroZ = buf16;
}

void GY521Sensor::ReadAccelX()
{
	ReadRegister(MPU6050_ACCEL_XOUT_H,buf16);
	accelX = buf16;
}
void GY521Sensor::ReadAccelY()
{
	ReadRegister(MPU6050_ACCEL_YOUT_H,buf16);
	accelY = buf16;
}
void GY521Sensor::ReadAccelZ()
{
	ReadRegister(MPU6050_ACCEL_ZOUT_H,buf16);
	accelZ = buf16;
}

void I2C1_EV_IRQHandler(void)
{
	EventHandler(0);
}

void I2C1_ER_IRQHandler(void)
{
	ErrorHandler(0);
}

void I2C2_EV_IRQHandler(void)
{
	EventHandler(1);
}

void I2C2_ER_IRQHandler(void)
{
	ErrorHandler(1);
}
