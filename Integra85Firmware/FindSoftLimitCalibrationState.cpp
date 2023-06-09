
#include "CalibrationStateMachine.h"

/*
Moves the focuser at full speed towards the hard stop. During the move, the CurrentPosition
of the motor is kept at the midpoint, to prevent it from exceeding safety limits.
*/

/*
	Move in until the FSR reaches the soft threshold, and record that position.
	Continue to move in until the hard stop is reached.
*/
void FindSoftLimitCalibrationState::Loop(CalibrationStateMachine & machine)
	{
	auto sensorValue = machine.sensor->AverageValue();
	auto position = machine.stepper->CurrentPosition();
	if (sensorValue >= machine.status->lowThreshold && softLimitPosition == 0)
		{
		softLimitPosition = position;
		}
	if (sensorValue >= machine.status->highThreshold)
		{
		machine.stepper->HardStop();
		auto distanceBetweenHardAndSoftLimits = softLimitPosition - position;
		machine.calibrationDistanceMovingIn=distanceBetweenHardAndSoftLimits;
		machine.stepper->SetCurrentPosition(0);	// We are now at the "hard stop" position.
		machine.ChangeState(new DelayAfterFindSoftLimitCalibrationState());
		}
	}

void FindSoftLimitCalibrationState::OnEnter(CalibrationStateMachine & machine)
	{
	softLimitPosition = 0;
	machine.calibrationDistanceMovingIn = 0;
	machine.calibrationDistanceMovingOut = 0;
	machine.stepper->SetCurrentPosition(machine.stepper->MidpointPosition());
	machine.stepper->MoveAtVelocity(-(int)(machine.status->slowSpeed));
	}
