/*
 * GY521Sensor.h
 *
 *  Created on: 27 окт. 2014 г.
 *      Author: shrike
 */

#ifndef GY521SENSOR_H_
#define GY521SENSOR_H_
#include "cmsis_device.h"
#include "mpu6050.h"


class GY521Sensor
{
	public:
		GY521Sensor(I2C_TypeDef*);
		virtual ~GY521Sensor();

		bool 		Init();
		float 		Temperature();
		int16_t 	GypoX();
		int16_t 	GypoY();
		int16_t 	GypoZ();

		void 		ReadData();
	protected:
		I2C_TypeDef* 				i2c;
		DeviceControl				deviceControl;
		int16_t 					gyroX;
		int16_t 					gyroY;
		int16_t 					gyroZ;
		int16_t 					accelX;
		int16_t 					accelY;
		int16_t 					accelZ;
		float 						temperature;
		uint16_t 					buf16;
		uint8_t						readBuf[2];

	protected:
		void		ErrorHandler();
		void		EventHandler();

		uint8_t 	ReadRegister(uint8_t);
		void		ReadRegister(uint8_t,uint16_t&);
		void 		WriteRegister(uint8_t , uint8_t*, uint32_t );
		void		LowLevelInit();
		void		ReadTemperature();
		void		ReadGypoX();
		void		ReadGypoY();
		void		ReadGypoZ();
		void		ReadAccelX();
		void		ReadAccelY();
		void		ReadAccelZ();

		friend void ErrorHandler(uint32_t num);
		friend void EventHandler(uint32_t num);
};

#endif /* GY521SENSOR_H_ */
