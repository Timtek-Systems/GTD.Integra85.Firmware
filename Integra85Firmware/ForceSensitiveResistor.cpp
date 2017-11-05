// 
// 
// 

#include "ForceSensitiveResistor.h"

ForceSensitiveResistor::ForceSensitiveResistor(uint8_t analogChannel)
	{
	movingAverage = MovingAverage(10);
	channel = analogChannel;
	}

/*
	Polls the current value. Call this from the Arduino main loop.
*/
void ForceSensitiveResistor::Loop()
	{
	auto currentValue = analogRead(channel);
	movingAverage.addSample(currentValue);
	}

/*
	Gets the current value.
*/
int ForceSensitiveResistor::Value()
	{
	return (int)movingAverage.average;
	}
