/*
 * GPIOSensor.h
 *
 * Created: 18.04.2013 15:21:45
 *  Author: Florian
 */

#ifndef GPIOSENSOR_H_
#define GPIOSENSOR_H_

#include "../bitmacros.h"
#include "../Configuration/Configuration.h"

/*! \brief
 * The class that is used to check the foot switch sensor
 * */
class GPIOSensor {
  private:
    /*! The pin to be observed */
    int pin;
    VINTP PUER;
    VINTP GFER;
    VINTP PVR;

  public:
    GPIOSensor();
    ~GPIOSensor();
    void init(Configuration::s_GPIOSensorConfig* thisGPIOSensorConfig_);
    bool getValue();
};

#endif /* GPIOSENSOR_H_ */