#ifndef ISYSTEM_TIMER_CLIENT_H
#define ISYSTEM_TIMER_CLIENT_H

#include "SystemTimer.h"

struct TimedEvent_t;

class ISystemTimerClient {
	public:
		// virtual ~ISystemTimerClient() {}
		virtual void notifyEvent(TimedEvent_t event) = 0;
};

#endif