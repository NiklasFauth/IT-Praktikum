/*
 * GPIOSensor.h
 *
 * Created: 18.04.2013 15:21:45
 *  Author: Florian
 */

#include "../bitmacros.h"

#define VINTP volatile int*


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
    void init(Configuration::s_GPIOSensorConfig* thisGPIOSensorConfig_);
    bool getValue();
    void cleanUp();
};


#endif /* GPIOSENSOR_H_ */

// adresses
#define PORT_OFFSET 0x0100

#define GPIO_MODULE 0xFFFF1000
#define GPER_OFFSET 0x00 // enable
#define PMR0_OFFSET 0x10 // peripheral mux 0
#define PMR1_OFFSET 0x20 // peripheral mux 1
#define ODER_OFFSET 0x40 // output driver
#define OVR_OFFSET 0x50 // output value
#define PVR_OFFSET 0x60 // value
#define PUER_OFFSET 0x70 // pull-up
#define GFER_OFFSET 0xC0 // glitch filter

#define PWM_MODULE 0xFFFF3000
#define ENA_OFFSET 0x004 // enable
#define DIS_OFFSET 0x008 // disable
#define SR_OFFSET 0x00C // status
#define CMR0_OFFSET 0x200 // channel 0
#define CHANNEL_OFFSET 0x020 // channel
#define CDTY0_OFFSET 0x004 // duty cycle
#define CPRD0_OFFSET 0x008 // period
#define CCNT0_OFFSET 0x00C // counter
#define CUPD0_OFFSET 0x010 // update
