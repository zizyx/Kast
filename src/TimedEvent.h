#ifndef TIMED_EVENT_H
#define TIMED_EVENT_H

#include "DateTime.h"
#include "SystemTimerClient.h"

struct timedEvent
{
	int eventId;					// ID of event/function to call
	datetime_t dateTime;			// datetime when notification will be send to client
	SystemTimerClient* client;		// Pointer to class where event should be handled
};

#endif