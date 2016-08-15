#ifndef __AGENT_MANAGER_H__
#define __AGENT_MANAGER_H__

#include <list>

#include "handle_error.h"
#include "Agent.h"

//factory of agent

class Agent_Manager
{
public:
	Agent_Manager();
	~Agent_Manager();

public:
	//create a agent
	Agent *new_agent();
	//recycle a agent
	void recycle(Agent *ptr);
	//delete all agent recycled
	void clear_recycle();

public:
	std::list<Agent *>	m_available_agent_list;

private:
	std::list<Agent *>	m_recycle_agent_list;
};

extern Agent_Manager g_agent_manager;

#endif