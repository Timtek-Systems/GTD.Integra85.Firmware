// MovingAverage.h

#ifndef _MOVINGAVERAGE_h
#define _MOVINGAVERAGE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <ArduinoSTL.h>

template <class sampleType> 
class MovingAverage
{
private:
	std::vector<sampleType> samples;
	sampleType runningTotal;
	int numSamples, addIndex, removeIndex;
 public:
	void init(sampleType initialValue);
	MovingAverage<sampleType>();
	MovingAverage<sampleType>(int numSamples);
	sampleType addSample(sampleType sample);
	sampleType average;
	};

// Implementation

template <class sampleType>
void MovingAverage<sampleType>::init(sampleType initialValue)
	{
	samples.clear();
	for (int i = 0; i < numSamples; i++)
		{
		samples.push_back(initialValue);
		}
	runningTotal = initialValue * numSamples;
	average = initialValue;
	}

template<class sampleType>
MovingAverage<sampleType>::MovingAverage() : MovingAverage<sampleType>(5)
	{
	}

template<class sampleType>
MovingAverage<sampleType>::MovingAverage(int window)
	{
	numSamples = window;
	samples = std::vector<sampleType>();
	init(sampleType());
	}

template<class sampleType>
sampleType MovingAverage<sampleType>::addSample(sampleType sample)
	{
	runningTotal -= samples.front();
	runningTotal += sample;
	samples.erase(samples.begin());
	samples.push_back(sample);
	average = runningTotal / numSamples;
	return average;
	}
#endif

