#include "GPIOSensor.h"

/*! \fn GPIOSensor::GPIOSensor
 * \brief
 *      Konstruktor wird nicht benutzt
 */
GPIOSensor::GPIOSensor() {}

/*! \fn GPIOSensor::GPIOSensor
 * \brief
 *      Destruktor setzt den Pull-Up Widerstand und den Glitch-Filter zurück
 */
GPIOSensor::~GPIOSensor() {
    // reset value to zero for PUER register
    *PUER = 0;
    // reset value to zero for GFER register
    *GFER = 0;
}

/*! \fn GPIOSensor::init
 * \param thisGPIOSensorConfig_ Initialisierungseinstellungen
 * \brief
 *      Schaltet den GPIO Pin frei und den Glitch Filter an.
 *      Optional auch den Pull-Up Widerstand.
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
 * \return Den aktuellen, binären Wert des Pins.
 */
bool GPIOSensor::getValue() {
    return BIT_IS_SET(*PVR, pin);
}