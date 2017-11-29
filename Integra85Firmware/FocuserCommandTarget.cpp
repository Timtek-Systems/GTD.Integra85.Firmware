
#include "CommandProcessor.h"

	FocuserCommandTarget::FocuserCommandTarget(char deviceAddress, Motor& motor, CalibrationStateMachine& calibrator, Calibration& calibrationState)
		{
		this->deviceAddress = deviceAddress;
		this->motor = &motor;
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

	Response FocuserCommandTarget::HandleCommand(Command& command)
		{
		if (deviceAddress != command.TargetDevice)
			return Response::Error();
		if (command.Verb == "MI") return HandleMI(command);
		return Response::Error();
		}

	Response FocuserCommandTarget::HandleMI(Command& command)
		{
		// Commands are in whole steps, motors operate in microsteps, so we must convert.
		auto microStepsToMove = command.StepPosition * MICROSTEPS_PER_STEP;
		auto targetPosition = motor->CurrentPosition() - microStepsToMove;
		if (targetPosition < 0)
			return Response::Error();
		motor->MoveToPosition(targetPosition);
		return Response::FromSuccessfulCommand(command);
		}
