/*
	Timer class for AVR32UC3B
	
	DEPENDENCIES:
	An interrupt handling system has to be set up. This class only manages the timer registers.
	
	EXAMPLE INTERRUPT HANDLING SYSTEM:
	See main.cpp.example, exception.s.example

 */
#include "../Configuration/Configuration.h";
#ifndef TIMER_H_
#define TIMER_H_

class Timer {
private:

public:
	Timer();
	~Timer();
	bool prepareTimer(unsigned long frequency);
	void cleanUpTimer(void);
	bool initTimer( unsigned long frequency );
	void setIsTimerEnabled( bool enabled );
	void setIsTimerInterruptEnabled( bool enabled );
	static void resetInterruptFlag(void);
	bool getIsTimerEnabled(void);
	bool getIsInterruptEnabled(void);
	void cleanUp(void);
	
};

#endif /* TIMER_H_ */