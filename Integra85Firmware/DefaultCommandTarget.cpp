/*
	A command target that handles commands without a device address
*/

#include "CommandProcessor.h"

DefaultCommandTarget::DefaultCommandTarget(char targetDevice, PersistentSettings& settings, Motor& focuser, Motor& rotator)
	{
	//commandProcessors.push_back(new SaveSettingsCommandProcessor(targetDevice, settings));
	//commandProcessors.push_back(new IsMovingCommandProcessor(targetDevice, focuser, rotator));
	//commandProcessors.push_back(new VersionReadCommandProcessor(targetDevice));
	}
