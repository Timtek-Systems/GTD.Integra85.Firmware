
#include "CommandProcessor.h"

bool Command::IsMotorCommand()
	{
	return TargetDevice == '1' || TargetDevice == '2';
	}

bool Command::IsSystemCommand()
	{
	return TargetDevice == '0';
	}

CommandProcessor::CommandProcessor(Motor& focuser, Motor& rotator, CalibrationStateMachine& calibrator, PersistentSettings& settings)
	{
	this->focuser = &focuser;
	this->rotator = &rotator;
	this->calibrator = &calibrator;
	this->settings = &settings;
	//commandProcessors.push_back(new MoveInCommandProcessor(deviceAddress, motor));
	//commandProcessors.push_back(new MoveOutCommandProcessor(deviceAddress, motor));
	//commandProcessors.push_back(new SetRampTimeCommandProcessor(deviceAddress, motor));
	//commandProcessors.push_back(new CalibrateStartCommandProcessor(deviceAddress, motor, calibrator));
	//commandProcessors.push_back(new PositionReadCommandProcessor(deviceAddress, motor));
	//commandProcessors.push_back(new StopMotorCommandProcessor(deviceAddress, motor));
	//commandProcessors.push_back(new CalibrateAbortCommandProcessor(deviceAddress, calibrator));
	//commandProcessors.push_back(new RangeReadCommandProcessor(deviceAddress, motor));
	//commandProcessors.push_back(new CalibrationStateCommandProcessor(deviceAddress, calibrationState));
	}

Motor* CommandProcessor::GetMotor(Command& command)
	{
	auto address = command.TargetDevice;
	return address == '1' ? focuser : rotator;
	}

Response CommandProcessor::HandleCommand(Command& command)
	{
	/*
		Commands that address a motor
	*/
	if (command.IsMotorCommand())
		{
		if (command.Verb == "MI") return HandleMI(command);
		if (command.Verb == "MO") return HandleMO(command);
		if (command.Verb == "RW") return HandleRW(command);
		if (command.Verb == "CS") return HandleCS(command);
		if (command.Verb == "CR") return HandleCR(command);
		if (command.Verb == "CE") return HandleCE(command);
		}
	if (command.IsSystemCommand())
		{
		if (command.Verb == "ZW") return HandleZW(command);
		if (command.Verb == "ZD") return HandleZD(command);
		}
	return Response::Error();
	}

Response CommandProcessor::HandleMI(Command& command)
	{
	// Commands are in whole steps, motors operate in microsteps, so we must convert.
	auto motor = GetMotor(command);
	auto microStepsToMove = command.StepPosition * MICROSTEPS_PER_STEP;
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
	auto microStepsToMove = command.StepPosition * MICROSTEPS_PER_STEP;
	auto targetPosition = motor->CurrentPosition() + microStepsToMove;
	if (targetPosition > motor->LimitOfTravel())
		return Response::Error();
	motor->MoveToPosition(targetPosition);
	return Response::FromSuccessfulCommand(command);
	}

Response CommandProcessor::HandleRW(Command & command)
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

Response CommandProcessor::HandleZW(Command & command)
	{
	settings->Save();
	return Response::FromSuccessfulCommand(command);
	}

Response CommandProcessor::HandleZD(Command & command)
	{
	*settings = PersistentSettings();
	settings->Save();
	return Response::FromSuccessfulCommand(command);
	}
