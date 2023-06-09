
#include "CommandProcessor.h"

bool Command::IsMotorCommand()
	{
	return TargetDevice == '1' || TargetDevice == '2';
	}

bool Command::IsSystemCommand()
	{
	return TargetDevice == '0';
	}

CommandProcessor::CommandProcessor(Motor& focuser, Motor& rotator, CalibrationStateMachine& calibrator, PersistentSettings& settings, TemperatureSensor& temperature, ForceSensitiveResistor& fsr)
	{
	this->focuser = &focuser;
	this->rotator = &rotator;
	this->calibrator = &calibrator;
	this->settings = &settings;
	this->temperature = &temperature;
	this->fsr = &fsr;
	}

Motor* CommandProcessor::GetMotor(Command& command)
	{
	auto address = command.TargetDevice;
	return address == '1' ? focuser : rotator;
	}

Response CommandProcessor::HandleCommand(Command& command)
	{
	if (command.IsMotorCommand())
		{
		if (command.Verb == "MI") return HandleMI(command);	// Move motor in
		if (command.Verb == "MO") return HandleMO(command);	// Move motor out
		if (command.Verb == "AW") return HandleAW(command);	// Set limit of travel
		if (command.Verb == "BR") return HandleBR(command);	// Set limit of travel
		if (command.Verb == "BW") return HandleBW(command);	// Set limit of travel
		if (command.Verb == "CS") return HandleCS(command);	// Calibration start
		if (command.Verb == "CR") return HandleCR(command);	// Read calibration state
		if (command.Verb == "CE") return HandleCE(command);	// Abort calibration
		if (command.Verb == "Cl") return HandleCl(command);	// Set FSR low threshold
		if (command.Verb == "CL") return HandleCL(command);	// Set FSR high threshold
		if (command.Verb == "Cv") return HandleCv(command);	// Set calibration slow speed
		if (command.Verb == "SW") return HandleSW(command);	// Stop motor
		if (command.Verb == "RF") return HandleRF(command);	// Read FSR value (actually a moving average value)
		if (command.Verb == "Rl") return HandleRl(command);	// Read FSR low threshold (soft limit)
		if (command.Verb == "RL") return HandleRL(command);	// Read FSR high threshold (hard limit)
		if (command.Verb == "PR") return HandlePR(command);	// Position read
		if (command.Verb == "PW") return HandlePW(command);	// Position write (sync)
		if (command.Verb == "Rl") return HandleRF(command);	// Read FSR current value
		if (command.Verb == "Rl") return HandleRl(command);	// Read FSR low threshold
		if (command.Verb == "RL") return HandleRL(command);	// Read FSR high threshold
		if (command.Verb == "RR") return HandleRR(command);	// Range Read (get limit of travel)
		if (command.Verb == "RW") return HandleRW(command);	// Range Write (set limit of travel)
		if (command.Verb == "VR") return HandleVR(command);	// Read maximum motor speed
		if (command.Verb == "VW") return HandleVW(command);	// Read maximum motor speed

		}
	if (command.IsSystemCommand())
		{
		if (command.Verb == "ER") return HandleER(command);	// Read force-sensitive resistor
		if (command.Verb == "FR") return HandleFR(command);	// Read firmware version
		if (command.Verb == "TR") return HandleTR(command);	// Read temperature probe
		if (command.Verb == "X") return HandleX(command);	// Get movement status
		if (command.Verb == "ZD") return HandleZD(command);	// Reset to factory settings (load defaults).
		if (command.Verb == "ZR") return HandleZR(command);	// Load settings from persistent storage
		if (command.Verb == "ZW") return HandleZW(command);	// Write settings to persistent storage
		}
	return Response::Error();
	}

Response CommandProcessor::HandleMI(Command& command)
	{
	// Commands are in whole steps, motors operate in microsteps, so we must convert.
	auto motor = GetMotor(command);
	auto microStepsToMove = StepsToMicrosteps(command.StepPosition);
	auto targetPosition = motor->CurrentPosition() - microStepsToMove;
	if (targetPosition < 0)
		return Response::Error();
	motor->MoveToPosition(targetPosition);
	return Response::FromSuccessfulCommand(command);
	}

Response CommandProcessor::HandleMO(Command& command)
	{
	// Commands are in whole steps, motors operate in microsteps, so we must convert.
	auto motor = GetMotor(command);
	auto microStepsToMove = StepsToMicrosteps(command.StepPosition);
	auto targetPosition = motor->CurrentPosition() + microStepsToMove;
	if (targetPosition > motor->LimitOfTravel())
		return Response::Error();
	motor->MoveToPosition(targetPosition);
	return Response::FromSuccessfulCommand(command);
	}

Response CommandProcessor::HandleAW(Command & command)
	{
	auto motor = GetMotor(command);
	auto rampTime = command.StepPosition;
	// The minimum ramp time is 100ms, fail if the user tries to set it lower.
	if (rampTime < MIN_RAMP_TIME)
		return Response::Error();
	motor->SetRampTime(rampTime);
	return Response::FromSuccessfulCommand(command);
	}

Response CommandProcessor::HandleCS(Command & command)
	{
	// Calibration is only valis for the focuser.
	if (command.TargetDevice != '1')
		return Response::Error();
	auto motor = GetMotor(command);
	motor->HardStop();
	calibrator->StartCalibration();
	return Response::FromSuccessfulCommand(command);
	}

Response CommandProcessor::HandleCR(Command & command)
	{
	if (command.TargetDevice != '1')
		return Response::Error();
	return Response::FromPosition(command, (uint32_t)settings->calibration.status);
	}

Response CommandProcessor::HandleCE(Command & command)
	{
	if (command.TargetDevice != '1')
		return Response::Error();
	calibrator->StopCalibration();
	return Response::FromSuccessfulCommand(command);
	}

// Sets the FSR lower threshold, which is the point where backlash is measured.
Response CommandProcessor::HandleCl(Command & command)
{
	if (command.TargetDevice != '1')
		return Response::Error();
	auto limit = command.StepPosition;
	if (limit > 1023)
		return Response::Error();
	settings->calibration.lowThreshold = limit;
	return Response::FromSuccessfulCommand(command);
}

// Sets the FSR high threshold, which is the "hard stop" position.
Response CommandProcessor::HandleCL(Command & command)
{
	if (command.TargetDevice != '1')
		return Response::Error();
	auto limit = command.StepPosition;
	if (limit > 1023)
		return Response::Error();
	settings->calibration.highThreshold = limit;
	return Response::FromSuccessfulCommand(command);
}

Response CommandProcessor::HandleCv(Command & command)
{
	if (command.TargetDevice != '1')
		return Response::Error();
	auto speed = command.StepPosition;
	settings->calibration.slowSpeed = speed;
	return Response::FromSuccessfulCommand(command);
}

Response CommandProcessor::HandleCW(Command & command)
{
	if (command.TargetDevice != '1')
		return Response::Error();
	if (calibrator->InProgress())
		return Response::Error();
	switch (command.StepPosition)
	{
	case 0:
		calibrator->SetUncalibrated();
		return Response::FromSuccessfulCommand(command);
	case 1:
		calibrator->SetCalibrated();
		return Response::FromSuccessfulCommand(command);
	default:
		return Response::Error();
		break;
	}
}

// Read the value of the Force Sensitive Resistor (FSR)
Response CommandProcessor::HandleER(Command & command)
{
	auto sensorValue = fsr->AverageValue();
	return Response::FromInteger(command, sensorValue);
}

Response CommandProcessor::HandleSW(Command & command)
	{
	auto motor = GetMotor(command);
	motor->HardStop();
	return Response::FromSuccessfulCommand(command);
	}

Response CommandProcessor::HandleZW(Command & command)
	{
	settings->Save();
	return Response::FromSuccessfulCommand(command);
	}

Response CommandProcessor::HandleZR(Command & command)
{
	*settings = PersistentSettings::Load();
	return Response::FromSuccessfulCommand(command);
}

Response CommandProcessor::HandleZD(Command & command)
	{
	*settings = PersistentSettings();
	settings->Save();
	return Response::FromSuccessfulCommand(command);
	}

Response CommandProcessor::HandlePR(Command & command)
	{
	auto motor = GetMotor(command);
	auto position = MicrostepsToSteps(motor->CurrentPosition());
	auto response = Response::FromPosition(command, position);
	return response;
	}

Response CommandProcessor::HandlePW(Command & command)
{
	auto microsteps = StepsToMicrosteps(command.StepPosition);
	auto motor = GetMotor(command);
	motor->SetCurrentPosition(microsteps);
	return Response::FromSuccessfulCommand(command);
}

Response CommandProcessor::HandleRF(Command& command)
{
	if (command.TargetDevice != '1')
		return Response::Error();
	return Response::FromInteger(command, fsr->AverageValue());
}

Response CommandProcessor::HandleRL(Command& command) 
	{
	if (command.TargetDevice != '1')
		return Response::Error();
	return Response::FromInteger(command, settings->calibration.highThreshold);
	}

Response CommandProcessor::HandleRl(Command& command) 
	{
	if (command.TargetDevice != '1')
		return Response::Error();
	return Response::FromInteger(command, settings->calibration.lowThreshold);
	}

Response CommandProcessor::HandleRW(Command & command)
{
	auto microsteps = StepsToMicrosteps(command.StepPosition);
	auto motor = GetMotor(command);
	motor->SetLimitOfTravel(microsteps);
	return Response::FromSuccessfulCommand(command);
}

Response CommandProcessor::HandleRR(Command & command)
	{
	auto motor = GetMotor(command);
	auto range = MicrostepsToSteps(motor->LimitOfTravel());
	return Response::FromPosition(command, range);
	}

Response CommandProcessor::HandleFR(Command & command)
	{
	return Response{ "FR"+(String)FIRMWARE_MAJOR_VERSION + "." + (String)FIRMWARE_MINOR_VERSION + "#" };
	}

Response CommandProcessor::HandleTR(Command & command)
	{
	auto celsius = temperature->GetValue();
	return Response{ "TR" + (String)celsius + "#" };
	}

Response CommandProcessor::HandleVR(Command & command)
{
	auto motor = GetMotor(command);
	auto maxSpeed = motor->MaximumSpeed();
	return Response::FromPosition(command, MicrostepsToSteps(maxSpeed));
}

Response CommandProcessor::HandleVW(Command & command)
{
	auto motor = GetMotor(command);
	uint16_t speed = StepsToMicrosteps(command.StepPosition);
	if (speed < motor->MinimumSpeed())
		return Response::Error();
	motor->SetMaximumSpeed(speed);
	return Response::FromSuccessfulCommand(command);
}

Response CommandProcessor::HandleBR(Command & command)
{
	if (command.TargetDevice != '1')
		return Response::Error();
	auto backlash = settings->calibration.backlash;
	return Response::FromPosition(command, MicrostepsToSteps(backlash));
}

Response CommandProcessor::HandleBW(Command & command)
{
	if (command.TargetDevice != '1')
		return Response::Error();
	auto motor = GetMotor(command);
	auto backlash = StepsToMicrosteps(command.StepPosition);
	if (backlash > (motor->LimitOfTravel() / 2))
		return Response::Error();
	settings->calibration.backlash = backlash;
	return Response::FromSuccessfulCommand(command);
}

Response CommandProcessor::HandleX(Command & command)
	{
	if (focuser->IsMoving())
		return Response::FromInteger(command, 1);
	if (rotator->IsMoving())
		return Response::FromInteger(command, 2);
	return Response::FromInteger(command, 0);
	}

int32_t CommandProcessor::MicrostepsToSteps(int32_t microsteps)
	{
	return microsteps / MICROSTEPS_PER_STEP;
	}

int32_t CommandProcessor::StepsToMicrosteps(int32_t wholesteps)
	{
	return wholesteps * MICROSTEPS_PER_STEP;
	}
