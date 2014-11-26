#include "ADC.h"
#include <stdio.h>
#include <stdlib.h>

ADC::ADC() {
}
ADC::~ADC() {
	
	cleanUpChannel( ID );

}
 
bool ADC::init() {
		
	//Channel enable
	*thisadc = ADC_BASE + CHANNEL_ENABLE_REGISTER_OFFSET;
	*thisadc = *thisadc | ID;
	
	//10-bit Konvertierung einstellen
	*thisadc = ADC_BASE + MODE_REGISTER_OFFSET;
	*thisadc = *thisadc & ~0b1000; 
	
	//Konvertierung starten
	*thisadc = ADC_BASE + CONTROL_REGISTER_OFFSET;
	*thisadc = *thisadc | 0b01;
	
	return 1;
}

bool ADC::enableInPinSelector( unsigned long channelID, bool enabled ) {
	
	if ( enabled == 1 ) {
	
	//enable pull-up resistor
	*gpio = GPIO_BASE + GPIO_PULL_UP_OFFSET;
	*gpio = *gpio | channelID;
	
	//enable GPIO channel
	*gpio = GPIO_BASE + GPIO_ENABLE_OFFSET;
	*gpio = *gpio | channelID;
	
	return 1;
	
	} else { return 0; }
}

unsigned long ADC::getChannelValue( unsigned long channelID, bool getAverage, unsigned long numberOfConversionsForAverage ) {

	if ( getAverage == true ) {
		
		unsigned long* channelValues [ numberOfConversionsForAverage ];
		
		for ( unsigned long i = 0; i < numberOfConversionsForAverage; i++ ){
			
			channelValues [ i ] = (unsigned long*) GPIO_BASE + GPIO_PIN_VALUE_OFFSET * channelID;
			
			channelValue += *channelValues [ i ];
			
			delay();
			
		}
		
		
	} else {
		
		unsigned long* getValue = (unsigned long*) GPIO_BASE + GPIO_PIN_VALUE_OFFSET * channelID;
		
		channelValue = *getValue;		
	}
 
	return channelValue;
}

void ADC::cleanUpChannel( unsigned char channelID ) {
	
	//Channel2 disable
	int intChannelID = ( int ) channelID;
	*thisadc = ADC_BASE + CHANNEL_DISABLE_REGISTER_OFFSET;
	*thisadc = *thisadc | intChannelID; //respektive 0b10
	
	//ADC reset
	*thisadc = ADC_BASE + CONTROL_REGISTER_OFFSET;
	*thisadc = *thisadc | 0x00;
}

void ADC::delay() {
	
	int i = 10000;
	
	while ( i-- ) {
		
		asm ( "nop" );
	}
	
}
