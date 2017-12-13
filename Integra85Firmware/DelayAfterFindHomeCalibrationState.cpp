
#include "CalibrationStateMachine.h"

DelayAfterFindHomeCalibrationState::DelayAfterFindHomeCalibrationState()
	{
	delayDuration = 1000;
	}

void DelayAfterFindHomeCalibrationState::TransitionToNextState(CalibrationStateMachine & machine)
	{
	machine.ChangeState(new BackOutCalibrationState());
	}
