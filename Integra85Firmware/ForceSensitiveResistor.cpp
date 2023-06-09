// 
// 
// 

#include "ForceSensitiveResistor.h"

ForceSensitiveResistor::ForceSensitiveResistor(uint8_t analogChannel)
	{
	movingAverage = MovingAverage<int>(5);
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
	Gets the current value, which is a moving average over a number of samples.
*/
int ForceSensitiveResistor::AverageValue()
	{
	return movingAverage.average;
	}
