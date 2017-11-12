#include "CommandProcessor.h"

const char *okResponse = "1";
const char *failResponse = "0";

MoveToPositionCommandProcessor::MoveToPositionCommandProcessor(char address, Motor& motor)
	{
	this->motor = motor;
	this->deviceAddress = address;
	this->commandVerb = 'M';
	}

Response MoveToPositionCommandProcessor::Execute(Command & command)
	{
	motor.MoveToPosition(command.Position);
	return okResponse;
	}
