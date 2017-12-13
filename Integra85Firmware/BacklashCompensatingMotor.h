// BacklashCompensatingMotor.h

#ifndef _BACKLASHCOMPENSATINGMOTOR_h
#define _BACKLASHCOMPENSATINGMOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Motor.h"
#include "CalibrationStateMachine.h"

class BacklashCompensatingMotor : public Motor
	{
	public:
		BacklashCompensatingMotor(uint8_t stepPin, uint8_t enablePin, uint8_t directionPin, IStepGenerator& stepper, MotorSettings& settings, Calibration& calibration);
		virtual void MoveToPosition(int32_t targetPosition) final;

	private:
		Calibration *calibration;
	};

#endif

