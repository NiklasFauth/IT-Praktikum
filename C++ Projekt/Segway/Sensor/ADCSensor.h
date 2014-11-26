#include "../Configuration/Configuration.h"
#include "ADC.h"

/*
	Requires the ADC to be initialized.
*/


#ifndef ADCSENSOR_H_
#define ADCSENSOR_H_

class ADCSensor {
private:
	Configuration::s_ADCSensorConfig* adcSensor;
	ADC controller;
	Configuration::s_gpioMultiplexData* multiplexData;
	signed long integerValue;
	float floatValue;
	unsigned long channelID;
	int* disable;
	
public:
	ADCSensor();
	~ADCSensor();
	bool init( Configuration::s_ADCSensorConfig* thisADCSensorConfig_, ADC* ADCController_ );
	long getIntegerValue ( bool average = false, unsigned long numberOfValuesForAverage = 0 ); //fertig
	void setZeroOffset( bool active, signed long offset ); //fertig
	bool getZeroOffsetIsActive(); //fertig
	signed long getZeroOffset();
	float getFloatValue ( bool average, unsigned long numberOfValuesForAverage );
	void setSlopeFactor ( bool active, float factor );
	bool getSlopeFactorIsActive ( void );
	float getSlopeFactor ( void );
	void cleanUp ( void );
	void setChannelID ( unsigned long newChannelID );
	unsigned long getChannelID ( void );
	
};



#endif /* ADCSENSOR_H_ */