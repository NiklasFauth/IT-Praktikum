#include "ADCSensor.h"

ADCSensor::ADCSensor() {

}

ADCSensor::~ADCSensor() {

}

bool ADCSensor::init( Configuration::s_ADCSensorConfig* thisADCSensorConfig_, ADC* ADCController_ ) {

	return 0;
}

signed long ADCSensor::getIntegerValue( bool average, unsigned long numberOfValuesForAverage ) {

	return 0;
}


void ADCSensor::setZeroOffset( bool active, signed long offset ) {

}

bool ADCSensor::getZeroOffsetIsActive() {
	
	return 0;
}

signed long ADCSensor::getZeroOffset() {
	
	return 0;
}