
#include "CalibrationStateMachine.h"

/*
Moves the focuser slowly out for 100,000 microsteps. During the move, the force on the FSR
is monitored until it drops below the soft limit threshold, at which point the position is recorded.
Backlash can then be computed.
*/

/*
Move in until the FSR reaches the soft threshold, and record that position.
Continue to move in until the hard stop is reached.
*/
void FindBacklashCalibrationState::Loop(CalibrationStateMachine & machine)
	{
	auto sensorValue = machine.sensor->AverageValue();
	auto position = machine.stepper->CurrentPosition();
	if (sensorValue <= machine.status->lowThreshold && softLimitPosition == 0)
		{
		softLimitPosition = position;
		}
	if (position >= CALIBRATE_SAFE_DISTANCE)
		{
		machine.stepper->HardStop();
		machine.calibrationDistanceMovingOut = softLimitPosition;
		machine.ChangeState(new FindMidpointCalibrationState());
		}
	}

void FindBacklashCalibrationState::OnEnter(CalibrationStateMachine & machine)
	{
	softLimitPosition = 0;
	machine.stepper->MoveAtVelocity(machine.status->slowSpeed);
	}
