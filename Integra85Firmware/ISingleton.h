// ISingleton.h

#ifndef _ISINGLETON_h
#define _ISINGLETON_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

template<typename T>
class ISingleton
	{
	public:
		static T& GetInstance()
			{
			static T instance;
			return instance;
			}
	};

#endif

