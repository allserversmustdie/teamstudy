#pragma once

#include "Dispatcher.h"

class TimerDispatcher : public ITimerDispatcher
{
public:
	TimerDispatcher();
	virtual ~TimerDispatcher();

	static int DISPATCHER_TIMER_LOG(void* object);
};