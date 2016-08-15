#include <iostream>
#include <boost/bind.hpp>

#include "common/Scheduler.h"
#include "common/Agent_manager.h"
#include "common/Timer_manager.h"
#include "common/Listener_manager.h"
#include "common/Task_manager.h"

#include "Persist_timer_task.h"

extern int g_persist_timeout;

Persist_timer_task::Persist_timer_task()
{
	this->m_ptimer = NULL;
	this->m_task_type = 1;
	this->m_task_discribe = "persist timer task";
}

Persist_timer_task::~Persist_timer_task()
{

}

int Persist_timer_task::initial(void *arg, boost::function<void (int, void *)> result_cb_obj)
{
	this->m_ptimer = this->open_timer();
	if(this->m_ptimer->initial(this, g_scheduler, 1, boost::bind(&Persist_timer_task::persist_timer_cb, this, _1)))
	{
		handle_error("Persist_timer_task::initial - initial");
		return -1;
	}
	Time_value timer_tv(g_persist_timeout, 0);
	this->m_ptimer->begin(timer_tv);
	return 0;
}

void Persist_timer_task::persist_timer_cb(Timer *ptimer)		//do cleaner work
{
	g_agent_manager.clear_recycle();
	g_listener_manager.clear_recycle();
	g_timer_manager.clear_recycle();
	g_task_manager.clear_recycle();
	dev_log("persist timer");
}