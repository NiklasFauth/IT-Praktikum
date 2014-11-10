/*
	Timer class for AVR32UC3B
	
	DEPENDENCIES:
	An interrupt handling system has to be set up. This class only manages the timer registers.
	
	EXAMPLE INTERRUPT HANDLING SYSTEM:
	See main.cpp.example, exception.s.example

 */

#ifndef TIMER_H_
#define TIMER_H_

class Timer {
private:

public:
	Timer();
	~Timer();
	static void resetInterruptFlag();
	bool initTimer( unsigned long frequency );
	void setIsTimerEnabled( bool enabled );
	void setIsTimerInterruptEnabled( bool enabled );
};

#endif /* TIMER_H_ */