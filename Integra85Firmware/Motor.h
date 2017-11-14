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
	uint32_t maxPosition;
	volatile uint32_t currentPosition;
	float rampTime;
	float maxSpeed;
	};

class Motor : public IStepSequencer
	{
	public:
		Motor(uint8_t stepPin, uint8_t enablePin, uint8_t directionPin);
		Motor(uint8_t stepPin, uint8_t enablePin, uint8_t directionPin, IStepGenerator *stepper, MotorSettings& settings);
		virtual void Step(bool state) final;
		void MoveAtVelocity(float stepsPerSecond);
		void EnergizeMotor();
		void ReleaseMotor();
		void SetRampTime(uint16_t milliseconds);
		void HardStop();
		void ComputeAcceleratedVelocity();
		void MoveToPosition(uint32_t position);
		void SetCurrentPosition(uint32_t position);
		const float CurrentVelocity();
		const uint32_t CurrentPosition();
		const uint32_t MidpointPosition();
		const uint32_t LimitOfTravel();

	private:
		MotorSettings *configuration;
		uint8_t stepPin, enablePin, directionPin;
		IStepGenerator *stepGenerator;
		int8_t direction = +1;
		uint32_t targetPosition;
		unsigned long startTime;
		float startVelocity, currentVelocity, targetVelocity, currentAcceleration;
		float minSpeed;
		void InitializeHardware();
		float AcceleratedVelocity();
		float DeceleratedVelocity();
		float AccelerationFromRampTime();
	};

#endif

