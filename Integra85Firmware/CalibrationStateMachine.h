// CalibrationStateMachine.h

#ifndef _CALIBRATIONSTATEMACHINE_h
#define _CALIBRATIONSTATEMACHINE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Motor.h"
#include "ForceSensitiveResistor.h"
#include "ICalibrationState.h"
#include "IdleCalibrationState.h"

class CalibrationStateMachine
	{
	public:
		CalibrationStateMachine(Motor *motor, ForceSensitiveResistor *limitSensor);
		void Loop();
	private:
		Motor *stepper;
		ForceSensitiveResistor *sensor;
		ICalibrationState *currentState;
	};

#endif

