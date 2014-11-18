// adresses
const int PORT_OFFSET = 0x0100;

const int GPIO_MODULE = 0xFFFF1000;
const int PMR0_OFFSET = 0x10; // peripheral mux 0
const int PMR1_OFFSET = 0x20; // peripheral mux 1
const int ODER_OFFSET = 0x40; // output driver
const int OVR_OFFSET = 0x50; // output value
const int PVR_OFFSET = 0x60; // value
const int PUER_OFFSET = 0x70; // pull-up
const int GFER_OFFSET = 0xC0; // glitch filter

const int PWM_MODULE = 0xFFFF3000;
const int ENA_OFFSET = 0x004; // enable
const int DIS_OFFSET = 0x008; // disable
const int CMR0_OFFSET = 0x200; // channel 0
const int CHANNEL_OFFSET = 0x020; // channel
const int CDTY0_OFFSET = 0x004; // duty cycle
const int CPRD0_OFFSET = 0x008; // period
const int CCNT0_OFFSET = 0x00C; // counter
const int CUPD0_OFFSET = 0x010; // update
