#include "CalibrationStateMachine.h"

void BackOutCalibrationState::Loop(CalibrationStateMachine& machine)
	{
	if (!machine.stepper->IsMoving())
		machine.ChangeState(new DelayAfterBackOutCalibrationState());
	}

void BackOutCalibrationState::OnEnter(CalibrationStateMachine & machine)
	{
	machine.stepper->MoveToPosition(CALIBRATE_SAFE_DISTANCE);
	}
