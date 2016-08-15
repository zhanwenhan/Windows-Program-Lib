#include "handle_error.h"
#include "Listener_manager.h"


Listener_Manager g_listener_manager;

Listener_Manager::Listener_Manager()
{
	dev_log("listener manager construct");
}

Listener_Manager::~Listener_Manager()
{
	dev_log("listener manager deconstruct");
}

void Listener_Manager::recycle(Listener *ptr)
{	
	this->m_available_listener_list.remove(ptr);
	ptr->recycle();
	dev_log("a listener recycled");
	this->m_recycle_listener_list.remove(ptr);
	this->m_recycle_listener_list.push_back(ptr);
}

void Listener_Manager::clear_recycle()
{
	std::list<Listener *>::iterator p_listener_iterator;
	for	(
			p_listener_iterator = this->m_recycle_listener_list.begin();
			p_listener_iterator != this->m_recycle_listener_list.end();
			p_listener_iterator++
		)
		delete *p_listener_iterator;

	this->m_recycle_listener_list.clear();
}

Listener *Listener_Manager::new_listener()
{
	Listener *ptr = NULL;
	ptr = new Listener();
	this->m_available_listener_list.remove(ptr);
	this->m_available_listener_list.push_back(ptr);
	return ptr;
}
