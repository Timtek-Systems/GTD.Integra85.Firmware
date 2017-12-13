
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
		machine.CalibrationComplete();
		auto writeSettings = Command{ "ZW",'0',0 };
		DispatchCommand(writeSettings);
		machine.ChangeState(new IdleCalibrationState());
		}
	}

void FindMidpointCalibrationState::OnEnter(CalibrationStateMachine & machine)
	{
	machine.CommitCalibration();
	auto midpoint = machine.stepper->MidpointPosition();
	auto deltaMicrosteps = midpoint - machine.stepper->CurrentPosition();
	auto deltaWholeSteps = deltaMicrosteps / MICROSTEPS_PER_STEP;
	auto moveCommand = Command{ "MO",'1', deltaWholeSteps };
	::DispatchCommand(moveCommand);
	}
