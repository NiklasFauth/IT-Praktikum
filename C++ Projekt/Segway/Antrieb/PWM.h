/* verwenden Sie diese Klasse f�r alle Funktionen die direkt auf die HW-PWM zugreifen. */

// #include "../UART/bitmacros.h"

#ifndef PWM_H_
#define PWM_H_

#include "../Configuration/Configuration.h"

class PWM {
private:
    int pin;
    int channelID;
    int maxPWMRatio;
    volatile int* ENA;
    volatile int* DIS;
	volatile int* SR;
	volatile int* CMR0;
	volatile int* GPIO;
    volatile int* CPRD0;
    volatile int* CDTY0;
    volatile int* CUPD0;

public:
	PWM();
	~PWM();
	bool init( Configuration::s_PWMConfig* thisPWMConfig_ );
	
	bool setChannelPWMRatio( unsigned char ratioOn, bool capRatioOn = false );
	char getChannelPWMRatio();
    bool isChannelEnabled();
	bool setChannelEnabled( bool enabled );
	
	void cleanUp();	
};

#endif /* PWM_H_ */