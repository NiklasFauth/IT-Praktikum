#include "PWM.h"

PWM::PWM() {

}


PWM::~PWM() {
	
}

bool PWM::init( Configuration::s_PWMConfig* thisPWMConfig_ ) {
    pin = thisPWMConfig_->GPIO_pin;
    channelID = thisPWMConfig_->channelID;
	ENA = (int*) (PWM_OFFSET + ENA_OFFSET);
	DIS = (int*) (PWM_OFFSET + DIS_OFFSET);
}



bool PWM::setChannelPWMRatio( unsigned char ratioOn, bool capRatioOn ) {
    
	return 0;
}

bool PWM::setChannelEnabled( bool enabled ) {
    if (enabled) SET_BIT(*ENA, channelID);
    else SET_BIT(*DIS, channelID);
}
bool PWM::enableInPinSelector( bool enabled ) {

	return 0;
}

void PWM::cleanUp() {
    *DIS = 1;
}