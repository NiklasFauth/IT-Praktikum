/*
 * GPIOSensor.h
 *
 * Created: 18.04.2013 15:21:45
 *  Author: Florian
 */ 

// #include "../UART/bitmacros.h"

#ifndef GPIOSENSOR_H_
#define GPIOSENSOR_H_

#include "../Configuration/Configuration.h"

class GPIOSensor {
private:
	int pin;
	VINTP PUER;
	VINTP GFER;
	VINTP PVR;
public:
	GPIOSensor();
	~GPIOSensor();
	void init( Configuration::s_GPIOSensorConfig* thisGPIOSensorConfig_ );
	bool getValue();
	void cleanUp();
};


#endif /* GPIOSENSOR_H_ */