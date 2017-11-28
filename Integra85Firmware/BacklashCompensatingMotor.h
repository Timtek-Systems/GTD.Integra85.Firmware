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

enum BacklashState
	{
	Stopped,
	MovingIn,
	MovingOut,
	Compensating
	};

class BacklashCompensatingMotor : public Motor
	{
	public:
		BacklashCompensatingMotor(uint8_t stepPin, uint8_t enablePin, uint8_t directionPin, IStepGenerator& stepper, MotorSettings& settings, Calibration& calibration);
		virtual void MoveToPosition(uint32_t targetPosition) final;
		virtual void Loop() final;
		virtual const bool IsMoving() override;
		virtual void HardStop() override;

	private:
		void ChangeState(BacklashState newState);
		Calibration *calibration;
		BacklashState backlashState = Stopped;
		void MovingInState(), MovingOutState(), CompensatingState();
	};

#endif

