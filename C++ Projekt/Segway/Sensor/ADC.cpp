#include "ADC.h"
#include <stdio.h>
#include <stdlib.h>
#include "../Configuration/Configuration.h"
#include "../bitmacros.h"


ADC::ADC() {
}
ADC::~ADC() {
	
	cleanUpChannel( ID );

}
 /************************************************************************/
 /* init resettet den ADC zunächst und setzt anschließend die ADC clock 
	und die Startup time. Danach wird die 10 bit Konvertierung eingestellt
	und der sleep Modus, sowie der Hardware Trigger werden deaktiviert.
	Am Ende wird noch eine einzelne Konvertierung gestartet.                                                                     */
 /************************************************************************/
bool ADC::init() {
	
	//ADC reset
	SET_BIT ( *( volatile unsigned int* ) ( ADC_BASE + CONTROL_REGISTER_OFFSET ), 0 );
	
	//ADC Clock einstellen
	prescalerRate = Configuration::ADCCLK / ( Configuration::ADC_Internal_Clock * 2 );
	//prescalerRate darf nicht zu groß werden
	if ( prescalerRate > 255 ){
		
		return false;
	}
	
	//Prescale Inhalt löschen
	CLEAR_BITS ( *( volatile unsigned int* ) ( ADC_BASE + MODE_REGISTER_OFFSET ), 0xFF << 8 );
	//prescalerRate in das Mode Register reinschreiben
	SET_BITS ( *( volatile unsigned int* ) ( ADC_BASE + MODE_REGISTER_OFFSET ), ( prescalerRate & 0xFF ) << 8 );
	
	
	//Startup time einstellen
	startupTime = Configuration::ADCCLK / ( ( prescalerRate + 1 ) * 800000 );
	
	if ( startupTime > 127 ){
		
		return false;
	}
	
	//startup time Inhalt löschen
	CLEAR_BITS ( *( volatile unsigned int* ) ( ADC_BASE + MODE_REGISTER_OFFSET ), 0x7F << 16 );
	//neuen Inhalt reinschreiben
	SET_BITS ( *( volatile unsigned int* ) ( ADC_BASE + MODE_REGISTER_OFFSET ), ( startupTime & 0x7F ) << 16 );
	
	//Hardware Trigger deaktivieren, 
	CLEAR_BIT ( *( volatile unsigned int* ) ( ADC_BASE + MODE_REGISTER_OFFSET ), 0 );
	
	//10-bit Konvertierung einstellen
	CLEAR_BIT ( *( volatile unsigned int* ) ( ADC_BASE + MODE_REGISTER_OFFSET ), 4 );
	
	//normalen Modus wählen, kein Sleep Mode
	CLEAR_BIT ( *( volatile unsigned int* ) ( ADC_BASE + MODE_REGISTER_OFFSET ), 5 );
	
	//eine Konvertierung starten
	SET_BIT ( *( volatile unsigned int* ) ( ADC_BASE + CONTROL_REGISTER_OFFSET ), 1 );
	
	return 1;
}
/************************************************************************/
/* Falls enabled = true, wird hier festgelegt, dass eine Peripheral
	Function den Pin kontrolliert. Außerdem werden der Pull-Up Widerstand
	und der Output Driver deaktiviert. Darüber hinaus werden die Peripheral
	Mux Register überprüft und gegebenenfalls gesetzt, oder gelöscht.
	Falls enabled = false, kontrolliert der Gpio den Pin  und der output 
	Driver wird deaktiviert               */
/************************************************************************/
bool ADC::enableInPinSelector( unsigned long channelID, bool enabled ) {
	
	if ( channelID >= ADC_NUM_CHANNELS ){
		
		return 0;
		
	}
	
	if ( enabled == 1 ) {
		
		//Peripheral Function einstellen
		CLEAR_BIT ( *( volatile unsigned int* ) ( GPIO_BASE + GPIO_ENABLE_OFFSET + Configuration::ADC_gpioMultiplexData[channelID].port * GPIO_PORT_SIZE ), Configuration::ADC_gpioMultiplexData[channelID].pin );
		
		//disable pull-up resistor
		CLEAR_BIT ( *( volatile unsigned int* ) ( GPIO_BASE + GPIO_PULL_UP_OFFSET + Configuration::ADC_gpioMultiplexData[channelID].port * GPIO_PORT_SIZE ), Configuration::ADC_gpioMultiplexData[channelID].pin );
		
		//Output driver deaktivieren
		CLEAR_BIT ( *( volatile unsigned int* ) ( GPIO_BASE + OUTPUT_DRIVER_ENABLE_OFFSET + Configuration::ADC_gpioMultiplexData[channelID].port * GPIO_PORT_SIZE ), Configuration::ADC_gpioMultiplexData[channelID].pin );
		
		
	
		//PMR1 prüfen, wenn 1, schreibe eine 1 rein, wenn nicht, schreibe eine 0 rein
		if ( BIT_IS_SET ( Configuration::ADC_gpioMultiplexData[channelID].multiplexRegisterValue, 1 ) ){
			
			SET_BIT ( *( volatile unsigned int* ) (GPIO_BASE + PMR1_OFFSET + Configuration::ADC_gpioMultiplexData[channelID].port * 0x0100 ) , Configuration::ADC_gpioMultiplexData[channelID].pin );
			
		} else {
			
			
			CLEAR_BIT ( *( volatile unsigned int* ) ( GPIO_BASE + PMR1_OFFSET + Configuration::ADC_gpioMultiplexData[channelID].port * 0x0100 ), Configuration::ADC_gpioMultiplexData[channelID].pin );
		}
		
		//PMR0 prüfen, analog zu PMR1
		if ( BIT_IS_SET ( Configuration::ADC_gpioMultiplexData[channelID].multiplexRegisterValue, 0 ) ){
			
			SET_BIT ( *( volatile unsigned int* ) ( GPIO_BASE + PMR0_OFFSET + Configuration::ADC_gpioMultiplexData[channelID].port * 0x0100 ), Configuration::ADC_gpioMultiplexData[channelID].pin );
			
		} else {
			
			CLEAR_BIT ( *(volatile unsigned int* ) ( GPIO_BASE +PMR0_OFFSET + Configuration::ADC_gpioMultiplexData[channelID].port * 0x0100 ), Configuration::ADC_gpioMultiplexData[channelID].pin );
			
		}
	
		return 1;
	
	} else { 
		
		//GPIO controll einstellen
		SET_BIT ( *( volatile unsigned int* ) ( GPIO_BASE + GPIO_ENABLE_OFFSET + Configuration::ADC_gpioMultiplexData[channelID].port * GPIO_PORT_SIZE ), Configuration::ADC_gpioMultiplexData[channelID].pin );
		//Deaktivieren des Output Drivers
		CLEAR_BIT ( *( volatile unsigned int* ) ( GPIO_BASE + OUTPUT_DRIVER_ENABLE_OFFSET + Configuration::ADC_gpioMultiplexData[channelID].port * GPIO_PORT_SIZE ), Configuration::ADC_gpioMultiplexData[channelID].pin );
		
		return 0; 
		
		}
}
/************************************************************************/
/*	Hier wird der aktuelle Wert des ADC im Last Converted Data Register 
	ausgelesen.
	Falls getAverage true ist, wird eine Schleife aufgerufen, die in jedem
	Durchlauf zunächst eine Konvertierung startet, anschließend wartet,
	bis die Konvertierung abgechlossen ist und dann die Konvertierten Werte
	aufsummiert. Am ende wird noch der Mittelwert gebildet.                                                                      */
/************************************************************************/
unsigned long ADC::getChannelValue( unsigned long channelID, bool getAverage, unsigned long numberOfConversionsForAverage ) {

	if ( channelID >= ADC_NUM_CHANNELS ) {
		
		return 0;
	}
	
	channelValue = 0;
	
	//Aktivieren des mit channelID übergebenen ADC Kanals
	SET_BIT ( *( volatile unsigned int* ) ( ADC_BASE + CHANNEL_ENABLE_REGISTER_OFFSET ), channelID );

	if ( getAverage == true ) {
		
		//In jedem Schleifendurchlauf wird eine Konvertierung gestartet. Daraufhin wird gewartet, bis die Konvertierung abgeschlossen ist
		//und im DRD bit des Status Registers eine 1 steht. Dies bedeutet, dass eine Konvertierung abgeschlossen wurde und im Last converted Data Register 
		//Daten ausgelesen werden kann. Die ausgelesenen Werte werden in der Variablen channelValue aufsummiert.
		
		for ( unsigned long i = 0; i < numberOfConversionsForAverage; i++ ){
			
			//Hier wird die Konvertierung gestartet
			SET_BIT ( *( volatile unsigned int* ) ( ADC_BASE + CONTROL_REGISTER_OFFSET ), 1 );
			
			//Hier wird gewartet, bis im Data ready bit eine 1 steht
			while ( !BIT_IS_SET ( *(volatile unsigned int* ) ( ADC_BASE + STATUS_REGISTER_OFFSET ), 16 ) );
			
			channelValue += *( volatile unsigned int* ) ( ADC_BASE + LAST_CONVERTED_DATA_REGISTER_OFFSET * channelID );
			
		}
		
		channelValue = channelValue / numberOfConversionsForAverage;
		
		
	} else {
		
		SET_BIT ( *( volatile unsigned int* ) ( ADC_BASE + CONTROL_REGISTER_OFFSET ), 1 );
		
		while ( !BIT_IS_SET ( *( volatile unsigned int* ) ( ADC_BASE + STATUS_REGISTER_OFFSET ), 16 ) );
		
		channelValue = *( volatile unsigned int* ) ( ADC_BASE + LAST_CONVERTED_DATA_REGISTER_OFFSET * channelID );		
	}
	
	//Deaktivieren des verwendeten Kanals und Reset des ADC
	SET_BIT ( *( volatile unsigned int* ) ( ADC_BASE + CHANNEL_DISABLE_REGISTER_OFFSET ), channelID );
	SET_BIT ( *( volatile unsigned int* ) ( ADC_BASE + CONTROL_REGISTER_OFFSET ), 0 );
 
	return channelValue;
}

void ADC::cleanUpChannel( unsigned char channelID ) {
	
	///Channel disable
	SET_BIT ( *( volatile unsigned int* ) ( ADC_BASE + CHANNEL_DISABLE_REGISTER_OFFSET ), channelID );
	
	///Output driver deaktivieren
	CLEAR_BIT ( *( volatile unsigned int* ) ( GPIO_BASE + OUTPUT_DRIVER_ENABLE_OFFSET + Configuration::ADC_gpioMultiplexData[channelID].port * GPIO_PORT_SIZE ), Configuration::ADC_gpioMultiplexData[channelID].pin );
	
	///disable pull-up resistor
	CLEAR_BIT ( *( volatile unsigned int* ) ( GPIO_BASE + GPIO_PULL_UP_OFFSET + Configuration::ADC_gpioMultiplexData[channelID].port * GPIO_PORT_SIZE ), Configuration::ADC_gpioMultiplexData[channelID].pin );
	
	///ADC reset
	SET_BIT ( *( volatile unsigned int* ) ( ADC_BASE + CONTROL_REGISTER_OFFSET ), 0 );
}

