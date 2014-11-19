/* verwenden Sie diese Klasse f�r alle Funktionen die direkt auf die HW-PWM zugreifen. */

#include "../UART/adresses.h"
#include "../UART/bitmacros.h"

#ifndef PWM_H_
#define PWM_H_

#include "../Configuration/Configuration.h"

class PWM {
private:
    int channelID;
    int* ENA;
    int* DIS;
    int* CPRD0;
    int* CDTY0;
    int* CUPD0;
    static int CLK_FREQ = 12000000;

public:
	PWM();
	~PWM();
	bool init( Configuration::s_PWMConfig* thisPWMConfig_ );
	bool enableInPinSelector( bool enabled );
	
	bool setChannelPWMRatio( unsigned char ratioOn, bool capRatioOn = false );
	bool setChannelEnabled( bool enabled );
	
	void cleanUp();	
};

#endif /* PWM_H_ */