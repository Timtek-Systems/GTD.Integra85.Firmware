
#include "CalibrationStateMachine.h"
#include "CommandProcessor.h"

/*
Moves the focuser at full speed to the midpoint of travel.
*/

/*
	Wait until the motor has stopped at the midpoint
*/
void FindMidpointCalibrationState::Loop(CalibrationStateMachine & machine)
	{
	if (!machine.stepper->IsMoving())
		{
		auto writeSettings = Command { "ZW" };
		CommandDispatcher::Dispatch(writeSettings);	// Write persistent settings
		machine.ChangeState(new IdleCalibrationState());
		}
	}

void FindMidpointCalibrationState::OnEnter(CalibrationStateMachine & machine)
	{
	machine.CommitCalibration();
	machine.stepper->MoveToPosition(machine.stepper->MidpointPosition());
	}
