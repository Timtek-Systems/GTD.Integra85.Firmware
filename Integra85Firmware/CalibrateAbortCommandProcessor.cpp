#include "CommandProcessor.h"

CalibrateAbortCommandProcessor::CalibrateAbortCommandProcessor(char targetDevice, CalibrationStateMachine& calibrator)
	{
	this->deviceAddress = targetDevice;
	this->machine = &calibrator;
	this->commandVerb = "CE";
	}

Response CalibrateAbortCommandProcessor::Execute(Command& command)
	{
	machine->StopCalibration();
	return Response::FromSuccessfulCommand(command);
	}