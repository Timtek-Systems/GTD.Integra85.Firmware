
#include "CalibrationStateMachine.h"

/*
Moves the focuser at full speed to the midpoint of travel.
*/

ICalibrationState & FindMidpointCalibrationState::GetInstance()
	{
	static FindMidpointCalibrationState instance;
	return instance;
	}

/*
	Wait until the motor has stopped at the midpoint
*/
void FindMidpointCalibrationState::Loop(CalibrationStateMachine & machine)
	{
	auto position = machine.stepper->CurrentPosition();
	if (!machine.stepper->IsMoving())
		{
		machine.CalibrationComplete();
		machine.ChangeState(IdleCalibrationState::GetInstance());
		}
	}

void FindMidpointCalibrationState::OnEnter(CalibrationStateMachine & machine)
	{
	machine.stepper->MoveToPosition(machine.stepper->MidpointPosition());
	}

FindMidpointCalibrationState::FindMidpointCalibrationState()
	{
	}
