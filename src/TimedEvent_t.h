#ifndef TIMED_EVENT_H
#define TIMED_EVENT_H

#include "DateTime.h"
#include "ISystemTimerClient.h"
#include <iostream>

struct TimedEvent_t
{
	int eventId;					// ID of event/function to call
	datetime_t dateTime;			// datetime when notification will be send to client
	ISystemTimerClient* client;		// Pointer to class where event should be handled

	TimedEvent_t() {}
	TimedEvent_t(int eventId, datetime_t dateTime, ISystemTimerClient *client) :
		eventId(eventId),
		dateTime(dateTime),
		client(client) {}

	TimedEvent_t& operator=(const TimedEvent_t& t)
	{
		eventId = t.eventId;
		dateTime = t.dateTime;
		client = t.client;

		return *this;
	}

	bool operator==(const TimedEvent_t& t) const
	{
		return (eventId == t.eventId);
	}
};

#endif