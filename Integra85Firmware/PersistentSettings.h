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
		struct MotorSettings focuser;
		struct MotorSettings rotator;
		uint32_t focuserBacklash;

		PersistentSettings()
			{
			focuser = {
				M1_MAX_POSITION,	// maxPosition
				M1_MAX_POSITION,	// currentPosition
				M1_RAMP_TIME,		// Ramp time (seconds)
				M1_MAX_SPEED		// Max speed (steps per sec)
				};
			rotator = {
				M2_MAX_POSITION,	// maxPosition
				M2_MAX_POSITION,	// currentPosition
				M2_RAMP_TIME,		// Ramp time (seconds)
				M2_MAX_SPEED		// Max speed (steps per sec)
				};
			focuserBacklash = 0;
			}
	};


#endif

