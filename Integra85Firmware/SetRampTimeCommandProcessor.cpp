#include "CommandProcessor.h"

SetRampTimeCommandProcessor::SetRampTimeCommandProcessor(char targetDevice, Motor& motor)
	{
	this->motor = &motor;
	this->deviceAddress = targetDevice;
	this->commandVerb = "RW";
	}

Response SetRampTimeCommandProcessor::Execute(Command& command)
	{
	auto rampTime = command.StepPosition;
	// The minimum ramp time is 100ms, fail if the user tries to set it lower.
	if (rampTime < MIN_RAMP_TIME)
		return Response::Fail();
	motor->SetRampTime(rampTime);
	return Response::FromSuccessfulCommand(command);
	}