// PersistentSettings.h

#ifndef _PERSISTENTSETTINGS_h
#define _PERSISTENTSETTINGS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Integra85.h"
#include "Motor.h"

struct PersistentSettings
	{
	public:
		MotorSettings focuserSettings 
			{
			M1_MAX_POSITION,	// maxPosition
			M1_MAX_POSITION,	// currentPosition
			M1_RAMP_TIME,		// Ramp time (seconds)
			M1_MAX_SPEED		// Max speed (steps per sec)
			};
		MotorSettings rotatorSettings
			{
			M2_MAX_POSITION,	// maxPosition
			M2_MAX_POSITION,	// currentPosition
			M2_RAMP_TIME,		// Ramp time (seconds)
			M2_MAX_SPEED		// Max speed (steps per sec)
			};
		uint32_t focuserBacklash = 0;
	};


#endif

