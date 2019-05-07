/////////////////////////////////////////////////////////////////////////////////
// SystemTimer
/////////////////////////////////////////////////////////////////////////////////
#include "SystemTimer.h"
#include "TimedEvent_t.h"

// sTEST
#include <iostream>
//

SystemTimer::SystemTimer() {

}

void SystemTimer::checkSystemTimer() {
	checkEvents();
}

void SystemTimer::createEvent(struct TimedEvent_t event) {
	events.push_back(event);
}

void SystemTimer::checkEvents() {
  // get time
}

std::vector<TimedEvent_t> SystemTimer::getEvents() {
	return events; 	
}


// class specific
// virtual notifyEvent(timer, int event_id) {
// 	switch(event_id) {
// 	}
// }
