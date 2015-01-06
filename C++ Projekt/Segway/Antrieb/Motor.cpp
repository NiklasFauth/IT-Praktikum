#include "Motor.h"

Motor::Motor() {
    pwm = new PWM();
}

Motor::~Motor() {}

void Motor::initEnablePin() {
    VINTP GPER = (VINTP)(GPIO_MODULE + (Configuration::Motor_enabledPinPort ? GPIO_PORT_OFFSET : 0) + GPIO_GPER_OFFSET);
    SET_BIT(*GPER, Configuration::Motor_enabledPinPin);
    VINTP ODER = (VINTP)(GPIO_MODULE + (Configuration::Motor_enabledPinPort ? GPIO_PORT_OFFSET : 0) + GPIO_ODER_OFFSET);
    SET_BIT(*ODER, Configuration::Motor_enabledPinPin);
    VINTP OVR = (VINTP)(GPIO_MODULE + (Configuration::Motor_enabledPinPort ? GPIO_PORT_OFFSET : 0) + GPIO_OVR_OFFSET);
    CLEAR_BIT(*OVR, Configuration::Motor_enabledPinPin);
}

bool Motor::init(Configuration::s_MotorConfig* thisMotorConfig_) {
    // init PWM
    pwm->init(thisMotorConfig_->PWMConfig);
    // init register
    OVR = (VINTP)(GPIO_MODULE + (thisMotorConfig_->directionPinPort ? GPIO_PORT_OFFSET : 0) + GPIO_OVR_OFFSET);
    directionPinForwardValue = thisMotorConfig_->directionPinForwardValue;
    directionPinPin = thisMotorConfig_->directionPinPin;
    return true;
}

bool Motor::setSpeed(unsigned char ratioOn) {
    // forward call to PWM
    return pwm->setChannelPWMRatio(ratioOn, true);
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
    VINTP OVR = (VINTP)(GPIO_MODULE + (Configuration::Motor_enabledPinPort ? GPIO_PORT_OFFSET : 0) + GPIO_OVR_OFFSET);
    if (enabled) SET_BIT(*OVR, Configuration::Motor_enabledPinPin);
    else CLEAR_BIT(*OVR, Configuration::Motor_enabledPinPin);
}

bool Motor::getIsEnabled() {
    // read enable value
    VINTP PVR = (VINTP)(GPIO_MODULE + (Configuration::Motor_enabledPinPort ? GPIO_PORT_OFFSET : 0) + GPIO_OVR_OFFSET);
    return BIT_IS_SET(*PVR, Configuration::Motor_enabledPinPin);
}