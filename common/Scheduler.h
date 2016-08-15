#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <event2/event.h>

//wrap the event base

class Scheduler
{
public:
	Scheduler();
	~Scheduler();

public:
	//initial the scheduler
	int initial();
	//schedule begin
	int run();
	//get the event base
	struct event_base *get_base();

private:
	struct event_base *m_eb;
};

extern Scheduler g_scheduler;

#endif
