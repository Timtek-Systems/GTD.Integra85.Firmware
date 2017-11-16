#include "CommandProcessor.h"

PositionReadCommandProcessor::PositionReadCommandProcessor(char targetDevice, Motor& motor)
	{
	this->motor = &motor;
	this->deviceAddress = targetDevice;
	this->commandVerb = "PR";
	}

Response PositionReadCommandProcessor::Execute(Command& command)
	{
	auto position = motor->CurrentPosition() / MICROSTEPS_PER_STEP;
	auto response = Response::FromPosition(command, position);
	return response;
	}