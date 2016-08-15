#include <event2/event.h>

#include "Scheduler.h"
#include "handle_error.h"

Scheduler g_scheduler;

Scheduler::Scheduler()
{
	this->m_eb = NULL;
}

Scheduler::~Scheduler()
{
	if(this->m_eb)
		event_base_free(this->m_eb);
}

struct event_base *Scheduler::get_base()
{
	return this->m_eb;
}

int Scheduler::initial()
{
	this->m_eb = event_base_new();
	if(!this->m_eb)
	{
		handle_error("int Scheduler::initial()");
		return -1;
	}
	return 0;
}

int Scheduler::run()
{
	return event_base_dispatch(this->m_eb);
}
