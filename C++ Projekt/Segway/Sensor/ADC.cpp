#include "ADC.h"

ADC::ADC() {
}
ADC::~ADC() {

}

bool ADC::init() {

	return 0;
}

bool ADC::enableInPinSelector( unsigned long channelID, bool enabled ) {

	return 0;
}

unsigned long ADC::getChannelValue( unsigned long channelID, bool getAverage, unsigned long numberOfConversionsForAverage ) {

	return 0;
}

void ADC::cleanUpChannel( unsigned char channelID ) {

}
