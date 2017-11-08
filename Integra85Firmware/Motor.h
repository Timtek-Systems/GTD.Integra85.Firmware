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
		void ReleaseMotor();
		void SetRampTime(float seconds);
		void HardStop();
		void ComputeAcceleratedVelocity();
		void MoveToPosition(uint32_t position);
		void SetCurrentPosition(uint32_t position);
		const float CurrentVelocity();
		const uint32_t CurrentPosition();
		const uint32_t MidpointPosition();

	private:
		uint8_t stepPin, enablePin, directionPin;
		IStepGenerator *stepGenerator;
		int8_t direction = +1;
		uint32_t currentPosition, targetPosition, maxPosition, midpointPosition;
		unsigned long startTime;
		float rampTime;
		float startVelocity, currentVelocity, targetVelocity, currentAcceleration;
		float maxSpeed, minSpeed;
		void InitializeHardware();
		float AcceleratedVelocity();
		float DeceleratedVelocity();
		float AccelerationFromRampTime();
	};

#endif

