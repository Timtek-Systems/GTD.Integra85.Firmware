#include "CommandProcessor.h"

VersionReadCommandProcessor::VersionReadCommandProcessor(char targetDevice)
	{
	this->deviceAddress = targetDevice;
	this->commandVerb = "VR";
	}

Response VersionReadCommandProcessor::Execute(Command& command)
	{
	return Response{ (String)FIRMWARE_MAJOR_VERSION + "." + (String)FIRMWARE_MINOR_VERSION + "#"};
	}