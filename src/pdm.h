#ifndef PDM_H
#define PDM_H

#include <avr/io.h>
#include <inttypes.h>
#include <string.h>

//PB1
#define OUTPUT 1
#define INPUT 0
#define HIGH 1
#define LOW 0
#define NO_PULL_UP 0
#define PULL_UP 1
//0C2A

class pdm {
	public:
		static pdm *getInstance();
		void setupFanPin(uint8_t pinNr);
		void setupPinTimer();
		void setPdm(uint8_t pdm);
	private:
		static pdm *instance;
		pdm();
};

#endif