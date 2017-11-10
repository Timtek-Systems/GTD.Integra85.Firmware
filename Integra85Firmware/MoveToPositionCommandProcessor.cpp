#include "CommandProcessor.h"

MoveToPositionCommandProcessor::MoveToPositionCommandProcessor(char address, Motor& motor)
	{
	this->motor = motor;
	this->deviceAddress = address;
	}

Response MoveToPositionCommandProcessor::Execute(Command & command)
	{
	motor.MoveToPosition(command.Position);
	return "OK"; // ToDo - this is not a real response
	}
