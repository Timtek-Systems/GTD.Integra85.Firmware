
#include "CommandProcessor.h"

	CommandProcessor::CommandProcessor(Motor& focuser, Motor& rotator, CalibrationStateMachine& calibrator, Calibration& calibrationState)
		{
		this->focuser = &focuser;
		this->rotator = &rotator;
		this->calibrator = &calibrator;
		this->calibrationState = &calibrationState;
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
		if (command.Verb == "MI") return HandleMI(command);
		if (command.Verb == "MO") return HandleMO(command);
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
