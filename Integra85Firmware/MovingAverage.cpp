#include "MovingAverage.h"

void MovingAverage::init(unsigned long initialValue)
{
	for (int i = 0; i < numSamples; i++)
		{
		samples[i] = initialValue;
		}
	addIndex = 0;
	removeIndex = 1;
	runningTotal = initialValue * numSamples;
	average = initialValue;
}

MovingAverage::MovingAverage() : MovingAverage(5) {}
MovingAverage::MovingAverage(int window) 
	{
	numSamples = window;
	samples = new unsigned long[numSamples];
	init(0);
	}

unsigned long MovingAverage::addSample(unsigned long sample)
	{
	runningTotal -= samples[removeIndex++];
	runningTotal += sample;
	samples[addIndex++] = sample;
	if (addIndex >= numSamples)
		addIndex = 0;
	if (removeIndex >= numSamples)
		removeIndex = 0;
	average = runningTotal / numSamples;
	return average;
	}


