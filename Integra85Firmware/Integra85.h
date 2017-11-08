#pragma once
// Analogue input channels
#define TEMPERATURE_SENSOR_CHANNEL  (11)
#define TOUCH_SENSOR_CHANNEL		(0)
//Motor1 pins
#define M1_STEP_PIN					(7)
#define M1_DIRECTION_PIN			(6)
#define M1_ENABLE_PIN				(13)
//Motor2 pins
#define M2_STEP_PIN					(5)
#define M2_DIRECTION_PIN			(4)
#define M2_ENABLE_PIN				(12)
// Motor Parameters
#define MICROSTEPS_PER_STEP			(16) 
//Software serial
#define BluetoothRxPin				(2)
#define BluetoothTxPin				(3)

// FSR limits
#define FSR_HARD_THRESHOLD			(600)	// Used during calibration to find the "hard stop" home position
#define FSR_SOFT_THRESHOLD			(300)	// Used during calibration to define the operational home position (Position = 0 steps).

// Motion limits
#define MOTOR_STEP_LIMIT			(200000)
#define MOTOR_STEP_MIDPOINT			(MOTOR_STEP_LIMIT / 2)

#define BitClear(target,position)	target &= ~(bit(position))
#define BitSet(target,position)		target |= bit(position)
#define BitToggle(target,position)	target ^= bit(position)

template <typename T> int sgn(T val) 
	{
	return (T(0) < val) - (val < T(0));
	}