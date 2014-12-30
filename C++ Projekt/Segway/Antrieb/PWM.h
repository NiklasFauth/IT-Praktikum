/* verwenden Sie diese Klasse f�r alle Funktionen die direkt auf die HW-PWM zugreifen. */

#ifndef PWM_H_
#define PWM_H_

#include "../bitmacros.h"
#include "../Configuration/Configuration.h"

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