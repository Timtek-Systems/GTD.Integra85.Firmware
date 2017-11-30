// TemperatureSensor.h

#ifndef _TEMPERATURESENSOR_h
#define _TEMPERATURESENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <OneWire.h>
#include <DallasTemperature.h>
#include "MovingAverage.h"

enum SensorState
	{
	Idle,
	WaitingForConversion,
	TimedOut,
	Absent
	};

class TemperatureSensor
	{
	public:
		TemperatureSensor(OneWire& wire);
		void Initialize();
		void Loop();
		float GetValue();

	private:
		OneWire *bus;
		DallasTemperature sensor;
		MovingAverage<float> readings;
		uint8_t deviceAddress, readAddress;
		SensorState state;
		unsigned long timestamp;
		static const unsigned long conversionTimeout = 750;
		static const unsigned long waitAfterError = 10000;
		static const unsigned long timeBetweenReadings = 1000;
		void IdleState();
		void WaitingForConversionState();
		void TimedOutState();
		void AbsentState();
		void ChangeState(SensorState newState);
		bool HasElapsed(unsigned long period);
	};

#endif

