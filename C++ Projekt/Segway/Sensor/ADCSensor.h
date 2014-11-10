#include "../Configuration/Configuration.h"
#include "ADC.h"

/*
	Requires the ADC to be initialized.
*/


#ifndef ADCSENSOR_H_
#define ADCSENSOR_H_

class ADCSensor {
private:

public:
	ADCSensor();
	~ADCSensor();
	bool init( Configuration::s_ADCSensorConfig* thisADCSensorConfig_, ADC* ADCController_ );
	long getIntegerValue ( bool average = false, unsigned long numberOfValuesForAverage = 0 );
	void setZeroOffset( bool active, signed long offset );
	bool getZeroOffsetIsActive();
	signed long getZeroOffset();
};



#endif /* ADCSENSOR_H_ */