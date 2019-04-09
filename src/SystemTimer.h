#ifndef SYSTEMTIMER_H
#define SYSTEMTIMER_H

class SystemTimer {
	public:
		SystemTimer();
		~SystemTimer(){};

		void checkSystemTimer();
		void createEvent(struct timedEvent);
		
	private:
		void checkEvents();
};

#endif
