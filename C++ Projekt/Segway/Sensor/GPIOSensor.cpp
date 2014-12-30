#include "GPIOSensor.h"

GPIOSensor::GPIOSensor() {}

GPIOSensor::~GPIOSensor() {
    cleanUp();
}

void GPIOSensor::init(Configuration::s_GPIOSensorConfig* thisGPIOSensorConfig_) {
    pin = thisGPIOSensorConfig_->pin;
    PVR = (int*)(GPIO_MODULE + (thisGPIOSensorConfig_->port ? PORT_OFFSET : 0) + PVR_OFFSET);
    PUER = (int*)(GPIO_MODULE + (thisGPIOSensorConfig_->port ? PORT_OFFSET : 0) + PUER_OFFSET);
    GFER = (int*)(GPIO_MODULE + (thisGPIOSensorConfig_->port ? PORT_OFFSET : 0) + GFER_OFFSET);
    // set pullup bit dependent on input
    if (thisGPIOSensorConfig_->pullupEnabled) SET_BIT(*PUER, pin);
    else CLEAR_BIT(*PUER, pin);
    // enable glitch filter
    SET_BIT(*GFER, pin);
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