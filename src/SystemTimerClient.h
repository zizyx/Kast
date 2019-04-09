#ifndef SYSTEM_TIMER_CLIENT_H
#define SYSTEM_TIMER_CLIENT_H

class SystemTimerClient {
	public:
		SystemTimerClient(); // Implementatie called parent constructor nadat die de zichzelf heeft subscribed bij systemTimer.
		void setEvent(timedEvent event); //wrapper om de systemTimer::setEvent() // dan kan elke classe self.setEvt() doen
		virtual void notifyEvent(timedEvent event);
};

#endif