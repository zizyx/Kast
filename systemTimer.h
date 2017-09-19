#ifndef SYSTEMTIMER_H
#define SYSTEMTIMER_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <string.h>

#define OUTPUT 1
#define INPUT 0
#define HIGH 1
#define LOW 0
#define NO_PULL_UP 0
#define PULL_UP 1

#define OCR_PRESCALER_10HZ	25000

class systemTimer {
	public:
		systemTimer();
		static systemTimer *getInstance();
		void enable();
		void disable();
	private:
		static systemTimer instance;
		void init();
};

#endif
