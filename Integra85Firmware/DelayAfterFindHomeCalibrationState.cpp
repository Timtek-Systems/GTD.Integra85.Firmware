
#include "CalibrationStateMachine.h"

DelayAfterFindHomeCalibrationState::DelayAfterFindHomeCalibrationState()
	{
	delayDuration = 1000;
	StateName = "DelayAfterFindHome";
	}

void DelayAfterFindHomeCalibrationState::TransitionToNextState(CalibrationStateMachine & machine)
	{
	machine.ChangeState(BackOutCalibrationState::GetInstance());
	}

ICalibrationState& DelayAfterFindHomeCalibrationState::GetInstance()
	{
	static DelayAfterFindHomeCalibrationState instance;
	return instance;
	}