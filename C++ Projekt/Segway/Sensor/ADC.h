/*
    use this class for ADC implementation
*/

#ifndef ADC_H_
#define ADC_H_

#include "../bitmacros.h"
#include "../Configuration/Configuration.h"

class ADC {
  private:
    unsigned long channelValue;
    unsigned int prescalerRate;
    unsigned int startupTime;

  public:
    unsigned long ID;
    signed long offsetValue;
    float ADCSlopeFactor;
    bool useADCZeroOffset;
    bool useADCSlopeFactor;
    ADC();
    ~ADC();
    bool init();
    bool enableInPinSelector(unsigned long channelID, bool enabled);
    unsigned long getChannelValue(unsigned long channelID, bool getAverage = false, unsigned long numberOfConversionsForAverage = 0);
    void cleanUpChannel(unsigned char channelID);
};

#endif /* ADC_H_ */