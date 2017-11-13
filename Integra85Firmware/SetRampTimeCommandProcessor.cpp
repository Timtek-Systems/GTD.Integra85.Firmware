#include "CommandProcessor.h"

SetRampTimeCommandProcessor::SetRampTimeCommandProcessor(char targetDevice, Motor& motor)
	{
	this->motor = &motor;
	this->deviceAddress = targetDevice;
	this->commandVerb = "RW";
	}

Response SetRampTimeCommandProcessor::Execute(Command& command)
	{
	// The command payload is the ramp time in milliseconds, we must convert to seconds.
	float rampTimeSeconds = command.StepPosition / 1000.0;
	// The minimum ramp time is 0.1 seconds, fail if the user tries to set it lower.
	if (rampTimeSeconds < 0.1)
		return Response::Fail();
	motor->SetRampTime(rampTimeSeconds);
	return Response::Ok();
	}