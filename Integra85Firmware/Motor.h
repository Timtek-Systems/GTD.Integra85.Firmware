// Motor.h

#ifndef _MOTOR_h
#define _MOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Integra85.h"
#include "IStepSequencer.h"
#include "IStepGenerator.h"

struct MotorSettings
	{
	uint32_t maxPosition;				// limit of travel, in steps
	volatile int32_t currentPosition;	// the current position (potentially updated by ISR)
	uint16_t rampTimeMilliseconds;		// milliseconds to ramp from minSpeed to maxSpeed
	uint16_t maxSpeed;					// maximum number of steps per second
	};

class Motor : public IStepSequencer
	{
	public:
		Motor(uint8_t stepPin, uint8_t enablePin, uint8_t directionPin);
		Motor(uint8_t stepPin, uint8_t enablePin, uint8_t directionPin, IStepGenerator& stepper, MotorSettings& settings);
		virtual void Step(bool state) final;
		void MoveAtVelocity(float stepsPerSecond);
		void EnergizeMotor();
		void ReleaseMotor();
		void SetRampTime(uint16_t milliseconds);
		virtual void HardStop();
		virtual void Loop();
		void ComputeAcceleratedVelocity();
		virtual void MoveToPosition(int32_t position);
		void SetCurrentPosition(int32_t position);
		void SetLimitOfTravel(uint32_t limit);
		void SetMaximumSpeed(uint16_t speed);
		const float CurrentVelocity();
		virtual const int32_t CurrentPosition();
		const int32_t MidpointPosition();
		const int32_t LimitOfTravel();
		const uint16_t MaximumSpeed();
		const uint16_t MinimumSpeed();
		virtual const bool IsMoving();

	protected:
		MotorSettings *configuration;

	private:
		uint8_t stepPin, enablePin, directionPin;
		IStepGenerator *stepGenerator;
		int8_t direction = +1;
		int32_t targetPosition;
		unsigned long startTime;
		float startVelocity, currentVelocity, targetVelocity, currentAcceleration;
		float minSpeed;
		void InitializeHardware();
		float AcceleratedVelocity();
		float DeceleratedVelocity();
		float AccelerationFromRampTime();
	};

#endif

