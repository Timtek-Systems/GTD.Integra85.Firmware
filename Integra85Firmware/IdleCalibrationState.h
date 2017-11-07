// IdleCalibrationState.h

#ifndef _IDLECALIBRATIONSTATE_h
#define _IDLECALIBRATIONSTATE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ICalibrationState.h"
#include "ISingleton.h"

class IdleCalibrationState : public ICalibrationState
	{
	public:
		// Inherited via ICalibrationState
		virtual void Loop() final;
		static ICalibrationState & GetInstance();
	};

#endif

