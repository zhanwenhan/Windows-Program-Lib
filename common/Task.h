#ifndef __TASK_H__
#define __TASK_H__

#include <list>

#include <boost/function.hpp>

#include <event2/util.h>

#include "Sock_addr.h"
#include "Agent.h"
#include "Agent_nh.h"
#include "Listener.h"
#include "Timer.h"
#include "Agent_manager.h"
#include "Agent_nh_manager.h"
#include "Listener_manager.h"
#include "Timer_manager.h"

class Task
{
public:
	Task();
	virtual ~Task();

public:
	int recycle();
	int is_recycled();

public:
	//virtual! init the task 
	//arg: argument to the task
	//result_cb_obj: when task over, call it
	virtual int initial(void *arg, boost::function<void (int, void *)> result_cb_obj) = 0;

public:
	//get task type
	int get_task_type();
	//get task describe
	std::string get_task_discribe();

	//the req number represent the reference count
	int get_nr_req();
	void inc_nr_req();
	void dec_nr_req();

protected:
	//open and close a agent for the task
	Agent *open_agent();
	void close_agent(Agent *ptr);

	//open and close a agent_nh for the task
	Agent_nh *open_agent_nh();
	void close_agent_nh(Agent_nh *ptr);

	//open and close a listener for the task
	Listener *open_listener();
	void close_listener(Listener *ptr);

	//open and close a timer for the task
	Timer *open_timer();
	void close_timer(Timer *ptr);

protected:
	int m_task_type;
	std::string m_task_discribe;

	int m_task_state;

	int m_nr_req;
	int m_is_recycled;

	boost::function<void (int, void *)> m_result_cb_obj;

	std::list<Agent *> m_agent_list;			//list of agent
	std::list<Agent_nh *> m_agent_nh_list;		//list of agent_nh
	std::list<Listener *> m_listener_list;		//list of listener
	std::list<Timer *> m_timer_list;			//list of timer
};

#endif