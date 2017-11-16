
#include "CommandProcessor.h"

SaveSettingsCommandProcessor::SaveSettingsCommandProcessor(char deviceAddress, PersistentSettings& settings)
	{
	this->deviceAddress = deviceAddress;
	this->settings = &settings;
	this->commandVerb = "ZW";
	}

Response SaveSettingsCommandProcessor::Execute(Command & command)
	{
	settings->Save();
	return Response::FromSuccessfulCommand(command);
	}
