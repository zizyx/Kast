#ifndef SYSTEMTIMER_H
#define SYSTEMTIMER_H

class systemTimer {
	public:
		systemTimer();
		~systemTimer(){};

		void checkSystemTimer();
		void createEvent(struct timedEvent);
		
	private:
		void checkEvents();
};

#endif
