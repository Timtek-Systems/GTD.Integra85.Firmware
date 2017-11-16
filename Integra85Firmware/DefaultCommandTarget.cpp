/*
	A command target that handles commands without a device address
*/

#include "CommandProcessor.h"

DefaultCommandTarget::DefaultCommandTarget(char targetDevice, PersistentSettings& settings)
	{
	deviceAddress = targetDevice;
	commandProcessors.push_back(new SaveSettingsCommandProcessor(targetDevice, settings));
	}
