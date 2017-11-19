#include "CommandProcessor.h"

StopMotorCommandProcessor::StopMotorCommandProcessor(char targetDevice, Motor& motor)
	{
	this->motor = &motor;
	this->deviceAddress = targetDevice;
	this->commandVerb = "SW";
	}

Response StopMotorCommandProcessor::Execute(Command& command)
	{
	motor->HardStop();
	return Response::FromSuccessfulCommand(command);
	}