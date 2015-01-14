#include "GPIOSensor.h"

/*! \fn GPIOSensor::GPIOSensor
 * \brief
 *      Constructor is unused
 */
GPIOSensor::GPIOSensor() {}

/*! \fn GPIOSensor::GPIOSensor
 * \brief
 *      Destructor resets the pull-up resistance and the glitch filter
 */
GPIOSensor::~GPIOSensor() {
    // reset value to zero for PUER register
    *PUER = 0;
    // reset value to zero for GFER register
    *GFER = 0;
}

/*! \fn GPIOSensor::init
 * \param thisGPIOSensorConfig_ initial settings
 * \brief
 *      Enables the GPIO pin and the glitch filter, optionally also the pull-up resistance.
 */
void GPIOSensor::init(Configuration::s_GPIOSensorConfig* thisGPIOSensorConfig_) {
    pin = thisGPIOSensorConfig_->pin;
    PVR = (VINTP)(GPIO_MODULE + thisGPIOSensorConfig_->port * GPIO_PORT_OFFSET + GPIO_PVR_OFFSET);
    PUER = (VINTP)(GPIO_MODULE + thisGPIOSensorConfig_->port * GPIO_PORT_OFFSET + GPIO_PUER_OFFSET);
    GFER = (VINTP)(GPIO_MODULE + thisGPIOSensorConfig_->port * GPIO_PORT_OFFSET + GPIO_GFER_OFFSET);
    // set pullup bit dependent on input
    if (thisGPIOSensorConfig_->pullupEnabled) SET_BIT(*PUER, pin);
    else CLEAR_BIT(*PUER, pin);
    // enable glitch filter
    SET_BIT(*GFER, pin);
}

/*! \fn GPIOSensor::getValue
 * \return the current value of the pin
 */
bool GPIOSensor::getValue() {
    return BIT_IS_SET(*PVR, pin);
}