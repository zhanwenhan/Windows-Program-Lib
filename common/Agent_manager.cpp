#include "handle_error.h"
#include "Agent_manager.h"


Agent_Manager g_agent_manager;

Agent_Manager::Agent_Manager()
{
	dev_log("agent manager construct");
}

Agent_Manager::~Agent_Manager()
{
	dev_log("agent manager deconstruct");
}

void Agent_Manager::recycle(Agent *ptr)
{	
	this->m_available_agent_list.remove(ptr);
	ptr->recycle();
	dev_log("a agent recycled");
	this->m_recycle_agent_list.remove(ptr);
	this->m_recycle_agent_list.push_back(ptr);
}

void Agent_Manager::clear_recycle()
{
	std::list<Agent *>::iterator p_agent_iterator;
	for	(
			p_agent_iterator = this->m_recycle_agent_list.begin();
			p_agent_iterator != this->m_recycle_agent_list.end();
			p_agent_iterator++
		)
		delete *p_agent_iterator;

	this->m_recycle_agent_list.clear();
}

Agent *Agent_Manager::new_agent()
{
	Agent *ptr = NULL;
	ptr = new Agent();
	this->m_available_agent_list.remove(ptr);
	this->m_available_agent_list.push_back(ptr);
	return ptr;
}
