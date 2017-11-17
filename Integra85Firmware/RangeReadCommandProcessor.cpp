#include "CommandProcessor.h"

RangeReadCommandProcessor::RangeReadCommandProcessor(char targetDevice, Motor& motor)
	{
	this->motor = &motor;
	this->deviceAddress = targetDevice;
	this->commandVerb = "RR";
	}

Response RangeReadCommandProcessor::Execute(Command& command)
	{
	auto range = motor->LimitOfTravel() / MICROSTEPS_PER_STEP;
	return Response::FromPosition(command, range);
	}