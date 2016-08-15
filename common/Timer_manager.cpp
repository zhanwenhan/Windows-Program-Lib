#include "handle_error.h"
#include "Timer_manager.h"

Timer_Manager g_timer_manager;

Timer_Manager::Timer_Manager()
{
	dev_log("timer manager construct");
}

Timer_Manager::~Timer_Manager()
{
	dev_log("timer manager deconstruct");
}

void Timer_Manager::recycle(Timer *ptr)
{	
	this->m_available_timer_list.remove(ptr);
	ptr->recycle();
	dev_log("a timer recycled");
	this->m_recycle_timer_list.remove(ptr);
	this->m_recycle_timer_list.push_back(ptr);
}

void Timer_Manager::clear_recycle()
{
	std::list<Timer *>::iterator p_timer_iterator;
	for	(
			p_timer_iterator = this->m_recycle_timer_list.begin();
			p_timer_iterator != this->m_recycle_timer_list.end();
			p_timer_iterator++
		)
		delete *p_timer_iterator;

	this->m_recycle_timer_list.clear();
}

Timer *Timer_Manager::new_timer()
{
	Timer *ptr = NULL;
	ptr = new Timer();
	this->m_available_timer_list.remove(ptr);
	this->m_available_timer_list.push_back(ptr);
	return ptr;
}
