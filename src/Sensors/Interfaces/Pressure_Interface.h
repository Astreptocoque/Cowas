#ifndef Pressure_Interface_h
#define Pressure_Interface_h

#include <AnalogIn.h>
#include <Thread.h>
#include <interfaces/DistanceInterface.h>

class Pressure_Interface
{
public:
	// This is the minimum range of the sensor
//	byte minPressure;
	// This is the maximum range of the sensor
//	byte maxPressure;

	// Should return the CACHED value of the sensor
	virtual byte getPressure();

	// Should READ and CACHE the value of the sensor
	virtual void readPressure();
};

#endif
