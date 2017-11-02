// Motor.h

#ifndef _MOTOR_h
#define _MOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "IStepGenerator.h"
#include "IStepSequencer.h"

class Motor : public IStepSequencer
	{
	public:
		Motor();
		Motor(uint8_t stepPin, uint8_t enablePin, uint8_t directionPin, IStepGenerator *stepper);
		virtual void Step(bool state) final;
		void MoveAtVelocity(float stepsPerSecond);
		void EnergizeMotor();

	private:
		uint8_t stepPin, enablePin, directionPin;
		IStepGenerator *stepGenerator;
		int8_t direction = +1;
		uint32_t currentPosition, maxPosition;
		void InitializeHardware();
	};

#endif

