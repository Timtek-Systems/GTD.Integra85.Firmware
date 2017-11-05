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
//Software serial
#define BluetoothRxPin				(2)
#define BluetoothTxPin						(3)

#define BitClear(target,position)	target &= ~(bit(position))
#define BitSet(target,position)		target |= bit(position)
#define BitToggle(target,position)	target ^= bit(position)

template <typename T> int sgn(T val) 
	{
	return (T(0) < val) - (val < T(0));
	}