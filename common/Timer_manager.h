#ifndef __TIMER_MANAGER_H__
#define __TIMER_MANAGER_H__

#include <list>

#include "handle_error.h"
#include "Timer.h"

//factory of timer

class Timer_Manager
{
public:
	Timer_Manager();
	~Timer_Manager();

public:
	//create a timer
	Timer *new_timer();
	//recycle a timer
	void recycle(Timer *ptr);
	void clear_recycle();

public:
	std::list<Timer *>	m_available_timer_list;

private:
	std::list<Timer *>	m_recycle_timer_list;
};

extern Timer_Manager g_timer_manager;


#endif