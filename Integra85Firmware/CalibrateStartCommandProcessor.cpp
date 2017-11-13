#include "CommandProcessor.h"

CalibrateStartCommandProcessor::CalibrateStartCommandProcessor(char targetDevice, Motor& motor, CalibrationStateMachine& stateMachine)
	{
	this->deviceAddress = targetDevice;
	this->motor = &motor;
	this->calibrator = &stateMachine;
	this->commandVerb = "CS";
	}

Response CalibrateStartCommandProcessor::Execute(Command& command)
	{
	motor->HardStop();
	calibrator->StartCalibration();
	return Response::FromSuccessfulCommand(command);
	}