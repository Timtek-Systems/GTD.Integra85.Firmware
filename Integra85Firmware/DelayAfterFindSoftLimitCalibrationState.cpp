#include "CalibrationStateMachine.h"

DelayAfterFindSoftLimitCalibrationState::DelayAfterFindSoftLimitCalibrationState()
	{
	delayDuration = 1000;
	}

void DelayAfterFindSoftLimitCalibrationState::TransitionToNextState(CalibrationStateMachine & machine)
	{
	machine.ChangeState(new FindBacklashCalibrationState());
	}
