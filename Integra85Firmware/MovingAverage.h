// MovingAverage.h

#ifndef _MOVINGAVERAGE_h
#define _MOVINGAVERAGE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class MovingAverage
{
private:
	unsigned long *samples;
	unsigned long runningTotal;
	int numSamples, addIndex, removeIndex;
 public:
	void init(unsigned long initialValue);
	MovingAverage();
	MovingAverage(int numSamples);
	unsigned long addSample(unsigned long sample);
	unsigned long average;
	};
#endif

