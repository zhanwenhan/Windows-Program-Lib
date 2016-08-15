#ifndef __PERSIST_TIMER_TASK_H__
#define __PERSIST_TIMER_TASK_H__

#include <string>

#include "common/Task.h"
#include "common/Timer.h"

class Persist_timer_task : public Task
{
public:
	Persist_timer_task();
	~Persist_timer_task();

public:
	int initial(void *arg, boost::function<void (int, void *)> result_cb_obj);

public:
	void persist_timer_cb(Timer *ptimer);

protected:
	Timer *m_ptimer;
};

#endif