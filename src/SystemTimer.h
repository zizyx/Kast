#ifndef SYSTEMTIMER_H
#define SYSTEMTIMER_H

#include "TimedEvent_t.h"
#include <vector>

class SystemTimer {
	public:
		SystemTimer();
		~SystemTimer(){};

		void checkSystemTimer();
		void createEvent(struct TimedEvent_t event);
		std::vector<TimedEvent_t> getEvents();
	private:
		std::vector<TimedEvent_t> events;
		void checkEvents();
};

#endif
