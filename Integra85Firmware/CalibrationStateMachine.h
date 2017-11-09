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

class ICalibrationState;

class CalibrationStateMachine
	{
	public:
		CalibrationStateMachine(Motor *motor, ForceSensitiveResistor *limitSensor);
		void Loop();
		void ChangeState(ICalibrationState& newState);
		void CalibrationComplete();
	private:
		Motor *stepper;
		ForceSensitiveResistor *sensor;
		ICalibrationState *currentState;
		uint32_t calibrationDistanceMovingIn, calibrationDistanceMovingOut;
		uint32_t backlashMeasurement;
		friend class FindHomeCalibrationState;
		friend class BackOutCalibrationState;
		friend class FindSoftLimitCalibrationState;
		friend class FindBacklashCalibrationState;
		friend class FindMidpointCalibrationState;
	};

class ICalibrationState
	{
	public:
		virtual void Loop(CalibrationStateMachine & machine) {};
		virtual void OnExit(CalibrationStateMachine& machine) {};
		virtual void OnEnter(CalibrationStateMachine& machine) {};
		char* StateName = "anon";
		virtual ~ICalibrationState() {};
	};

class IdleCalibrationState : public ICalibrationState
	{
	public:
		static ICalibrationState & GetInstance();
		// Inherited via ICalibrationState
		virtual void Loop(CalibrationStateMachine & machine) final;
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

