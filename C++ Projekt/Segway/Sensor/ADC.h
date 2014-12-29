/*
    use this class for ADC implementation
*/

#ifndef ADC_H_
#define ADC_H_

#define ADC_NUM_CHANNELS 8
#define ADC_BASE 0xFFFF3C00
#define MODE_REGISTER_OFFSET 0x04
#define LAST_CONVERTED_DATA_REGISTER_OFFSET 0x20
#define CHANNEL_ENABLE_REGISTER_OFFSET 0x10
#define CHANNEL_DISABLE_REGISTER_OFFSET 0x14
#define CHANNEL_STATUS_REGISTER_OFFSET 0x18
#define CONTROL_REGISTER_OFFSET 0x00
#define STATUS_REGISTER_OFFSET 0x1C

#define GPIO_BASE 0xFFFF1000
#define PMR0_OFFSET 0x10
#define PMR1_OFFSET 0x20
#define GPIO_ENABLE_OFFSET 0x00
#define GPIO_PORT_SIZE 0x0100

#define VINTP volatile int*

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

#define VINTP volatile int*

