#include "Motor.h"

Motor::Motor() {
    pwm = new PWM();
}

Motor::~Motor() {

}

void Motor::initEnablePin() {

}

bool Motor::init( Configuration::s_MotorConfig* thisMotorConfig_ ) {
    pwm->init(thisMotorConfig_->PWMConfig);
    OVR = (int*) (GPIO_MODULE + (thisMotorConfig_->directionPinPort ? PORT_OFFSET : 0) + OVR_OFFSET);
    directionPinForwardValue = thisMotorConfig_->directionPinForwardValue;
}

bool Motor::setSpeed( unsigned char ratioOn ) {
    if (ratioOn < 0 && ratioOn > PWMConfig->maxPWMRatio) return false;
    pwm->setChannelPWMRatio(ratioOn);
	return true;
}

unsigned char Motor::getSpeed() {

	return 0;
}

void Motor::setDirection( bool forward ) {
    if (forward == directionPinForwardValue) SET_BIT(*OVR, thisMotorConfig_->directionPinPin);
    else CLEAR_BIT(*OVR, thisMotorConfig_->directionPinPin);
}

void Motor::setEnabled( bool enabled ) {
    pwm->setChannelEnabled(enabled);
}

bool Motor::getIsEnabled() {
	return pwm->isChannelEnabled();
}
