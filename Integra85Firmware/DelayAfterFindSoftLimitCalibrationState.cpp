#include "CalibrationStateMachine.h"

DelayAfterFindSoftLimitCalibrationState::DelayAfterFindSoftLimitCalibrationState()
	{
	delayDuration = 1000;
	}

void DelayAfterFindSoftLimitCalibrationState::TransitionToNextState(CalibrationStateMachine & machine)
	{
	machine.ChangeState(FindBacklashCalibrationState::GetInstance());
	}

ICalibrationState& DelayAfterFindSoftLimitCalibrationState::GetInstance()
	{
	static DelayAfterFindSoftLimitCalibrationState instance;
	return instance;
	}