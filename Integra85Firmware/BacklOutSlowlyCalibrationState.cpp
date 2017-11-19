#include "CalibrationStateMachine.h"

void BackOutCalibrationState::Loop(CalibrationStateMachine& machine)
	{
	if (machine.stepper->CurrentVelocity() == 0)
		machine.ChangeState(DelayAfterBackOutCalibrationState::GetInstance());
	}

void BackOutCalibrationState::OnEnter(CalibrationStateMachine & machine)
	{
	machine.stepper->MoveToPosition(160000);
	}

BackOutCalibrationState::BackOutCalibrationState()
	{
	}

ICalibrationState& BackOutCalibrationState::GetInstance()
	{
	static BackOutCalibrationState instance;
	return instance;
	}
