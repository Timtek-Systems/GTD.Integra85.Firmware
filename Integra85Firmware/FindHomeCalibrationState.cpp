
#include "CalibrationStateMachine.h"

/*
	Moves the focuser at full speed towards the hard stop. During the move, the CurrentPosition
	of the motor is kept at the midpoint, to prevent it from exceeding safety limits.
*/

void FindHomeCalibrationState::Loop(CalibrationStateMachine & machine)
	{
	auto sensorValue = machine.sensor->AverageValue();
	if (sensorValue >= FSR_HARD_THRESHOLD)
		{
		machine.stepper->HardStop();
		machine.stepper->SetCurrentPosition(0);	// We are now at the "hard stop" position.
		machine.ChangeState(new DelayAfterFindHomeCalibrationState());
		}
	else
		{
		machine.stepper->SetCurrentPosition(machine.stepper->MidpointPosition());
		}
	}

void FindHomeCalibrationState::OnEnter(CalibrationStateMachine & machine)
	{
	machine.startTime = millis();
	machine.status->backlash = 0;
	machine.stepper->SetCurrentPosition(machine.stepper->MidpointPosition());
	machine.stepper->MoveToPosition(0);
	}
