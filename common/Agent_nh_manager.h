#ifndef __AGENT_NH_MANAGER_H__
#define __AGENT_NH_MANAGER_H__

#include <list>

#include "handle_error.h"
#include "Agent_nh.h"

//factory of agent_nh

class Agent_nh_Manager
{
public:
	Agent_nh_Manager();
	~Agent_nh_Manager();

public:
	//create a agent_nh
	Agent_nh *new_agent_nh();
	//recycle a agent_nh
	void recycle(Agent_nh *ptr);
	//delete all agent_nh recycled
	void clear_recycle();

public:
	std::list<Agent_nh *>	m_available_agent_nh_list;

private:
	std::list<Agent_nh *>	m_recycle_agent_nh_list;
};

extern Agent_nh_Manager g_agent_nh_manager;

#endif