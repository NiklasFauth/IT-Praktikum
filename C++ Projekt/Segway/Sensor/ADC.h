/*
	use this class for ADC implementation
*/

#ifndef ADC_H_
#define ADC_H_

#define ADC_NUM_CHANNELS 8

class ADC {
private:

public:
	ADC();
	~ADC();
	bool init();
	bool enableInPinSelector( unsigned long channelID, bool enabled );
	
	unsigned long getChannelValue( unsigned long channelID, bool getAverage = false, unsigned long numberOfConversionsForAverage = 0 );
	
	void cleanUpChannel( unsigned char channelID );
};

#endif /* ADC_H_ */