#include "CalibrationStateMachine.h"

DelayAfterBackOutCalibrationState::DelayAfterBackOutCalibrationState()
	{
	delayDuration = 1000;
	StateName = "DelayAfterBackOut";
	}

void DelayAfterBackOutCalibrationState::TransitionToNextState(CalibrationStateMachine & machine)
	{
	machine.ChangeState(FindSoftLimitCalibrationState::GetInstance());
	}

ICalibrationState& DelayAfterBackOutCalibrationState::GetInstance()
	{
	static DelayAfterBackOutCalibrationState instance;
	return instance;
	}