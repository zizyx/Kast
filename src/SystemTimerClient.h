class SystemTimerClient {

	public:
		SystemTimerClient(); // Implementatie called parent constructor nadat die de zichzelf heeft subscribed bij systemTimer.
		setEvent(timedEvent event); //wrapper om de systemTimer::setEvent() // dan kan elke classe self.setEvt() doen
		virtual notifyEvent(timedEvent event);
}