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
	uint16_t focuserBacklash = 0;
	};

class ICalibrationState;

class CalibrationStateMachine
	{
	public:
		CalibrationStateMachine(Motor *motor, ForceSensitiveResistor *limitSensor, Calibration& status);
		void Loop();
		void StartCalibration();
		void StopCalibration();
	private:
		Motor *stepper;
		ForceSensitiveResistor *sensor;
		Calibration *status;
		ICalibrationState *currentState;
		uint32_t calibrationDistanceMovingIn, calibrationDistanceMovingOut;
		unsigned long startTime;
		void ChangeState(ICalibrationState& newState);
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
		static ICalibrationState & GetInstance();
		// Inherited via ICalibrationState
		virtual void Loop(CalibrationStateMachine & machine) final;
		virtual void StartCalibration(CalibrationStateMachine & machine) final;
	private:
		IdleCalibrationState();
	};

class FindHomeCalibrationState : public ICalibrationState
	{
	public:
		static ICalibrationState & GetInstance();
		// Inherited via ICalibrationState
		virtual void Loop(CalibrationStateMachine & machine) final;
		virtual void OnEnter(CalibrationStateMachine& machine) final;
	private:
		FindHomeCalibrationState();
	};

class BackOutCalibrationState : public ICalibrationState
	{
	public:
		static ICalibrationState & GetInstance();
		// Inherited via ICalibrationState
		virtual void Loop(CalibrationStateMachine & machine) final;
		virtual void OnEnter(CalibrationStateMachine& machine) final;
	private:
		BackOutCalibrationState();
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
		static ICalibrationState & GetInstance();
	protected:
		virtual void TransitionToNextState(CalibrationStateMachine& machine) final;
	private:
		DelayAfterFindHomeCalibrationState();
	};

class DelayAfterBackOutCalibrationState : public DelayBaseCalibrationState
	{
	public:
		static ICalibrationState & GetInstance();
	protected:
		virtual void TransitionToNextState(CalibrationStateMachine& machine) final;
	private:
		DelayAfterBackOutCalibrationState();
	};

class DelayAfterFindSoftLimitCalibrationState : public DelayBaseCalibrationState
	{
	public:
		static ICalibrationState & GetInstance();
	protected:
		virtual void TransitionToNextState(CalibrationStateMachine& machine) final;
	private:
		DelayAfterFindSoftLimitCalibrationState();
	};


class FindSoftLimitCalibrationState : public ICalibrationState
	{
	public:
		static ICalibrationState & GetInstance();
		// Inherited via ICalibrationState
		virtual void Loop(CalibrationStateMachine & machine) final;
		virtual void OnEnter(CalibrationStateMachine& machine) final;
	private:
		uint32_t softLimitPosition;
		FindSoftLimitCalibrationState();
	};

class FindBacklashCalibrationState : public ICalibrationState
	{
	public:
		static ICalibrationState & GetInstance();
		// Inherited via ICalibrationState
		virtual void Loop(CalibrationStateMachine & machine) final;
		virtual void OnEnter(CalibrationStateMachine& machine) final;
	private:
		uint32_t softLimitPosition;
		FindBacklashCalibrationState();
	};

class FindMidpointCalibrationState : public ICalibrationState
	{
	public:
		static ICalibrationState & GetInstance();
		// Inherited via ICalibrationState
		virtual void Loop(CalibrationStateMachine & machine) final;
		virtual void OnEnter(CalibrationStateMachine& machine) final;
	private:
		FindMidpointCalibrationState();
	};



#endif

