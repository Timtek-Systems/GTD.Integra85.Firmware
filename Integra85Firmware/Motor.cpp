/*
	Accelerated stepper motor control for Integra 85.

	Note: within this file the following definitions are used:
	Speed - measured in steps per second.
	Velocity - speed in a specified direction. Positive velocity results in increasing step position.
	Distance - measured in motor steps
	Time - measured in seconds
	Acceleration - measured in steps per second per second
*/

#include <Arduino.h>
#include "Integra85.h"
#include "Motor.h"
#include "IStepSequencer.h"
#include "IStepGenerator.h"

// Configures the I/O pins and sets a safe starting state.
void Motor::InitializeHardware()
	{
	pinMode(stepPin, OUTPUT);
	pinMode(directionPin, OUTPUT);
	pinMode(enablePin, OUTPUT);
	digitalWrite(enablePin, HIGH);
	}

Motor::Motor() {}

// Creates a new motor instance with the specified I/O pins and step generator.
Motor::Motor(uint8_t stepPin, uint8_t enablePin, uint8_t directionPin, IStepGenerator *stepper)
	{
	this->stepPin = stepPin;
	this->enablePin = enablePin;
	this->directionPin = directionPin;
	stepGenerator = stepper;
	currentPosition = 0;
	maxPosition = 200000;
	maxSpeed = 16000;	// Dictated by physical constraints
	minSpeed = 250;		// Below this speed there is a risk of timer overflow
	rampTime = 1.0;
	InitializeHardware();
	}

/*
The Step method will be called from an interrupt service routine, so
operations must be a short as possible and modify as little state as possible.
*/
void Motor::Step(bool state)
	{
	digitalWrite(stepPin, state ? HIGH : LOW);
	if (state)
		{
		// Increment position on leading edge.
		currentPosition += state ? direction : 0;
		}
	else
		{
		// Check hard limits on falling edge
		if (currentPosition == targetPosition || currentPosition >= maxPosition || currentPosition <= 0)
			{
			HardStop();
			}
		}
	}

// Moves the motor with the specified velocity. Movement continues until stopped or a hard limit is reached.
void Motor::MoveAtVelocity(float stepsPerSecond)
	{
	direction = stepsPerSecond < 0 ? -1 : +1;
	auto absoluteStepsPerSecond = abs(stepsPerSecond);
	EnergizeMotor();
	stepGenerator->Start(absoluteStepsPerSecond, this);
	}

// Energizes the motor coils (applies holding torque) and prepares for stepping.
void Motor::EnergizeMotor()
	{
	digitalWrite(stepPin, LOW);		// active high, so ensure we are not commanding a step.
	digitalWrite(directionPin, direction >= 0 ? HIGH : LOW);
	digitalWrite(enablePin, LOW);	// Active low, so energize the coils.
	}

// Disables the motor coils (releases holding torque).
void Motor::ReleaseMotor()
	{
	digitalWrite(enablePin, HIGH);	// active low, so de-energize the coils
	digitalWrite(stepPin, LOW);		// active high, so ensure we are not commanding a step.
	}

void Motor::SetRampTime(float seconds)
	{
	rampTime = seconds;
	}

/*
	Configures the motor to move to an absolute step position. Unless interrupted,
	the motor will commence stepping at minSpeed and will accelerate uniformly
	to maxSpeed. When nearing the target position, the motor will decelerate uniformly
	down to minSpeed and upon reaching the target position, will perform a hard stop.
	Note: for short moves the motor may never reach maxSpeed.
*/
void Motor::MoveToPosition(uint32_t position)
	{
	uint32_t deltaPosition = position - currentPosition;
	targetPosition = position;
	direction = sgn(deltaPosition);
	targetVelocity = maxSpeed * direction;
	currentAcceleration = AccelerationFromRampTime() * direction;
	startTime = millis();
	if (currentVelocity == 0)
		{
		// Starting from rest
		startVelocity = minSpeed * direction;
		stepGenerator->Start(startVelocity, this);
		}
	else
		{
		// Starting with the motor already in motion
		startVelocity = currentVelocity;
		stepGenerator->SetStepRate(startVelocity);
		}
	}

/*
	Computes the linear acceleration required to accelerate from rest to the maximum
	speed in the ramp time. The returned value is always positive.
	From v = u + at; since u is 0, v = at where t is the ramp time. Therefore, a = v/t.
*/
float Motor::AccelerationFromRampTime()
	{
	return maxSpeed / rampTime;
	}

/*
	Computes the theoretical accelerated velocity assuming uniform acceleration since start time.
	v = u + at
	u = startVelocity, a is acceleration, t is elapsed time since start
*/
float Motor::AcceleratedVelocity()
	{
	float elapsedTime = (millis() - startTime) / 1000.0;
	float acceleratedVelocity = startVelocity + currentAcceleration * elapsedTime; // v = u + at
	return acceleratedVelocity;
	}

/*
	Computes the maximum velocity that will still allow the motor to decelerate to minSpeed
	before reaching the target position. We do this by computing what the velocity would have been
	if we had started at the target position and accelerated back for n steps, then changing the sign of
	that velocity to match the current dorection of travel.
	v² = u² + 2as
	u² = minSpeed, a = |acceleration|, s = steps still to go
	|v| = √(2as + u²) (positive root)
*/
float Motor::DeceleratedVelocity()
	{
	uint32_t stepsRemaining = abs(targetPosition - currentPosition);
	float uSquared = minSpeed * minSpeed;
	float vSquared = uSquared + abs(2 * currentAcceleration * stepsRemaining);
	float speed = sqrtf(vSquared);
	float velocity = speed * direction;
	return velocity;
	}

/*
	Brings the motor to an immediate hard stop.
*/
void Motor::HardStop()
	{
	stepGenerator->Stop();
	currentAcceleration = 0;
	currentVelocity = 0;
	direction = 0;
	ReleaseMotor();
	}

/*
	Recomputes the current motor velocity. Call this from within the main loop.
*/
void Motor::ComputeAcceleratedVelocity()
	{
	float accelerationCurve = AcceleratedVelocity();
	float decelerationCurve = DeceleratedVelocity();
	float theoreticalVelocity = (abs(accelerationCurve) > abs(decelerationCurve)) ? decelerationCurve : accelerationCurve;
	float theoreticalSpeed = abs(theoreticalVelocity);
	float constrainedSpeed = constrain(theoreticalSpeed, maxSpeed, minSpeed);
	currentVelocity = constrainedSpeed * direction;
	stepGenerator->SetStepRate(currentVelocity);
	}
