// CalibrationStateMachine.h

#ifndef _CALIBRATIONSTATEMACHINE_h
#define _CALIBRATIONSTATEMACHINE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Integra85.h"
#include "Motor.h"
#include "ForceSensitiveResistor.h"

enum CalibrationResult
	{
	Uncalibrated = 0,
	Calibrated = 1,
	InProgress = 2,
	Cancelled = 3
	};

struct Calibration
	{
	CalibrationResult status = Uncalibrated;
	uint32_t backlash = 0;							// Backlash amoutn in microsteps
	uint16_t lowThreshold = FSR_SOFT_THRESHOLD;
	uint16_t highThreshold = FSR_HARD_THRESHOLD;
	uint16_t slowSpeed = CALIBRATE_SLOW_MOTION;
	bool lastDirectionIn = false;
	};

class ICalibrationState;

class CalibrationStateMachine
	{
	public:
		CalibrationStateMachine(Motor *motor, ForceSensitiveResistor *limitSensor, Calibration& status);
		void Loop();
		void StartCalibration();
		void StopCalibration();
		void SetCalibrated();
		void SetUncalibrated();
		bool InProgress();
	private:
		Motor *stepper;
		ForceSensitiveResistor *sensor;
		Calibration *status;
		ICalibrationState *currentState;
		uint32_t calibrationDistanceMovingIn, calibrationDistanceMovingOut;
		unsigned long startTime;
		void ChangeState(ICalibrationState *newState);
		void CommitCalibration();
		void CalibrationComplete();
		void StopCalibrationIfTimedOut();
		friend class IdleCalibrationState;
		friend class FindHomeCalibrationState;
		friend class DelayAfterFindHomeCalibrationState;
		friend class BackOutCalibrationState;
		friend class DelayAfterBackOutCalibrationState;
		friend class FindSoftLimitCalibrationState;
		friend class DelayAfterFindSoftLimitCalibrationState;
		friend class FindBacklashCalibrationState;
		friend class FindMidpointCalibrationState;
	};

class ICalibrationState
	{
	public:
		virtual void Loop(CalibrationStateMachine & machine) {};
		virtual void OnExit(CalibrationStateMachine& machine) {};
		virtual void OnEnter(CalibrationStateMachine& machine) {};
		virtual ~ICalibrationState() {};
		// State maching input events
		virtual void StartCalibration(CalibrationStateMachine& machine) {};
	};

class IdleCalibrationState : public ICalibrationState
	{
	public:
		// Inherited via ICalibrationState
		virtual void Loop(CalibrationStateMachine & machine) final;
		virtual void StartCalibration(CalibrationStateMachine & machine) final;
	};

class FindHomeCalibrationState : public ICalibrationState
	{
	public:
		// Inherited via ICalibrationState
		virtual void Loop(CalibrationStateMachine & machine) final;
		virtual void OnEnter(CalibrationStateMachine& machine) final;
	};

class BackOutCalibrationState : public ICalibrationState
	{
	public:
		// Inherited via ICalibrationState
		virtual void Loop(CalibrationStateMachine & machine) final;
		virtual void OnEnter(CalibrationStateMachine& machine) final;
	};

class DelayBaseCalibrationState : public ICalibrationState
	{
	public:
		DelayBaseCalibrationState();
		virtual void Loop(CalibrationStateMachine & machine) override;
		virtual void OnEnter(CalibrationStateMachine& machine) override;
	protected:
		virtual void TransitionToNextState(CalibrationStateMachine& machine) = 0;
		unsigned long startTime, delayDuration;
	};

class DelayAfterFindHomeCalibrationState : public DelayBaseCalibrationState
	{
	public:
		DelayAfterFindHomeCalibrationState();
	protected:
		virtual void TransitionToNextState(CalibrationStateMachine& machine) final;
	};

class DelayAfterBackOutCalibrationState : public DelayBaseCalibrationState
	{
	public:
		DelayAfterBackOutCalibrationState();
	protected:
		virtual void TransitionToNextState(CalibrationStateMachine& machine) final;
	};

class DelayAfterFindSoftLimitCalibrationState : public DelayBaseCalibrationState
	{
	public:
		DelayAfterFindSoftLimitCalibrationState();
	protected:
		virtual void TransitionToNextState(CalibrationStateMachine& machine) final;
	};


class FindSoftLimitCalibrationState : public ICalibrationState
	{
	public:
		// Inherited via ICalibrationState
		virtual void Loop(CalibrationStateMachine & machine) final;
		virtual void OnEnter(CalibrationStateMachine& machine) final;
	private:
		uint32_t softLimitPosition;
	};

class FindBacklashCalibrationState : public ICalibrationState
	{
	public:
		// Inherited via ICalibrationState
		virtual void Loop(CalibrationStateMachine & machine) final;
		virtual void OnEnter(CalibrationStateMachine& machine) final;
	private:
		uint32_t softLimitPosition;
	};

class FindMidpointCalibrationState : public ICalibrationState
	{
	public:
		// Inherited via ICalibrationState
		virtual void Loop(CalibrationStateMachine & machine) final;
		virtual void OnEnter(CalibrationStateMachine& machine) final;
	};



#endif

