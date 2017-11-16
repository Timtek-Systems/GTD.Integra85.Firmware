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
#include "CalibrationStateMachine.h"

struct PersistentSettings
	{
	static const uint16_t fingerprint = 0x4AFB;
	static const uint8_t majorVersion = FIRMWARE_MAJOR_VERSION;
	static const uint8_t minorVersion = FIRMWARE_MINOR_VERSION;

	struct MotorSettings focuser;
	struct MotorSettings rotator;
	struct Calibration calibration;

	PersistentSettings();
	static PersistentSettings Load();
	void Save();
	};


#endif

