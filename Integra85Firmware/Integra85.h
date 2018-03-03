#pragma once
// Versioning
#define FIRMWARE_MAJOR_VERSION		(2)
#define FIRMWARE_MINOR_VERSION		(0)

// Analogue input channels
#define TOUCH_SENSOR_CHANNEL		(0)


// Motor Parameters
#define MICROSTEPS_PER_STEP			(16) 
#define MIN_SPEED					(250)	// Minimum speed that can be timed by the hardware timer
#define MIN_RAMP_TIME				(100)	// Minimum ramp up/down time in milliseconds

// Motor1/Focuser
#define M1_STEP_PIN					(7)
#define M1_DIRECTION_PIN			(6)
#define M1_ENABLE_PIN				(13)
#define M1_MAX_POSITION				(188600 * MICROSTEPS_PER_STEP)
#define M1_MIDPOINT					(M1_MAX_POSITION / 2)
#define M1_RAMP_TIME				(500)	// milliseconds
#define M1_MAX_SPEED				(1500 * MICROSTEPS_PER_STEP)

// Motor2/Rotator
#define M2_STEP_PIN					(5)
#define M2_DIRECTION_PIN			(4)
#define M2_ENABLE_PIN				(12)
#define M2_MAX_POSITION				(61802 * MICROSTEPS_PER_STEP)
#define M2_MIDPOINT					(M2_MAX_POSITION / 2)
#define M2_RAMP_TIME				(1000)	// milliseconds
#define M2_MAX_SPEED				(1500 * MICROSTEPS_PER_STEP)

// Temperature Probe
#define TEMPERATURE_SENSOR_PIN		(11)

//Software serial
#define BluetoothRxPin				(2)
#define BluetoothTxPin				(3)
#define RX_BUFFER_SIZE				(16)


// FSR limits
#define FSR_HARD_THRESHOLD			(800)	// Used during calibration to find the "hard stop" home position
#define FSR_SOFT_THRESHOLD			(300)	// Used during calibration to define the operational home position (Position = 0 steps).

// Calibration
#define MAX_CALIBRATION_TIME		(420000)	// Milliseconds, obviously.
#define CALIBRATE_SAFE_DISTANCE		(100000)	// Distance to back off from hard stop
#define CALIBRATE_SLOW_MOTION		(2880)		// High precision calibration slow motion speed, steps per second.

// Error codes

#define ERROR_NO_COMMAND_PROCESSOR	(1); 

#define BitClear(target,position)	target &= ~(bit(position))
#define BitSet(target,position)		target |= bit(position)
#define BitToggle(target,position)	target ^= bit(position)

template <typename T> int sgn(T val) 
	{
	return (T(0) < val) - (val < T(0));
	}