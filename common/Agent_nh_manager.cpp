#include "handle_error.h"
#include "Agent_nh_manager.h"


Agent_nh_Manager g_agent_nh_manager;

Agent_nh_Manager::Agent_nh_Manager()
{
	dev_log("agent_nh manager construct");
}

Agent_nh_Manager::~Agent_nh_Manager()
{
	dev_log("agent_nh manager deconstruct");
}

void Agent_nh_Manager::recycle(Agent_nh *ptr)
{	
	this->m_available_agent_nh_list.remove(ptr);
	ptr->recycle();
	dev_log("a agent_nh recycled");
	this->m_recycle_agent_nh_list.remove(ptr);
	this->m_recycle_agent_nh_list.push_back(ptr);
}

void Agent_nh_Manager::clear_recycle()
{
	std::list<Agent_nh *>::iterator p_agent_nh_iterator;
	for	(
			p_agent_nh_iterator = this->m_recycle_agent_nh_list.begin();
			p_agent_nh_iterator != this->m_recycle_agent_nh_list.end();
			p_agent_nh_iterator++
		)
		delete *p_agent_nh_iterator;

	this->m_recycle_agent_nh_list.clear();
}

Agent_nh *Agent_nh_Manager::new_agent_nh()
{
	Agent_nh *ptr = NULL;
	ptr = new Agent_nh();
	this->m_available_agent_nh_list.remove(ptr);
	this->m_available_agent_nh_list.push_back(ptr);
	return ptr;
}
