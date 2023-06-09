// 
// 
// 

#include "PersistentSettings.h"
#include <eeprom.h>

PersistentSettings::PersistentSettings()
	{
	focuser = {
		M1_MAX_POSITION,	// maxPosition
		M1_MIDPOINT,		// currentPosition
		M1_RAMP_TIME,		// Ramp time (seconds)
		M1_MAX_SPEED		// Max speed (steps per sec)
		};
	rotator = {
		M2_MAX_POSITION,	// maxPosition
		M2_MIDPOINT,		// currentPosition
		M2_RAMP_TIME,		// Ramp time (seconds)
		M2_MAX_SPEED		// Max speed (steps per sec)
		};
	calibration.backlash = 0;
	calibration.status = Uncalibrated;
	calibration.lowThreshold = FSR_SOFT_THRESHOLD;
	calibration.highThreshold = FSR_HARD_THRESHOLD;
	calibration.slowSpeed = CALIBRATE_SLOW_MOTION;
	}

/*
	Saves persistent settings to EEPROM.
	Uses update rather than write in an attempt to minimize unnecessary
	write cycles.
	Writes a 16-bit fingerprint into the first two EEPROM locations to indicate
	that valid settings exist.
*/
void PersistentSettings::Save()
	{
	uint16_t *destination = 0;
	auto source = (const byte *)this;
	auto byteCount = sizeof(PersistentSettings);
	eeprom_update_block(source, destination, byteCount);
	// Now write a "fingerprint" immediately after the settings.
	destination += sizeof(PersistentSettings);
	eeprom_update_word(destination, fingerprint);
	}

/*
	Loads and returns persistent settings from EEPROM.
	The fingerprint must be valid, and the loaded settings must have the same
	major version as the firmware version. If these conditions are not met
	then default settings will be used.
*/
PersistentSettings PersistentSettings::Load()
	{
	auto defaultSettings = PersistentSettings();
	uint16_t *source = 0;
	auto loadedSettings = PersistentSettings();
	eeprom_read_block(&loadedSettings, source, sizeof(PersistentSettings));
	// Read the fingerprint and make sure it is valid
	source += sizeof(PersistentSettings);
	auto eepromFingerprint = eeprom_read_word(source);
	if (eepromFingerprint != fingerprint)
		return defaultSettings;	// use defaults if fingerprint is invalid
	// Ensure that the firmware major version is the same as when the settings were saved.
	if (loadedSettings.majorVersion != FIRMWARE_MAJOR_VERSION)
		return defaultSettings;
	// All is well, we can return the loaded settings.
	return loadedSettings;
	}

