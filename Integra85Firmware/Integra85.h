#pragma once
// Analogue input channels
#define TEMPERATURE_SENSOR_CHANNEL  (11)
#define TOUCH_SENSOR_CHANNEL		(0)


// Motor Parameters
#define MICROSTEPS_PER_STEP			(16) 
#define MIN_SPEED					(250) // Minimum speed that can be timed by the hardware timer

// Motor1/Focuser
#define M1_STEP_PIN					(7)
#define M1_DIRECTION_PIN			(6)
#define M1_ENABLE_PIN				(13)
#define M1_MAX_POSITION				(200000 * MICROSTEPS_PER_STEP)
#define M1_MIDPOINT					(M1_MAX_POSITION / 2)
#define M1_RAMP_TIME				(250)	// milliseconds
#define M1_MAX_SPEED				(16000)

// Motor2/Rotator
#define M2_STEP_PIN					(5)
#define M2_DIRECTION_PIN			(4)
#define M2_ENABLE_PIN				(12)
#define M2_MAX_POSITION				(61802 * MICROSTEPS_PER_STEP)
#define M2_MIDPOINT					(M2_MAX_POSITION / 2)
#define M2_RAMP_TIME				(500)	// milliseconds
#define M2_MAX_SPEED				(16000)

//Software serial
#define BluetoothRxPin				(2)
#define BluetoothTxPin				(3)

// FSR limits
#define FSR_HARD_THRESHOLD			(600)	// Used during calibration to find the "hard stop" home position
#define FSR_SOFT_THRESHOLD			(300)	// Used during calibration to define the operational home position (Position = 0 steps).


// Error codes

#define ERROR_NO_COMMAND_PROCESSOR	(1); 

#define BitClear(target,position)	target &= ~(bit(position))
#define BitSet(target,position)		target |= bit(position)
#define BitToggle(target,position)	target ^= bit(position)

template <typename T> int sgn(T val) 
	{
	return (T(0) < val) - (val < T(0));
	}