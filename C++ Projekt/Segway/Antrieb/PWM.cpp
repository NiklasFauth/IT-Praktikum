#include "PWM.h"

PWM::PWM() {

}


PWM::~PWM() {
	cleanup();
}

bool PWM::init( Configuration::s_PWMConfig* thisPWMConfig_ ) {
    pin = thisPWMConfig_->GPIO_pin;
    channelID = thisPWMConfig_->channelID;
	ENA = (int*) (PWM_OFFSET + ENA_OFFSET);
	DIS = (int*) (PWM_OFFSET + DIS_OFFSET);
    CMR0 = (int*) (PWM_OFFSET + CMR0_OFFSET);
    CUPD0 = (int*) (PWM_OFFSET + CMR0_OFFSET + CHANNEL_OFFSET * channelID + CUPD0_OFFSET);
    CPRD0 = (int*) (PWN_OFFSET + CMR0_OFFSET + CHANNEL_OFFSET * channelID + CPRD0_OFFSET);
    *CPRD0 = (int) CLK_FREQ / thisPWMConfig_->frequency;
    CDTY0 = (int*) (PWN_OFFSET + CMR0_OFFSET + CHANNEL_OFFSET * channelID + CDTY0_OFFSET);
    // set maxPWMRatio
    setChannelPWMRatio(thisPWMConfig_->maxPWMRatio, false);
}

bool PWM::setChannelPWMRatio( unsigned char ratioOn, bool capRatioOn ) {
    // set new duty cycle value
    *CUPD0 = (int) ((1 - (float) ratioOn / 255 /* * 0.6 */) * *CPRD0);
    // reset mode register pin 10 to initiate duty cycle update
    CLEAR_BIT(*CMR0, 10);
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
    *CPRD0 = 0;
    *CDTY0 = 0;
}