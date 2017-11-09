
#include "CalibrationStateMachine.h"

/*
Moves the focuser slowly out for 100,000 microsteps. During the move, the force on the FSR
is monitored until it drops below the soft limit threshold, at which point the position is recorded.
Backlash can then be computed.
*/

ICalibrationState & FindBacklashCalibrationState::GetInstance()
	{
	static FindBacklashCalibrationState instance;
	return instance;
	}

/*
Move in until the FSR reaches the soft threshold, and record that position.
Continue to move in until the hard stop is reached.
*/
void FindBacklashCalibrationState::Loop(CalibrationStateMachine & machine)
	{
	auto sensorValue = machine.sensor->MovingAverage();
	auto position = machine.stepper->CurrentPosition();
	if (sensorValue <= FSR_SOFT_THRESHOLD && softLimitPosition == 0)
		{
		softLimitPosition = position;
		}
	if (position >= 100000)
		{
		machine.stepper->HardStop();
		machine.calibrationDistanceMovingOut = softLimitPosition;
		machine.stepper->SetCurrentPosition(0);	// We are now at the "hard stop" position.
		machine.ChangeState(FindMidpointCalibrationState::GetInstance());
		}
	}

void FindBacklashCalibrationState::OnEnter(CalibrationStateMachine & machine)
	{
	softLimitPosition = 0;
	machine.stepper->MoveAtVelocity(+2880);
	}

FindBacklashCalibrationState::FindBacklashCalibrationState()
	{
	StateName = "FindBacklash";
	}
