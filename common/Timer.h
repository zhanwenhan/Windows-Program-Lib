#ifndef __TIMER_H__
#define __TIMER_H__

#include <event2/event.h>
#include <event2/util.h>
#include <boost/function.hpp>

#include "Scheduler.h"
#include "Time_value.h"

class Task;

//wrap the timer

class Timer
{
public:
	Timer();
	~Timer();

public:
	void recycle();
	int is_recycled();

public:
	static void timer_cb(evutil_socket_t fd, short what, void *arg);

public:
	//initial the timer
	//ptask: owner task
	//sched: scheduler
	//cb_obj: callback function obj
	int initial(Task *ptask, Scheduler &sched, int is_persist, boost::function<void (Timer *)> cb_obj);
	//timer begin
	int begin(Time_value &tv);
	//timer end
	int end();
	//see the time to occur
	int pending(Time_value *tv);
	int is_persist();

private:
	struct event *m_timer;
	int m_is_persist;

	int m_is_recycled;

	boost::function<void (Timer *)> m_cb_obj;
	Task *m_ptask;
};

#endif