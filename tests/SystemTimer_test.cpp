#include <gmock/gmock.h>
#include "../src/SystemTimer.h"
#include "../src/TimedEvent_t.h"
#include "../tests/SystemTimerClientMock.h"

TEST(SystemTimerClient_test, createEvent)
{
	SystemTimer 			timer;
	SystemTimerClientMock 	mockSystemTimerClient;
	datetime_t				dateTime( 4, 1, 1, 1, 3, 2, 2001);
	TimedEvent_t			event(1, dateTime, &mockSystemTimerClient);

	timer.createEvent(event);

	// Get list of events and check if it is in there.
    std::vector<TimedEvent_t> events = timer.getEvents();
    std::vector<TimedEvent_t>::iterator v = std::find(events.begin(), events.end(), event);

    EXPECT_NE(v, events.end()); //Here we now it found nothing.
    EXPECT_EQ(*v, event);  //Here we check if we got the right thing.
}

TEST(SystemTimerClient_test, testCallbackFromCreateEvent)
{
	SystemTimer 			timer;
	SystemTimerClientMock 	mockSystemTimerClient;
	datetime_t				now( 1, 1, 1, 1, 3, 2, 2001);
	datetime_t				futureEvent( 4, 1, 1, 1, 3, 2, 2001);
	TimedEvent_t			event(1, futureEvent, &mockSystemTimerClient);

	// Set event in future
	timer.createEvent(event);

	// EXPECT_CALL(mockDS_3231, getTime().ReturnOnce(testing::Return(now)));
	timer.checkSystemTimer();

	EXPECT_CALL(mockSystemTimerClient, notifyEvent(event));

    ASSERT_THAT(1, 1);
}