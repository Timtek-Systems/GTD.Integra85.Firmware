// ForceSensitiveResistor.h

#ifndef _FORCESENSITIVERESISTOR_h
#define _FORCESENSITIVERESISTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "MovingAverage.h"

class ForceSensitiveResistor
	{
	private:
		MovingAverage movingAverage;
		uint8_t channel;				// Analog input channel
	public:
		ForceSensitiveResistor(uint8_t analogPin);
		void Loop();
		int Value();
	};

#endif

