#include "Motor.h"

Motor::Motor() {
    pwm = new PWM();
}

Motor::~Motor() {}

void Motor::initEnablePin() {
    VINTP GPER = (int*)(GPIO_MODULE + (Configuration::Motor_enabledPinPort ? PORT_OFFSET : 0) + GPER_OFFSET);
    SET_BIT(*GPER, Configuration::Motor_enabledPinPin);
    VINTP ODER = (int*)(GPIO_MODULE + (Configuration::Motor_enabledPinPort ? PORT_OFFSET : 0) + ODER_OFFSET);
    SET_BIT(*ODER, Configuration::Motor_enabledPinPin);
    VINTP OVR = (int*)(GPIO_MODULE + (Configuration::Motor_enabledPinPort ? PORT_OFFSET : 0) + OVR_OFFSET);
    CLEAR_BIT(*OVR, Configuration::Motor_enabledPinPin);
}

bool Motor::init(Configuration::s_MotorConfig* thisMotorConfig_) {
    // init PWM
    pwm->init(thisMotorConfig_->PWMConfig);
    // init register
    OVR = (int*)(GPIO_MODULE + (thisMotorConfig_->directionPinPort ? PORT_OFFSET : 0) + OVR_OFFSET);
    directionPinForwardValue = thisMotorConfig_->directionPinForwardValue;
    directionPinPin = thisMotorConfig_->directionPinPin;
    return true;
}

bool Motor::setSpeed(unsigned char ratioOn) {
    // forward call to PWM
    return pwm->setChannelPWMRatio(ratioOn);
}

unsigned char Motor::getSpeed() {
    // forward call to PWM
    return pwm->getChannelPWMRatio();
}

void Motor::setDirection(bool forward) {
    // set direction pin value
    if (forward == directionPinForwardValue) SET_BIT(*OVR, directionPinPin);
    else CLEAR_BIT(*OVR, directionPinPin);
}

void Motor::setEnabled(bool enabled) {
    // set enable value
    VINTP OVR = (int*)(GPIO_MODULE + (Configuration::Motor_enabledPinPort ? PORT_OFFSET : 0) + OVR_OFFSET);
    if (enabled) SET_BIT(*OVR, Configuration::Motor_enabledPinPin);
    else CLEAR_BIT(*OVR, Configuration::Motor_enabledPinPin);
}

bool Motor::getIsEnabled() {
    // read enable value
    VINTP PVR = (int*)(GPIO_MODULE + (Configuration::Motor_enabledPinPort ? PORT_OFFSET : 0) + OVR_OFFSET);
    return BIT_IS_SET(*PVR, Configuration::Motor_enabledPinPin);
}