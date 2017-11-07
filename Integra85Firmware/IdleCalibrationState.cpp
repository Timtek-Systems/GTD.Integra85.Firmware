// 
// 
// 

#include "IdleCalibrationState.h"

void IdleCalibrationState::Loop()
	{
	// Do nothing.
	}

ICalibrationState& IdleCalibrationState::GetInstance()
	{
	static IdleCalibrationState instance;
	return instance;
	}
