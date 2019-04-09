#include "dateTime.h"

struct timedEvent
{
	int eventId;					// ID of event/function to call
	dateTime dateTime;				// datetime when notification will be send to client
	systemTimerClient* client;		// Pointer to class where event should be handled
};