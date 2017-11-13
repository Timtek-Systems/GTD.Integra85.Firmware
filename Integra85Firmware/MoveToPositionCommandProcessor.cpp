#include "CommandProcessor.h"

MoveToPositionCommandProcessor::MoveToPositionCommandProcessor(char address, Motor& motor)
	{
	this->motor = &motor;
	this->deviceAddress = address;
	this->commandVerb = 'M';
	}

Response MoveToPositionCommandProcessor::Execute(Command & command)
	{
	motor->MoveToPosition(command.Position);
	return Response::Ok();
	}
