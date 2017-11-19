#include "CommandProcessor.h"

CalibrationStateCommandProcessor::CalibrationStateCommandProcessor(char targetDevice, Calibration& status)
	{
	this->calibration = &status;
	this->deviceAddress = targetDevice;
	this->commandVerb = "CR";
	}

Response CalibrationStateCommandProcessor::Execute(Command& command)
	{
	return Response::FromPosition(command, (uint32_t)calibration->status);
	}