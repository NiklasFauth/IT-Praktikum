/* verwenden Sie diese Klasse f�r alle Funktionen die direkt auf die HW-PWM zugreifen. */

#include "../bitmacros.h"

#ifndef PWM_H_
#define PWM_H_

#include "../Configuration/Configuration.h"
#define VINTP volatile int*

class PWM {
  private:
    int pin;
    int channelID;
    int maxPWMRatio;
    VINTP ENA;
    VINTP DIS;
    VINTP SR;
    VINTP CMR0;
    VINTP GPIO;
    VINTP CPRD0;
    VINTP CDTY0;
    VINTP CUPD0;

  public:
    PWM();
    ~PWM();
    bool init(Configuration::s_PWMConfig* thisPWMConfig_);

    bool setChannelPWMRatio(unsigned char ratioOn, bool capRatioOn = false);
    unsigned char getChannelPWMRatio();
    bool isChannelEnabled();
    bool setChannelEnabled(bool enabled);

    void cleanUp();
};

#endif /* PWM_H_ */

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
