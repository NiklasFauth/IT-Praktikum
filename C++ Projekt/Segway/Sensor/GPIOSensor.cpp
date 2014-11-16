#include "GPIOSensor.h"

const int *GPIO_ADRESS = 0xFFFF1000;
const int *PORT_OFFSET = 0x0100;
const int *PVR_ADRESS = 0x60; // value
const int *PUER_ADRESS = 0x70; // pull-up
// const int *GFER_ADRESS = 0xC0; // glitch filter

GPIOSensor::GPIOSensor() {

}

GPIOSensor::~GPIOSensor() {
	cleanUp();
}

void GPIOSensor::init( Configuration::s_GPIOSensorConfig* thisGPIOSensorConfig_ ) {
	pin = *thisGPIOSensorConfig_->pin;
	PVR = (int*) (GPIO_ADRESS + (config.port ? PORT_OFFSET : 0) + PVR_ADRESS);
	PUER = (int*) (GPIO_ADRESS + (config.port ? PORT_OFFSET : 0) + PUER_ADRESS);
	// set pullup bit dependent on input
	if (thisGPIOSensorConfig_->pullupEnabled) SET_BIT(*PUER, pin);
	else CLEAR_BIT(*PUER, pin);
	// enable glitch filter
	//GFER = (int*) (GPIO_ADRESS + (config.port ? PORT_OFFSET : 0) + GFER_ADRESS);
	//if (glitchFilterEnabled) SET_BIT(*GFER, pin);
	//else CLEAR_BIT(*GFER, pin);
}

bool GPIOSensor::getValue() {
	return BIT_IS_SET(*PVR, pin);
}
	
void GPIOSensor::cleanUp() {
	// reset value to zero for PUER register
	*PUER = 0;
	// reset value to zero for GFER register
	*GFER = 0;
}