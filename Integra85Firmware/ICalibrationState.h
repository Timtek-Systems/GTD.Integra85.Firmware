// ICalibrationState.h

#ifndef _ICALIBRATIONSTATE_h
#define _ICALIBRATIONSTATE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class ICalibrationState
	{
	public:
		virtual void Loop() = 0;
	};

#endif

