#include "Timer.h"

// Adressen
// Frage: Woher weiﬂ ich, welche Nummer ich nehme (0 oder 1 oder 2..) Was ist dabei der Unterschied?
volatile unsigned int* TC = (volatile unsigned int*) 0xFFF3800;
volatile unsigned int* CCR0= (volatile unsigned int*) 0xFFF3800; //Channel Control Register
volatile unsigned int* CMR0= (volatile unsigned int*) 0xFFF3804;
volatile unsigned int* CV0= (volatile unsigned int*) 0xFFF3810;
volatile unsigned int* RA0= (volatile unsigned int*) 0xFFF3814;
volatile unsigned int* RB0= (volatile unsigned int*) 0xFFF3818;
volatile unsigned int* RC0= (volatile unsigned int*) 0xFFF381C;
volatile unsigned int* SR0= (volatile unsigned int*) 0xFFF3820; //Channel Status Register
volatile unsigned int* IER0= (volatile unsigned int*) 0xFFF3824; //Channel Interrupt Enable Register
volatile unsigned int* IDR0= (volatile unsigned int*) 0xFFF3828;
volatile unsigned int* IMR0= (volatile unsigned int*) 0xFFF382C;

Timer::Timer() {

}

Timer::~Timer() {

}

// RC value will determinate
bool prepareTimer(unsigned long frequency){
	setIsTimerInterruptEnabled(true);
	SET_BIT(*CMR0,15) //*CMR0 |= 1 << 15; // WAVE: Waveform mode is enabled
	SET_BIT(*CMR0,14) //*CMR0 |= 1 << 14; // WAVSEL: RC is chosen as the maximum value (UP mode with automatic trigger on RC Compare)
	SET_BIT(*CMR0,1) //*CMR0 |= 1 << 1; // TCCLKS: TIMER_CLOCK3 is chosen, the clock choise is define in Configuration.cpp
	if (2000000/frequency <= 65535){  // > 2^16 - 1 -> There are only 16 bits availabe
		*RC0 = 2000000/frequency; // frequency will hand over // One Interrupt per second happens when RC=2 MHz, if we want 5 Interrupts (f=5 Hz) we have to divide 2 MHz with 5
		return true;	
	}
	else if (2000000/frequency > 65535){
		return false;
	}
}
void cleanUpTimer(void){
	*CCR0 = 0x0;
	*CMR0 = 0x0;
	*CV0 = 0x0;
	*RA0 = 0x0;
	*RB0 = 0x0;
	*RC0 = 0x0;
	*SR0 = 0x0;
	*IER0 = 0x0;
	*IDR0 = 0x0;
	*IMR0 =0x0;
}

bool Timer::initTimer( unsigned long frequency ) {
	if (prepareTimer(frequency) == true){
		setIsTimerEnabled(true);
		setIsTimerInterruptEnabled(true);
		return true;
	}
	else if (prepareTimer(frequency) == false){
		return false;	
	}
}

void Timer::setIsTimerEnabled( bool enabled ) {
if (enabled == true){
	SET_BIT(*CCR0, 0); //Entpricht: *CCR0 |= 1; CLKEN: This bit will enable the clock 
	SET_BIT(*CCR0, 2); //Entspricht: *CCR0 |= 1 << 2; SWTRG: The clock is started
}
else{
	SET_BIT(*CCR0, 1); //Entspricht: *CCR0 |= 1 << 1; CLKDIS: This bit will disable the clock
}
}

void Timer::setIsTimerInterruptEnabled( bool enabled ) {
if (enabled == true){
	SET_BIT(*IER0, 4); //Entspricht *IER0 |= 1 << 4; // CPCS:This bit is set when an RC Compare has occured 
}
else{
	SET_BIT(*IDR0, 4); //Entspricht: *IDR0 |= 1 << 4; //CPCS: This bit ill clear the sorresponding bit in IMR (Interrupt Mask Register, read-only)
}
}

void Timer::resetInterruptFlag(void) {
	*SR0; //Reading the Status Register will also clear the interrupt bit for the corresponding interrupts.
}


//If timer is enabled, the return value is true
bool getIsTimerEnabled(void){
	return (bool)(*SR0 & 0x10000); // Bit 16 CLKSTA is set when the clock is enabled -> must be 1 (2^16 = 16^4)
}

//If interrupts are enabled, the return value is true
bool getIsInterruptEnabled(void){
	return (bool)(*IMR0 & 0x10); // Bit 4 CPCS is set when an RC Compare has occured (2^4 = 16^1)
}

void cleanUp(void){
	*CCR0 = 0x0;
	*CMR0 = 0x0;
	*CV0 = 0x0;
	*RA0 = 0x0;
	*RB0 = 0x0;
	*RC0 = 0x0;
	*SR0 = 0x0;
	*IER0 = 0x0;
	*IDR0 = 0x0;
	*IMR0 =0x0;
}
