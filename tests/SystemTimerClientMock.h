#include "gmock/gmock.h"
#include "../src/ISystemTimerClient.h"
#include "../src/TimedEvent_t.h"

class SystemTimerClientMock : public ISystemTimerClient {
 	public:
		MOCK_METHOD1(notifyEvent, void(TimedEvent_t event));
		MOCK_METHOD1(createEvent, void(TimedEvent_t event));
};