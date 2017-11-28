#include "CalibrationStateMachine.h"

void BackOutCalibrationState::Loop(CalibrationStateMachine& machine)
	{
	if (!machine.stepper->IsMoving())
		machine.ChangeState(DelayAfterBackOutCalibrationState::GetInstance());
	}

void BackOutCalibrationState::OnEnter(CalibrationStateMachine & machine)
	{
	machine.stepper->MoveToPosition(CALIBRATE_SAFE_DISTANCE);
	}

BackOutCalibrationState::BackOutCalibrationState()
	{
	}

ICalibrationState& BackOutCalibrationState::GetInstance()
	{
	static BackOutCalibrationState instance;
	return instance;
	}
