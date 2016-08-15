#include "Task.h"

Task::Task()
{
	this->m_is_recycled = 0;
	this->m_nr_req = 0;
	this->m_result_cb_obj = NULL;
	this->m_task_state = -1;
	this->m_task_type = -1;
	this->m_task_discribe = "uninit";
}

Task::~Task()
{
	this->recycle();
}

int Task::recycle()
{
	if(this->m_is_recycled)
		return 0;
	this->m_is_recycled = 1;

	//close all opened resource
	std::list<Agent *>::iterator p_agent_iterator;
	for	(
			p_agent_iterator = this->m_agent_list.begin();
			p_agent_iterator != this->m_agent_list.end();
			p_agent_iterator++
		)
		g_agent_manager.recycle(*p_agent_iterator);
	this->m_agent_list.clear();

	std::list<Agent_nh *>::iterator p_agent_nh_iterator;
	for	(
		p_agent_nh_iterator = this->m_agent_nh_list.begin();
		p_agent_nh_iterator != this->m_agent_nh_list.end();
		p_agent_nh_iterator++
		)
		g_agent_nh_manager.recycle(*p_agent_nh_iterator);
	this->m_agent_nh_list.clear();

	std::list<Listener *>::iterator p_listener_iterator;
	for	(
			p_listener_iterator = this->m_listener_list.begin();
			p_listener_iterator != this->m_listener_list.end();
			p_listener_iterator++
		)
		g_listener_manager.recycle(*p_listener_iterator);
	this->m_listener_list.clear();

	std::list<Timer *>::iterator p_timer_iterator;
	for	(
			p_timer_iterator = this->m_timer_list.begin();
			p_timer_iterator != this->m_timer_list.end();
			p_timer_iterator++
		)
		g_timer_manager.recycle(*p_timer_iterator);
	this->m_timer_list.clear();
	
	return 0;
}

int Task::is_recycled()
{
	return this->m_is_recycled;
}

int Task::get_task_type()
{
	return this->m_task_type;
}

std::string Task::get_task_discribe()
{
	return this->m_task_discribe;
}

int Task::get_nr_req()
{
	return this->m_nr_req;
}

void Task::inc_nr_req()
{
	this->m_nr_req++;
}

void Task::dec_nr_req()
{
	this->m_nr_req--;
}

Agent *Task::open_agent()
{
	Agent *ptr = g_agent_manager.new_agent();
	this->m_agent_list.remove(ptr);
	this->m_agent_list.push_back(ptr);
	return ptr;
}

void Task::close_agent(Agent *ptr)
{
	this->m_agent_list.remove(ptr);
	g_agent_manager.recycle(ptr);
}

Agent_nh *Task::open_agent_nh()
{
	Agent_nh *ptr = g_agent_nh_manager.new_agent_nh();
	this->m_agent_nh_list.remove(ptr);
	this->m_agent_nh_list.push_back(ptr);
	return ptr;
}

void Task::close_agent_nh(Agent_nh *ptr)
{
	this->m_agent_nh_list.remove(ptr);
	g_agent_nh_manager.recycle(ptr);
}

Listener *Task::open_listener()
{
	Listener *ptr = g_listener_manager.new_listener();
	this->m_listener_list.remove(ptr);
	this->m_listener_list.push_back(ptr);
	return ptr;
}

void Task::close_listener(Listener *ptr)
{
	this->m_listener_list.remove(ptr);
	g_listener_manager.recycle(ptr);
}

Timer *Task::open_timer()
{
	Timer *ptr = g_timer_manager.new_timer();
	this->m_timer_list.remove(ptr);
	this->m_timer_list.push_back(ptr);
	return ptr;
}

void Task::close_timer(Timer *ptr)
{
	this->m_timer_list.remove(ptr);
	g_timer_manager.recycle(ptr);
}

