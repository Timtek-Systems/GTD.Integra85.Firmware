// 
// 
// 

#define REQUIRESNEW false		// DallasTemperature implements dynamic allocation
#define REQUIRESALARMS false	// DallasTemperature implements alarm handling
#include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor(OneWire& wire)
	{
	this->bus = &wire;
	bus->reset();
	this->sensor = DallasTemperature(bus);
	this->readings = MovingAverage<float>(5);
	}

void TemperatureSensor::Initialize()
	{
	readings.init(12.0);
	sensor.begin();
	sensor.setWaitForConversion(false);					// Non-blocking mode
	sensor.setCheckForConversion(true);					// Polling mode
	sensor.setResolution(12);							// A to D resolution in bits.
	bool found = sensor.getAddress(&deviceAddress, 0);	// Get the address of the 1st sensor
	state = Idle;
	ChangeState(found && sensor.validFamily(&deviceAddress) ? Idle : Absent);
	}

void TemperatureSensor::ChangeState(SensorState newState)
	{
	timestamp=millis();
	//Serial.print(timestamp);
	//Serial.print(": ");
	//Serial.print(state);
	//Serial.print("->");
	//Serial.println(newState);
	state = newState;
	}

void TemperatureSensor::Loop()
	{
	switch (state)
		{
		case Idle:
			IdleState();
			break;
		case WaitingForConversion:
			WaitingForConversionState();
			break;
		case TimedOut:
			TimedOutState();
			break;
		case Absent:
		default:
			break;
		}
	}

bool TemperatureSensor::HasElapsed(unsigned long period)
	{
	auto now = millis();
	return now - timestamp >= period;
	}

void TemperatureSensor::IdleState()
	{
	if (HasElapsed(timeBetweenReadings))
		{
		sensor.getAddress(&deviceAddress, 0);
		sensor.requestTemperaturesByAddress(&deviceAddress);
		ChangeState(WaitingForConversion);
		}
	}

void TemperatureSensor::WaitingForConversionState()
	{
	if (HasElapsed(conversionTimeout))
		{
		auto celsius = sensor.getTempCByIndex(0);
		readings.addSample(celsius);
		ChangeState(Idle);
		}
	}

void TemperatureSensor::TimedOutState()
	{
	if (HasElapsed(waitAfterError))
		ChangeState(Idle);
	}

void TemperatureSensor::AbsentState()
	{
	// Thsi is a dead state, the sensor will never produce any readings.
	}

float TemperatureSensor::GetValue()
	{
	return readings.average;
	}
