#include "GPIOSensor.h"

GPIOSensor::GPIOSensor() {}

GPIOSensor::~GPIOSensor() {
    // reset value to zero for PUER register
    *PUER = 0;
    // reset value to zero for GFER register
    *GFER = 0;
}

void GPIOSensor::init(Configuration::s_GPIOSensorConfig* thisGPIOSensorConfig_) {
    pin = thisGPIOSensorConfig_->pin;
    PVR = (VINTP)(GPIO_MODULE + (thisGPIOSensorConfig_->port ? GPIO_PORT_OFFSET : 0) + GPIO_PVR_OFFSET);
    PUER = (VINTP)(GPIO_MODULE + (thisGPIOSensorConfig_->port ? GPIO_PORT_OFFSET : 0) + GPIO_PUER_OFFSET);
    GFER = (VINTP)(GPIO_MODULE + (thisGPIOSensorConfig_->port ? GPIO_PORT_OFFSET : 0) + GPIO_GFER_OFFSET);
    // set pullup bit dependent on input
    if (thisGPIOSensorConfig_->pullupEnabled) SET_BIT(*PUER, pin);
    else CLEAR_BIT(*PUER, pin);
    // enable glitch filter
    SET_BIT(*GFER, pin);
}

bool GPIOSensor::getValue() {
    return BIT_IS_SET(*PVR, pin);
}