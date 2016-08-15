#include "Timer.h"
#include "handle_error.h"

Timer::Timer()
{
	this->m_is_recycled = 0;
	this->m_timer = NULL;
	this->m_ptask = NULL;
	this->m_is_persist = 0;
}

Timer::~Timer()
{
	this->recycle();
}


void Timer::recycle()
{
	if(this->m_is_recycled)
		return;
	this->m_is_recycled = 1;
	if(this->m_timer)
	{
		event_free(this->m_timer);
		this->m_timer = NULL;
	}
	this->m_ptask = NULL;

	return;
}

int Timer::is_recycled()
{
	return this->m_is_recycled;
}



void Timer::timer_cb(evutil_socket_t fd, short what, void *arg)
{
	Timer *pthis = (Timer *)arg;
	pthis->m_cb_obj(pthis);
}



int Timer::initial(Task *ptask, Scheduler &sched, int is_persist, boost::function<void (Timer *)> cb_obj)
{
	this->m_ptask = ptask;
	this->m_cb_obj = cb_obj;
	if(is_persist)
		this->m_is_persist = EV_PERSIST;
	else
		this->m_is_persist = 0;
	this->m_timer = event_new(sched.get_base(), -1, this->m_is_persist, Timer::timer_cb, this);
	if(!this->m_timer)
	{
		handle_error("Timer::initial - error constructing timer");
		return -1;
	}
	return 0;
}

int Timer::begin(Time_value &tv)
{
	struct timeval temp_tv; 
	temp_tv = tv.get_timeval();
	return event_add(this->m_timer, &temp_tv);
}

int Timer::end()
{
	return event_del(this->m_timer);
}

int Timer::pending(Time_value *tv)
{
	struct timeval temp_tv;
	int ret;
	ret = event_pending(this->m_timer, EV_TIMEOUT, &temp_tv);
	tv->set(&temp_tv);
	return ret;
}


int Timer::is_persist()
{
	return this->m_is_persist;
}