#include "Motor.h"

Motor::Motor() {
    pwm = new PWM();
}

Motor::~Motor() {

}

void Motor::initEnablePin() {

}

bool Motor::init( Configuration::s_MotorConfig* thisMotorConfig_ ) {
    // init PWM
    pwm->init(thisMotorConfig_->PWMConfig);
    // init register
    OVR = (int*) (GPIO_MODULE + (thisMotorConfig_->directionPinPort ? PORT_OFFSET : 0) + OVR_OFFSET);
    directionPinForwardValue = thisMotorConfig_->directionPinForwardValue;
    directionPinPin = thisMotorConfig_->directionPinPin;
}

bool Motor::setSpeed( unsigned char ratioOn ) {
    // forward call to PWM
    return pwm->setChannelPWMRatio(ratioOn);
}

unsigned char Motor::getSpeed() {
    // forward call to PWM
	return pwm->getChannelPWMRatio();
}

void Motor::setDirection( bool forward ) {
    // set direction pin value
    if (forward == directionPinForwardValue) SET_BIT(*OVR, directionPinPin);
    else CLEAR_BIT(*OVR, directionPinPin);
}

void Motor::setEnabled( bool enabled ) {
    // forward call to PWM
    pwm->setChannelEnabled(enabled);
}

bool Motor::getIsEnabled() {
    // forward call to PWM
	return pwm->isChannelEnabled();
}
