#include "PWM.h"

PWM::PWM() {

}


PWM::~PWM() {
	cleanUp();
}

bool PWM::init( Configuration::s_PWMConfig* thisPWMConfig_ ) {
    pin = thisPWMConfig_->GPIO_pin;
	int *GPER = (int*) (GPIO_MODULE + (thisPWMConfig_->GPIO_port ? PORT_OFFSET : 0) + GPER_OFFSET);
    // init register
    int* PMR0 = (int*) (GPIO_MODULE + (thisPWMConfig_->GPIO_port ? PORT_OFFSET : 0) + PMR0_OFFSET);
    int* PMR1 = (int*) (GPIO_MODULE + (thisPWMConfig_->GPIO_port ? PORT_OFFSET : 0) + PMR1_OFFSET);
    // disable GPIO
    CLEAR_BIT(*GPER, pin);
    // set peripheral function
    if (BIT_IS_SET(thisPWMConfig_->GPIO_multiplexRegisterValue, 0)) SET_BIT(*PMR0, pin);
    else CLEAR_BIT(*PMR0, pin);
    if (BIT_IS_SET(thisPWMConfig_->GPIO_multiplexRegisterValue, 1)) SET_BIT(*PMR1, pin);
    else CLEAR_BIT(*PMR1, pin);
    channelID = thisPWMConfig_->channelID;
    // init register
	ENA = (int*) (PWM_MODULE + ENA_OFFSET);
	DIS = (int*) (PWM_MODULE + DIS_OFFSET);
	SR = (int*) (PWM_MODULE +SR_OFFSET);
    CMR0 = (int*) (PWM_MODULE + CMR0_OFFSET);
    CUPD0 = (int*) (PWM_MODULE + CMR0_OFFSET + CHANNEL_OFFSET * channelID + CUPD0_OFFSET);
    CPRD0 = (int*) (PWM_MODULE + CMR0_OFFSET + CHANNEL_OFFSET * channelID + CPRD0_OFFSET);
    *CPRD0 = (int) Configuration::CPUCLK / thisPWMConfig_->frequency;
    CDTY0 = (int*) (PWM_MODULE + CMR0_OFFSET + CHANNEL_OFFSET * channelID + CDTY0_OFFSET);
    // set maxPWMRatio
    maxPWMRatio = thisPWMConfig_->maxPWMRatio;
}

bool PWM::setChannelPWMRatio( unsigned char ratioOn, bool capRatioOn ) {
    if (0 < ratioOn || ratioOn > maxPWMRatio) return false;
    // set new duty cycle value
    *CUPD0 = (int) (((1 - (float) ratioOn / 255) * maxPWMRatio / 255) * *CPRD0);
    // reset mode register pin 10 to initiate duty cycle update
    CLEAR_BIT(*CMR0, 10);
    return true;
}

char PWM::getChannelPWMRatio() {
    // return current ratio
    return (char) ((1 - (float) *CDTY0 / *CPRD0 * 255 / maxPWMRatio) * 255);
}

bool PWM::isChannelEnabled() {
    // check if bit is set
    return BIT_IS_SET(*SR, channelID);
}
bool PWM::setChannelEnabled( bool enabled ) {
    // set ena/dis bit
    if (enabled) SET_BIT(*ENA, channelID);
    else SET_BIT(*DIS, channelID);
    return true;
}
void PWM::cleanUp() {
    *DIS = 1;
    *CPRD0 = 0;
    *CDTY0 = 0;
    SET_BIT(*GPIO, pin);
}