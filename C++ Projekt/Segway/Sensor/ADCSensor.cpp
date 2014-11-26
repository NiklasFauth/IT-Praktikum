#include "ADCSensor.h"

ADCSensor::ADCSensor() {

}

ADCSensor::~ADCSensor() {

	cleanUp();
}

bool ADCSensor::init( Configuration::s_ADCSensorConfig* thisADCSensorConfig_, ADC* ADCController_ ) {
	
	ADCController_->ID = thisADCSensorConfig_->ADCChannelID;
	ADCController_->offsetValue = thisADCSensorConfig_->zeroOffset;
	ADCController_->ADCSlopeFactor = thisADCSensorConfig_->slopeFactor;
	ADCController_->useADCSlopeFactor = thisADCSensorConfig_->useSlopeFactor;
	ADCController_->useADCZeroOffset = thisADCSensorConfig_->useZeroOffset;
	
	return 1;
}

signed long ADCSensor::getIntegerValue( bool average, unsigned long numberOfValuesForAverage ) {
	
	channelID = adcSensor->ADCChannelID;
	
	//Wert des ADC ausgeben; Messen im ADC durch polling -> andauernd messen; 
	
	//controller.init();:ADC enable, 10 bit konvertierung einstellen (Mode Register), konvertierung starten (control Register), siehe ADC.cpp
	//controller ist vom Typ ADC

	if ( average == true ) {
		
		signed long *values [ numberOfValuesForAverage ];
		
		//In jedem Schleifendurchgang wird der ADC initialisiert (d.h. eine Konvertierung gestartet),
		//der letzte Wert wird ausgelesen und in dem array values gespeichert. Danach wird der ADC wird zurückgesetzt.
		//Falls ein Offset verwendet wird, wird dieser vom ausgelesenen wert abgezogen.
		
		for ( unsigned int i = 0; i < numberOfValuesForAverage; i++ ){
			
			controller.init();
			
			values [ i ] = (signed long*) ADC_BASE + LAST_CONVERTED_DATA_REGISTER_OFFSET * channelID;
			
			controller.cleanUpChannel( channelID );
			
			if ( adcSensor->zeroOffset == 0 ) {
			
				integerValue += *values [ i ];
			
			} else {
				
				integerValue += *values [ i ] - adcSensor->zeroOffset;
			}
		}
		
		integerValue = integerValue / numberOfValuesForAverage;
		
		return integerValue;
		
	} else {
		
		controller.init();
		
		//letzten Wert auslesen
		int* getValue = (int*) ADC_BASE + LAST_CONVERTED_DATA_REGISTER_OFFSET * channelID;
		
		if ( adcSensor->zeroOffset == 0 ){
			
			integerValue = *getValue;
			
		} else {
		
		integerValue = *getValue - adcSensor->zeroOffset;
		
		}
		
		return integerValue;
		
	}

}


void ADCSensor::setZeroOffset( bool active, signed long offset ) {
	
	adcSensor->zeroOffset = offset;
	adcSensor->useZeroOffset = active;	

}

bool ADCSensor::getZeroOffsetIsActive() {
	
	return adcSensor->useZeroOffset;
}

signed long ADCSensor::getZeroOffset() {
	
	return adcSensor->zeroOffset;
}

float ADCSensor::getFloatValue( bool average, unsigned long numberOfValuesForAverage ) {
	
	channelID = adcSensor->ADCChannelID;
	
	if ( average == true ) {
		
		float *floatValues [ numberOfValuesForAverage ];
		
/*! \getFloatValue falls average = true

		\brief		In jedem schleifedurchlauf wird der ADC initialisiert beinhaltet den Start einer Konvertierung),
					der letzte konvertierte Wert  ausgelesen und im floatValues array abgespeichert.
					Danach wird der Mittelwert über alle Werte gebildet*/

		for ( unsigned int i = 0; i < numberOfValuesForAverage; i++ ) {
			
			controller.init();
			
			floatValues [ i ] = (float *) ADC_BASE + LAST_CONVERTED_DATA_REGISTER_OFFSET * channelID;
			
			controller.cleanUpChannel( channelID );
			
			floatValue += *floatValues [ i ];		
		}
		
		floatValue = floatValue / numberOfValuesForAverage;
		
	} else {
		
		controller.init();
		
		float* valueGet = (float*) ADC_BASE + LAST_CONVERTED_DATA_REGISTER_OFFSET * channelID;
		
		floatValue = *valueGet;		
	}	
	
	return floatValue;
}

void ADCSensor::setSlopeFactor ( bool active, float factor ) {
	
	adcSensor->useSlopeFactor = active;
	adcSensor->slopeFactor = factor;
	
}

bool ADCSensor::getSlopeFactorIsActive( void ) {
	
	return adcSensor->useSlopeFactor;
	
}

float ADCSensor::getSlopeFactor( void ) {
	
	return adcSensor->slopeFactor;
	
}


void ADCSensor::cleanUp( void ) {
	
	//deaktiviert den Ausgangspin
	*disable = ADC_BASE + CHANNEL_DISABLE_REGISTER_OFFSET * adcSensor->ADCChannelID;;
	*disable = *disable | adcSensor->ADCChannelID;
	
	//löscht die Variablen
	delete adcSensor;
}

void ADCSensor::setChannelID( unsigned long newChannelID ) {
	
	adcSensor->ADCChannelID = newChannelID;
	
}

unsigned long ADCSensor::getChannelID( void ) {
	
	return adcSensor->ADCChannelID;
	
}